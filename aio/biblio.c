#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

FILE *file_opened = NULL, *temp_file = NULL;

// All the structures to be used typedef(ed)
// DATE
typedef struct {
    int day;
    int month;
    int year;
} date;

// USER
typedef struct {
    char user_id[10];
    char name[15];
    char phone[10];
} user;

// AUTHOR
typedef struct {
    char author_id[10];
    char first_name[10];
    char last_name[10];
} author;

// BOOK
typedef struct {
    char isbn[14];
    char title[20];
    int quantity;
    char author_id[10];
} book;

// BOOK WHEN LOANED
typedef struct {
    char borrow_id[10];
    char isbn[14];
    char user_id[10];
    date *issuing;
    date *returning;
    date *returned;
} book_loaned;

// helper function's definitions
// add a single book data to book.bin
void add_book(book *book_to_add) {
    file_opened = fopen("book.bin", "ab");
    fseek(file_opened, 0, SEEK_END);
    fwrite(book_to_add, sizeof(book), 1, file_opened);
    fclose(file_opened);
}

// deletes a book from book.bin
void delete_book() {

}

// return 1 if the requested author_id exists in author.bin else 0
void search_book(char *req_isbn) {
    book temp;
    file_opened = fopen("book.bin", "rb");
    
    while(fread(&temp, sizeof(book), 1, file_opened)) {
        if(req_isbn == temp.isbn) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}

// return 1 if the requested author_id exists in author.bin else 0
int search_author(char *req_author_id) {
    author temp;
    file_opened = fopen("author.bin", "rb");

    while(fread(&temp, sizeof(author), 1, file_opened)) {
        if(req_author_id == temp.author_id) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}

// issues 'n' books, depending on existence in book.bin
void issue_books() {

}

// something related to returning
void return_books() {

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

//----------------MAIN---------------
void main() {
    int menu_option;
    //before menu start stuff

    do {
        printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 MAIN MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "1. Add Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "2. Delete books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "3. Search Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "4. Issue Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "5. Return Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "6. Close Application");
        printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        do {
            printf("\nEnter option:");
            scanf("%d", &menu_option);
            switch(menu_option) {
                default: printf("\nInvalid option!\n");
                        break;
                case 1: add_books();
                        break;
                case 2: delete_books();
                        break;
                case 3: search_books();
                        break;
                case 4: issue_books();
                        break;
                case 5: return_books();
                        break;
                case 6: return;   
            }
        } while(menu_option > 6 || menu_option < 1);    
    } while(menu_option != 6);

    //after menu exit stuff
}