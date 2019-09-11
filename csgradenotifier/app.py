#Changed libraries a bunch, some of these might be removeable
import os
import sys
import json
import time
import random
import requests
import datetime
from bs4 import BeautifulSoup
from pymongo import MongoClient
from urllib.parse import urlencode
from urllib.request import Request, urlopen

from flask import Flask, request

LOGIN_URL = "https://ualearn.blackboard.com/webapps/login/"

def getbbNonce():
    session_requests = requests.session()
    loginPage = session_requests.get(LOGIN_URL)
    soup = BeautifulSoup(loginPage.content, 'html.parser')
    nonceElement = soup.find('input', attrs={'name':'blackboard.platform.security.NonceUtil.nonce'})
    nonce = nonceElement["value"]
    print(nonce)
    return nonce, session_requests


#Logs in to your blackboard account with your login info
#put username and password in heroku environment variables
def login():
    nonce, session_requests = getbbNonce()
    # Create payload
    payload = {
        "user_id": os.getenv('USERNAME'), 
        "password": os.getenv('PASSWORD'), 
        "login": "Login",
        "action": "login",
        "new_loc": "",
        "blackboard.platform.security.NonceUtil.nonce": nonce
    }
    # Perform login
    result = session_requests.post(LOGIN_URL, data = payload)
    return result, session_requests

#scrapes gradebook html page.  Collects more data than I store, but you could add these fields
#to the database
def getClassGrades(id, session_requests):
    base = "https://ualearn.blackboard.com/webapps/bb-mygrades-BBLEARN/myGrades?course_id=_"
    end = "_1&stream_name=mygrades&is_stream=false"
    url = base + str(id) + end
    result = session_requests.get(url)
    soup = BeautifulSoup(result.content, 'html.parser')
    names = soup.find('div', attrs={'id':'grades_wrapper'})
    gradeDivs = names.find_all('div', attrs={'class':'sortable_item_row graded_item_row row expanded'})
    upcomingDivs = names.find_all('div', attrs={'class':'sortable_item_row upcoming_item_row row expanded'})
    gradeList = []
    for divs in gradeDivs:
        entry = {}
        entry["graded"] = True
        name = divs.find('div', attrs={'class':'cell gradable'})
        rubric = name.find('div',attrs={'class':'eval-links horizontal'})
        if rubric != None:
            rubric.decompose()
        dueDate = ''
        activityType = name.find('div', attrs={'class':'activityType'})
        if activityType != None:
            dueDate = activityType.text
            activityType.decompose()
        category = ''
        itemCat = name.find('div', attrs={'class':'itemCat'})
        if itemCat != None:
            category = itemCat.text
            itemCat.decompose()
            entry["category"] = category.strip().rstrip()
        
        points = divs.find('span', attrs={'class':'grade'})
        if points != None:
            points = points.text.strip().rstrip()
        total = divs.find('span', attrs={'class':'pointsPossible clearfloats'})
        if total != None:
            total = total.text.replace('/', '').strip().rstrip()
        name = name.text.strip().rstrip()
        entry["name"] = name.strip().rstrip()
        if points != None:
            entry["points"] = points
        if total != None:
            entry["total"] = total
        if total != None and points != None:
            entry["score"] = str(round(100 * float(points) / float(total), 2 ))
        insert = {}
        insert["entry"] = entry
        gradeList.append(entry)

    # Don't want upcoming grades for the time being
    # for divs in upcomingDivs:
    #     entry = {}
    #     name = divs.find('div', attrs={'class':'cell gradable'})
    #     rubric = name.find('div',attrs={'class':'eval-links horizontal'})
    #     if rubric != None:
    #         rubric.decompose()
    #     dueDate = ''
    #     activityType = name.find('div', attrs={'class':'activityType'})
    #     if activityType != None:
    #         dueDate = activityType.text
    #         activityType.decompose()
    #     category = ''
    #     itemCat = name.find('div', attrs={'class':'itemCat'})
    #     if itemCat != None:
    #         category = itemCat.text
    #         itemCat.decompose()
    #     name = name.text.strip().rstrip()
    #     entry["name"] = name
    #     entry["graded"] = False
    #     insert = {}
    #     insert["entry"] = entry
    #     gradeList.append(entry)
    return gradeList

#semester is 4 digit year plus 40 for fall or 10 for spring
#ex spring 2019 would be 201910
#function scrapes html page
def getSemesterCourses(year, term, session):
    season = 10 if term == "spring" else 40
    termID = str(year) + str(season)
    url = "https://ualearn.blackboard.com/webapps/portal/execute/tabs/tabAction"
    #data = "action=refreshAjaxModule&modId=_4_1&tabId=_1_1&tabId=_1_1&tab_tab_group_id=_1_1"
    data = "action=refreshAjaxModule&modId=_4_1&tabId=_1_1&tab_tab_group_id=_1_1"
    page = session.get(url, params=data)
    raw = page.text
    raw = raw[raw.find("<div"):]
    soup = BeautifulSoup(raw, 'html.parser')
    semesters = soup.find_all(attrs={'class':'termHeading-coursefakeclass'})
    classes = ''
    for div in soup.find_all('span', attrs={'class':'hideoff'}):
        div.decompose()
    for sem in semesters:
        ID = sem.find(attrs={'class':'termToggleLink itemHead itemHeadOpen'})
        semID = ID.text.strip()
        if semID == termID:
            value = ID['id']
            value = value[value.find('_'):]
            semDiv = soup.find('div', attrs={'id':value})
            classes = semDiv.find_all('a')
            break        
    termDict = {}
    for CLASS in classes:
        link = CLASS['href'].strip()
        link = link[link.find('_')+1:link.rfind('_')]
        title = CLASS.text[CLASS.text.find('-')+1:CLASS.text.rfind('-')]
        title = title.replace('-', ' ')
        termDict[title] = link
    return termDict
    


app = Flask(__name__)
# Webhook, updates grade listing when a message is sent from specific owner
#Optional, could schedule job in heroku with heroku scheduler
@app.route('/', methods=['POST'])
def webhook():
    data = request.get_json()
    log('Recieved {}'.format(data))

    if data['sender_id'] == '6703311':
        main()
    return "ok", 200

#Sends notification through groupme's api
def send_message(msg, course):
    courseBotIds = os.getenv('GROUPME_BOT_ID')
    courseBotIds = courseBotIds.split(',')
    courseNames = os.getenv('GROUPME_BOT_ID_NAMES')
    courseNames = courseNames.split(',')
    courseMap = {}
    for a, b in zip(courseNames, courseBotIds):
        courseMap[a] = b
    if course not in courseMap:
        log("Course not mapped to a bot id")
        return
    botID = courseMap[course]
    url  = 'https://api.groupme.com/v3/bots/post'
    payload = {
                    'bot_id' : botID,
                    'text'   : msg,
                 }
    log(msg)
    request = Request(url, urlencode(payload).encode())
    json = urlopen(request).read().decode()

def log(msg):
    print(str(msg))
    sys.stdout.flush()

def buildCollection(termDict, session_requests, course):
    gradeList = getClassGrades(termDict[course], session_requests)
    return gradeList

def updateCollection(dbCollection, collection, course):
    for document in collection:
        count = dbCollection.count({'name':document['name']})
        if count == 0:
            dbCollection.insert(document)
            msg = str("Grade for " + document['name'] + " has been posted")
            #comment out line below to not spam groups while debugging
            send_message(msg, course)
            log(msg)

def main():
    #Course titles should be in the environment variables, comma seperated
    #Ex: Key: "GROUPME_BOT_ID_NAMES"  Value: CS 470,CS 301
    #Same should be done with Bot ID's.  Bot ID's and course names should be
    #ordered such that a course name's bot ID corresponds to the same index
    #in the arrays resulting from .split(',')
    YEAR = 2019
    SEMESTER = "fall"
    courses = os.getenv('GROUPME_BOT_ID_NAMES')
    courses = courses.split(',')
    result, session_requests = login()
    #course = "CS 470"
    #database name from mLab (Free mongodb heroku service)
    db = "heroku_4nnw7jjc"
    #MONGO_URI automatically added to heroku environment variables
    #when mLab is added
    client = MongoClient(os.getenv('MONGODB_URI'))
    db = client[db]
    for course in courses:
        dbCollection = db[course.replace(' ', '')]
        #Build list of courses
        termDict = getSemesterCourses(YEAR, SEMESTER, session_requests)
        #print(termDict)
        collection = {}
        if course in termDict:
            #build local collection of grades
            collection = buildCollection(termDict, session_requests, course)
        else:
            log("Course not found in semester course list")
        #compares local collection against collection in mongodb instance
        updateCollection(dbCollection, collection, course)

main()