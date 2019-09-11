#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void displayGame(char*, int, int, int*);
int makeGuess(char*, char*, char, int, int*);
char singleChar();
int main(int argc, char** argv) {
    char word[100];
    char current[100];
    int guesses[26];
    char guess;
    char answer[2] = "y";
    int deathCount = 0;
    int flag;
    while (answer[0] == 'y') {
        deathCount = 0;
        FILE* wordList = fopen("hangwords", "r");
        if (wordList == NULL) {
            printf("No file named \"%s\" found\n", argv[1]);
            return -1;
        }
        srand(time(NULL));
        for (int i = 0; i < 26; ++i) {
            guesses[i] = 0;
        }
        int wordNumber = rand() % 855;
        for (int i = 0; i < wordNumber; ++i) {
            fscanf(wordList, "%s", word);
        }
        fclose(wordList);
        if (argc == 3) strcpy(word, argv[2]);
        int length = strlen(word);
        current[0] = '\0';
        for (int i = 0; i < length; ++i) {
            strcat(current, "_");
        }
        displayGame(current, length, deathCount, guesses);
        while (deathCount < 7) {
            if (strcmp(current, word) == 0) {
                printf("You won!\n");
                break;
            }
            if (flag == 2) {
                printf("You've already guessed that character\n");
            }
            printf("Enter a character: ");
            guess = singleChar();
            flag = makeGuess(word, current, guess, length, guesses);
            if (isalpha(guess)) guesses[guess-97]++;
            if (flag == 0) deathCount++;
            if (flag == 42) deathCount = -26;
            displayGame(current, length, deathCount, guesses);
        }
        if (deathCount == 7) printf("You've been hanged.\nThe word was: %s\n", word);
        printf("Play again? (y / n): ");
        scanf("%s", answer);
    }
    system("clear");
    return 0;
}

void displayGame(char* current, int length, int deathCount, int* guesses) {
    char command[20] = "clear";
    system(command);
    system("clear");
    printf("Guessed letters: ");
    for (int j = 0; j < 26; ++j) {
        if (guesses[j] > 0) printf("%c ", j + 97);
        else printf("  ");
    }
    printf("\n");
    printf("Limb count: %d/7\n", deathCount);
    for (int j = 0; j < 7; ++j) {
        if (j == 0 || j == 6) {
            for (int i = 0; i < (2 * length) + 7; ++i) {
                printf("*");
            }
            if (j == 0) printf("      ___");
            if (j == 6) printf("     ____|__");
            printf("\n");
        }
        if (j == 1 || j == 2 || j == 4 || j == 5) {
            for (int i = 0; i < (2* length) + 7; ++i) {
                if (i == 0 || i == (2*length) + 6) printf("*");
                else printf(" ");
            }
            if (j == 1) printf("     |   |");
            if (j == 2) {
                if (deathCount > 0) printf("     o   |");
                else printf("         |");
            }
            if (j == 4) {
                if (deathCount > 4) printf("     |   |");
                else printf("         |");
            }
            if (j == 5) {
                if (deathCount > 6) printf("    / \\  |");
                else if (deathCount > 5) printf("    /    |");
                else printf("         |");
            }
            printf("\n");
        }
        if (j == 3) {
            printf("*   ");
            for (int i = 0; i < length; ++i) {
                printf("%c ", current[i]);
            }
            printf("  *    ");
            if (deathCount <= 1)     printf( "     |");
            else if (deathCount > 3) printf("\\|/  |");
            else if (deathCount > 2) printf("\\|   |");
            else if (deathCount > 1) printf( " |   |");
            printf("\n");
        }
    }
    return;
}

int makeGuess(char* word, char* current, char guess, int length, int* guesses) {
    int flag = 0;
    if (guess == '~') return 42;
    if (guess == '\n') return 3;
    if (guesses[guess - 97] > 0) return 2;
    for (int i = 0; i < length; ++i) {
        if (word[i] == guess) {
            current[i] = guess;
            flag = 1;
        }
    }
    return flag;
}

char singleChar(void) {
    char temp[100];
    fgets(temp, 99, stdin);
    return temp[0];
}
