#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUESTION_COUNT 10
#define NAME_LENGTH 80
#define SCORE_FILE "quiz_scores.txt"

typedef struct {
    const char *question;
    const char *options[4];
    char answer;
} Question;

static const Question questions[QUESTION_COUNT] = {
    {"Which function displays formatted output in C?", {"printf", "scanf", "strlen", "main"}, 'A'},
    {"Which symbol ends most C statements?", {".", ";", ":", ","}, 'B'},
    {"Which header provides printf and scanf?", {"stdlib.h", "string.h", "stdio.h", "math.h"}, 'C'},
    {"What is the value of 7 % 3?", {"1", "2", "3", "0"}, 'A'},
    {"Which type stores a single character?", {"int", "float", "char", "double"}, 'C'},
    {"Which loop checks its condition before each iteration?", {"do-while", "while", "switch", "case"}, 'B'},
    {"Array indexing in C starts at which number?", {"0", "1", "-1", "It varies"}, 'A'},
    {"Which operator means logical AND?", {"&", "||", "&&", "!"}, 'C'},
    {"Which keyword returns a value from a function?", {"break", "return", "continue", "exit"}, 'B'},
    {"What does sizeof return?", {"Number of elements", "A memory address", "Size in bytes", "A character"}, 'C'}
};

static void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

static int read_choice(void) {
    char line[32];
    if (!fgets(line, sizeof line, stdin)) return -1;
    return atoi(line);
}

static void play_quiz(void) {
    char name[NAME_LENGTH];
    int score = 0;

    printf("\nEnter your name: ");
    if (!fgets(name, sizeof name, stdin)) return;
    name[strcspn(name, "\r\n")] = '\0';
    if (name[0] == '\0') strcpy(name, "Player");

    printf("\n--- Quiz: %d questions ---\n", QUESTION_COUNT);
    for (int i = 0; i < QUESTION_COUNT; ++i) {
        char answer;
        printf("\n%d. %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; ++j)
            printf("   %c) %s\n", 'A' + j, questions[i].options[j]);
        printf("Your answer (A-D): ");
        if (scanf(" %c", &answer) != 1) {
            clear_input();
            answer = '\0';
        }
        clear_input();
        if (answer >= 'a' && answer <= 'z') answer -= 'a' - 'A';

        if (answer == questions[i].answer) {
            puts("Correct!");
            ++score;
        } else {
            printf("Incorrect. The correct answer is %c.\n", questions[i].answer);
        }
    }

    printf("\n%s, your score is %d/%d (%.0f%%).\n", name, score,
           QUESTION_COUNT, 100.0 * score / QUESTION_COUNT);
    FILE *file = fopen(SCORE_FILE, "a");
    if (!file) {
        perror("Could not save the score");
        return;
    }
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char date[32] = "unknown date";
    if (local) strftime(date, sizeof date, "%Y-%m-%d %H:%M", local);
    /* Keep each record on one line, even if a name contains punctuation. */
    for (char *p = name; *p; ++p) if (*p == '\t' || *p == '\n' || *p == '\r') *p = ' ';
    fprintf(file, "%s\t%d/%d\t%s\n", name, score, QUESTION_COUNT, date);
    fclose(file);
    printf("Score saved in %s.\n", SCORE_FILE);
}

static void view_scores(void) {
    char line[256];
    FILE *file = fopen(SCORE_FILE, "r");
    if (!file) {
        printf("\nNo saved scores yet. Play a quiz first.\n");
        return;
    }
    printf("\n--- Saved Scores ---\n");
    printf("%-24s %-10s %s\n", "Player", "Score", "Date");
    printf("--------------------------------------------------\n");
    while (fgets(line, sizeof line, file)) {
        char *name = strtok(line, "\t");
        char *score = strtok(NULL, "\t");
        char *date = strtok(NULL, "\r\n");
        if (name && score && date) printf("%-24.24s %-10s %s\n", name, score, date);
    }
    fclose(file);
}

int main(void) {
    int choice;
    puts("====================================");
    puts("      QUIZ AND SCORE MANAGER");
    puts("====================================");
    do {
        puts("\n1. Take the quiz\n2. View saved scores\n3. Exit");
        printf("Choose an option: ");
        choice = read_choice();
        switch (choice) {
            case 1: play_quiz(); break;
            case 2: view_scores(); break;
            case 3: puts("Goodbye!"); break;
            default: puts("Please enter 1, 2, or 3."); break;
        }
    } while (choice != 3);
    return 0;
}
