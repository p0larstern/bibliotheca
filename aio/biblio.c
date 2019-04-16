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
    int deleted;
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
    char author_id[10];
    int quantity;
    int deleted;
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

// BOOK LEDGER (BOOK LOAN QUEUE)
typedef struct {
    int capacity;
    int size;
    int front;
    int rear;
    book_loaned *elements;
} book_ledgar;

// helper function's definitions
// add a single book data to book.bin
void add_book(book *book_to_add) {
    file_opened = fopen("book.bin", "ab");
    fseek(file_opened, 0, SEEK_END);
    fwrite(book_to_add, sizeof(book), 1, file_opened);
    fclose(file_opened);
}

// add a single author data to author.bin
void add_author(author *author_to_add) {
    file_opened = fopen("author.bin", "ab");
    fseek(file_opened, 0, SEEK_END);
    fwrite(author_to_add, sizeof(author), 1, file_opened);
    fclose(file_opened);
}

// add a single user data to user.bin
void add_author(user *user_to_add) {
    file_opened = fopen("user.bin", "ab");
    fseek(file_opened, 0, SEEK_END);
    fwrite(user_to_add, sizeof(user), 1, file_opened);
    fclose(file_opened);
}

// deletes a book from book.bin
void delete_book(char *req_isbn) {
    book temp;
    file_opened = fopen("book.bin", "a+b");
    fseek(file_opened, 0, SEEK_SET);
    while(fread(&temp, sizeof(book), 1, file_opened)) {
        if(req_isbn == temp.isbn) {
            temp.deleted = 1;
            fseek(file_opened, -1, SEEK_CUR);
            fwrite(&temp, sizeof(book), 1, file_opened);
            printf("\nBook deleted.");
        }
    }
}

// deletes a user from user.bin
void delete_user(char *req_user_id) {
    user temp;
    file_opened = fopen("user.bin", "a+b");
    fseek(file_opened, 0, SEEK_SET);
    while(fread(&temp, sizeof(user), 1, file_opened)) {
        if(req_user_id == temp.user_id) {
            temp.deleted = 1;
            fseek(file_opened, -1, SEEK_CUR);
            fwrite(&temp, sizeof(user), 1, file_opened);
            printf("\nUser deleted.");
        }
    }
}

/* return 1 if the requested attribute exists in book.bin else 0
    1 : isbn
    2 : title
    3 : author_id
*/
int search_book(char *req_attr, int type_attr) {
    book temp;
    char **temp_attr;
    file_opened = fopen("book.bin", "rb");

    switch(type_attr) {
        case 1 : temp_attr = &temp.isbn;
                break;
        case 2 : temp_attr = &temp.title;
                break;
        case 3 : temp_attr = &temp.author_id;
                break;
        default : return 0;
    }

    while(fread(&temp, sizeof(book), 1, file_opened)) {
        if(req_attr == *temp_attr && temp.deleted == 0) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}

/*
// return 1 if the requested isbn exists in book.bin else 0
int search_book_by_isbn(char *req_isbn) {
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

// return 1 if the requested title exists in book.bin else 0
int search_book_by_title(char *req_title) {
    book temp;
    file_opened = fopen("book.bin", "rb");
    
    while(fread(&temp, sizeof(book), 1, file_opened)) {
        if(req_title == temp.title) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}*/

/* return 1 if the requested attribute exists in author.bin else 0
    1 : author_id
    2 : first_name
    3 : last_name
*/
int search_author(char *req_attr, int type_attr) {
    author temp;
    char **temp_attr;
    file_opened = fopen("author.bin", "rb");

    switch(type_attr) {
        case 1 : temp_attr = &temp.author_id;
                break;
        case 2 : temp_attr = &temp.first_name;
                break;
        case 3 : temp_attr = &temp.last_name;
                break;
        default : return 0;
    }

    while(fread(&temp, sizeof(author), 1, file_opened)) {
        if(req_attr == *temp_attr) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}
/*
// return 1 if the requested author_id exists in author.bin else 0
int search_author_by_id(char *req_author_id) {
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

// return 1 if the requested first_name exists in author.bin else 0
int search_author_by_fname(char *req_first_name) {
    author temp;
    file_opened = fopen("author.bin", "rb");

    while(fread(&temp, sizeof(author), 1, file_opened)) {
        if(req_first_name == temp.first_name) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}

// return 1 if the requested first_name exists in author.bin else 0
int search_author_by_lname(char *req_last_name) {
    author temp;
    file_opened = fopen("author.bin", "rb");

    while(fread(&temp, sizeof(author), 1, file_opened)) {
        if(req_last_name == temp.last_name) {
            fclose(file_opened);
            return 1;
        }
    }

    fclose(file_opened);
    return 0;
}
*/

// return 1 if the requested user_id exists in user.bin else 0
int search_user(char *req_user_id) {
    user temp;
    file_opened = fopen("user.bin", "rb");
    
    while(fread(&temp, sizeof(user), 1, file_opened)) {
        if(req_user_id == temp.user_id && temp.deleted == 0) {
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

// functions to work book ledger (book loan queue)
// create book ledgar by book_loaned.bin
book_ledgar *create_ledger(int max_space) {
    file_opened = fopen("book_loaned.bin", "rb");
    book_loaned temp;
    book_ledgar *new_lg;

    new_lg = (book_ledgar *)malloc(sizeof(book_ledgar));
    new_lg->elements = (book_loaned *)malloc(sizeof(book_loaned)*max_space);
    new_lg->size = 0;
    new_lg->capacity = max_space;
    new_lg->front = -1;
    new_lg->rear = -1;
    
    while(fread(&temp, sizeof(book_loaned), 1, file_opened)) {
        enqueue(new_lg, &temp);
    }
    fclose(file_opened);
    return new_lg;
}

//----------------MAIN---------------
void main() {
    book_ledgar *ledger;
    int menu_option;
    ledger = create_ledger(50);

    do {
        printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 MAIN MENU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "1. Add Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "2. Delete books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "3. Search Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "4. Issue Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "5. Return Books");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "6. Add Users");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "7. Delete Users");
        printf("\n\xDB\xDB\xDB\xB2%-31s\xB2\xDB\xDB\xDB", "8. Close Application");
        printf("\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        do {
            printf("\nEnter option:");
            scanf("%d", &menu_option);
            switch(menu_option) {
                default : printf("\nInvalid option!\n");
                        break;
                case 1 : add_books();
                        break;
                case 2 : delete_books();
                        break;
                case 3 : search_books();
                        break;
                case 4 : issue_books();
                        break;
                case 5 : return_books();
                        break;
                case 6 : add_users();
                        break;
                case 7 : delete_users();
                        break;
                case 8 : return;   
            }
        } while(menu_option > 8 || menu_option < 1);    
    } while(menu_option != 8);

    //after menu exit stuff
}