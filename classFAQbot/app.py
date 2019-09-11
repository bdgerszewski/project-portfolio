import os
import sys
import json
import random
import datetime
from urllib.parse import urlencode
from urllib.request import Request, urlopen

from flask import Flask, request

app = Flask(__name__)


# Webhook
@app.route('/', methods=['POST'])
def webhook():
  data = request.get_json()
  log('Recieved {}'.format(data))
  msg = ''
  if data['name'] != 'Cordes Bot': # the bot is not to respond to itself
    text = data['text'].lower()
    if 'cordes' in text: # only respond if cordes is "mentioned"
      msg = get_res(text)
  send_message(msg)
  return "ok", 200

# The FAQ section
def get_res(text):
  now = datetime.datetime.now()
  content = ''
  day = int(now.day)
  month = int(now.month)
  year = int(now.year)
  if '@cordes' in text:
    content = 'I hear ya, but i\'m still in alpha.  Ask Ben to add a feature.'
  if ('when' in text or 'where' in text) and 'final' in text:
    content = 'Final Exam is on Monday, April 30th (8:00am-10:30am).  Might be in North Lawn 1000?' #update location 
  if 'zybooks' in text or 'chapter' in text:
    
    if month == 2:
      if day <= 5 or 'chapter 4' in text:
        chapter = '4'
        date = 'Feb 5'
      if day > 5 and day <= 12 or 'chapter 5' in text:
        chapter = '5'
        date = 'Feb 12'
      if day > 12 and day <= 19 or 'chapter 6' in text:
        chapter = '6'
        date = 'Feb 19'
      if day > 19 and day <= 26 or 'chapter 7' in text:
        chapter = '7'
        date = 'Feb 19'
      if day > 26 or 'chapter 8' in text:
        chapter = '8'
        date = 'Mar 5'
    if month == 3:
      if day <= 5 or 'chapter 8' in text:
        chapter = '8'
        date = 'Mar 5'
      if day > 5 or 'chapter 9' in text:
        chapter = '9'
        date = 'Apr 2'
    if month == 4:
      if day <= 2 or 'chapter 9' in text:
        chapter = '9'
        date = 'Apr 2'
      if day > 2 and day <= 9 or 'chapter 10' in text:
        chapter = '10'
        date = 'Apr 9'
      if day > 9 and day <= 23 or 'chapter 11' in text:
        chapter = '11'
        date = 'Apr 23'
      if day > 23:
        content = 'No more zyBooks!'
    if month > 4:
      content = 'No more zyBooks!'
    
    content = 'Chapter ' + chapter + ' is due ' + date + ' at midnight.'
    
  if 'prep course' in text or '191' in text:
    content = 'ENGR 191-001 is a one hour pass/fail course being offered for the first time this spring. It is taught by Dr Lusth himself to prepare everyone better for the infamous CS 201. There is an application for it (and a scholarship), so apply now at http://students.eng.ua.edu/programs/learning-advance/.'
  if 'how' in text and ('project' in text or 'project?' in text):
    content = 'I thought I explained this very well in class!'
  if 'office' in text and 'hours' in text:
    content = 'Office Hours: M 1:30-2:30, W 9:30-10:30, R 10:30-11:30'
  if 'vim' in text and ('exit' in text or 'quit' in text or 'get out of' in text):
    content = ':wq to save and exit'
  if 'cancel' in text and 'class' in text:
    content = 'I\'ll never cancel class.  Be there or be square.'
  
  return content

# DO NOT MODIFY ANYTHING BELOW THIS LINE
def send_message(msg):
  url  = 'https://api.groupme.com/v3/bots/post'
  data = {
          'bot_id' : os.getenv('GROUPME_BOT_ID'),
          'text'   : msg,
         }
  request = Request(url, urlencode(data).encode())
  json = urlopen(request).read().decode()

def log(msg):
  print(str(msg))
  sys.stdout.flush()
