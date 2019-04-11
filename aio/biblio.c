#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>
#include<time.h>

// All the structures to be used typedef(ed)
// DATE
typedef struct {
    int day;
    int month;
    int year;
} date;

// USER
typedef struct {
    char user_id[15];
    char name[15];
    char phone[10];
} user;

// AUTHOR
typedef struct {
    char author_id[15];
    char first_name[15];
    char last_name[15];
} author;

// BOOK
typedef struct {
    char isbn[13];
    char title[20];
    char author_id[15];
} book;

// BOOK WHEN LOANED
typedef struct {
    char borrow_id[15];
    char isbn[13];
    char user_id[15];
    date *issuing;
    date *returning;
    date *returned;
} book_loaned;

// function's definitions
// print the main menu on screen
void show_main_menu() {
    printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 MAIN MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "1. Add Books   ");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "2. Delete books");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "3. Search Books");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "4. Issue Books");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "5. View Book list");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "6. Edit Book's Record");
    printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "7. Close Application");
    printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
}

// return's system date
date *get_date() {
    date *new_date;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    new_date = (date *)malloc(sizeof(date));
    new_date->day = tm.tm_mday;
    new_date->month = tm.tm_mon + 1;
    new_date->year = tm.tm_year + 1900;
    return new_date;
}

void main() {
    unsigned short menu_option;
    //before menu start stuff

    do {
        show_main_menu();
        printf("\nEnter option:");
        scanf("%d", menu_option);
    } while(menu_option != 7);

    //after menu exit stuff
}