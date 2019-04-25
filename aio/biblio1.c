#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

FILE *file_opened = NULL;

// All the structures to be used typedef(ed)
// DATE
typedef struct {
    int day;
    int month;
    int year;
} date;

// USER
typedef struct {
    char user_id[9];
    char name[15];
    char phone[11];
} user;

// AUTHOR
typedef struct {
    char author_id[9];
    char first_name[15];
    char last_name[15];
} author;

// BOOK
typedef struct {
    char isbn[14];
    char title[30];
    char author_id[9];
    int quantity;
} book;

// BOOK WHEN LOANED
typedef struct {
    char borrow_id[17];
    char isbn[14];
    char user_id[9];
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

struct bn {
    book *book_data;
    struct bn *next;
};
typedef struct bn book_node;

struct un {
    user *user_data;
    struct un *next;
};
typedef struct un user_node;

struct an {
    author *author_data;
    struct an *next;
};
typedef struct an author_node;

//-------------------------------------funtions of date start------------------------------
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

date *get_return_date(date *og) {
    int days_in_months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    og->day += 7;
    if(og->day > days_in_months[og->month - 1]) {
        og->day -= days_in_months[og->month - 1];
        og->month++;
    }
    if(og->month == 13) {
        og->month = 1;
        og->year++;
    }
    return og;
}
//---------------------------------funtions of date compete---------------------------

// ---------------------------------------------functions to work book ledger (book loan queue)---------
// create book ledgar by book_loaned.bin
book_ledgar *create_ledger(int max_space) {
    book_ledgar *new_lg;
    new_lg = (book_ledgar *)malloc(sizeof(book_ledgar));
    new_lg->elements = (book_loaned *)malloc(sizeof(book_loaned)*max_space);
    new_lg->size = 0;
    new_lg->capacity = max_space;
    new_lg->front = -1;
    new_lg->rear = -1;
    return new_lg;
}

// check whether the ledger is full or not
int is_full(book_ledgar *lg) {
    if((lg->front == lg->rear + 1) || (lg->front == 0 && lg->rear == lg->capacity-1)) {
        return 1;
    }
    return 0;
}

// check whether the ledger is empty or not
int is_empty(book_ledgar *lg) {
    if(lg->front == -1){
        return 1;
    }
    return 0;
}

void enqueue(book_loaned *element, book_ledgar *lg) {
    if(lg->front == -1) {
        lg->front = 0;
    }
    lg->rear = (lg->rear + 1) % lg->capacity;
    lg->elements[lg->rear] = *element;
}

book_loaned *dequeue(book_ledgar *lg) {
    book_loaned *element;
    element = (book_loaned *)malloc(sizeof(book_loaned));
    *element = lg->elements[lg->front];
    if (lg->front == lg->rear){
        lg->front = -1;
        lg->rear = -1;
    } /* lg has only one element, so we reset the queue after dequeing it. ? */
    else {
        lg->front = (lg->front + 1) % lg->capacity;
    }
    return element;
}

/* fill up the ledger from local file */
void fill_ledger(book_ledgar *lg) {
    int eof;
    book_loaned *temp;
    file_opened = fopen("book_loaned.bin", "rb");
    fseek(file_opened, 0, SEEK_END);
    eof = ftell(file_opened);
    fseek(file_opened, 0, SEEK_SET);
    while(ftell(file_opened) != eof) {
        printf("\nstuck here.");
        temp = (book_loaned *)malloc(sizeof(book_loaned));
        fread(temp, sizeof(book_loaned), 1, file_opened);
        enqueue(temp, lg);
    }
    fclose(file_opened);
}

// updates book_loaned.bin a/c to final ledger
void final_ledger(book_ledgar *lg) {
    file_opened = fopen("book_loaned.bin", "wb");
    book_loaned *temp;

    while(!is_empty(lg)) {
        temp = dequeue(lg);
        fwrite(temp, sizeof(book_loaned), 1, file_opened);
    }
    fclose(file_opened);
}
//------------------------------functions complete--------------------------

//----------------------------funtions for initializing and filling of linked list-------------------------------
// initialize head for books linked list
book_node *init_book_list() {
    book *data;
    book_node *temp;
    file_opened = fopen("book.bin", "rb");
    data = (book *)malloc(sizeof(book));
    temp = (book_node *)malloc(sizeof(book_node));
    fread(data, sizeof(book), 1, file_opened);
    temp->book_data = data;
    temp->next = NULL;
    fclose(file_opened);
    return temp;
}

// fill the remaining objects from books linked list
void fill_books(book_node *start) {
    int eof;
    book *data;
    book_node *temp, *cur;
    file_opened = fopen("book.bin", "rb");
    fseek(file_opened, 0, SEEK_END);
    eof = ftell(file_opened);
    fseek(file_opened, sizeof(book), SEEK_SET);
    cur = start;
    while(ftell(file_opened) != eof) {
        data = (book *)malloc(sizeof(book));
        temp = (book_node *)malloc(sizeof(book_node));
        fread(data, sizeof(book), 1, file_opened);
        temp->book_data = data;
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }
    fclose(file_opened);
}

// initializes head for users linked list
user_node *init_user_list() {
    user *data;
    user_node *temp;
    file_opened = fopen("user.bin", "rb");
    data = (user *)malloc(sizeof(user));
    temp = (user_node *)malloc(sizeof(user_node));
    fread(data, sizeof(user), 1, file_opened);
    temp->user_data = data;
    temp->next = NULL;
    fclose(file_opened);
    return temp;
}

// fill the remaining objects from users linked list
void fill_users(user_node *start) {
    int eof;
    user *data;
    user_node *temp, *cur;
    
    file_opened = fopen("user.bin", "rb");
    fseek(file_opened, 0, SEEK_END);
    eof = ftell(file_opened);
    
    fseek(file_opened, sizeof(user), SEEK_SET);
    cur = start;
    while(ftell(file_opened) != eof) {
        data = (user *)malloc(sizeof(user));
        temp = (user_node *)malloc(sizeof(user_node));
        fread(data, sizeof(user), 1, file_opened);
        temp->user_data = data;
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }
    fclose(file_opened);
}

// initializes head for authors linked list
author_node *init_author_list() {
    author *data;
    author_node *temp;
    file_opened = fopen("author.bin", "rb");
    data = (author *)malloc(sizeof(author));
    temp = (author_node *)malloc(sizeof(author_node));
    fread(data, sizeof(author), 1, file_opened);
    temp->author_data = data;
    temp->next = NULL;
    fclose(file_opened);
    return temp;
}

// fill the remaining objects from author linked list
void fill_authors(author_node *start) {
    int eof;
    author *data;
    author_node *temp, *cur;
    
    file_opened = fopen("author.bin", "rb");
    fseek(file_opened, 0, SEEK_END);
    eof = ftell(file_opened);
    
    fseek(file_opened, sizeof(author), SEEK_SET);
    cur = start;
    while(ftell(file_opened) != eof) {
        data = (author *)malloc(sizeof(author));
        temp = (author_node *)malloc(sizeof(author_node));
        fread(data, sizeof(author), 1, file_opened);
        temp->author_data = data;
        temp->next = NULL;
        cur->next = temp;
        cur = cur->next;
    }
    fclose(file_opened);
}
//-------------------------------------funtions complete----------------------------------

// ---------------------------------------BOOKS LINKED LIST EFFECTING FUNCTIONS-------------------------------
// This function counts number of leap years before the 
// given date 
int count_leap_years(date d) { 
    int years = d.year; 
  
    // Check if the current year needs to be considered 
    // for the count of leap years or not 
    if(d.month <= 2) 
        years--; 
  
    // An year is a leap year if it is a multiple of 4, 
    // multiple of 400 and not a multiple of 100. 
    return years / 4 - years / 100 + years / 400; 
} 
  
// This function returns number of days between two given 
// dates 
int get_difference(date dt1, date dt2) { 
    int i;
    int days_in_months[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    // COUNT TOTAL NUMBER OF DAYS BEFORE FIRST DATE 'dt1' 
  
    // initialize count using years and day 
    long int n1 = dt1.year*365 + dt1.day; 
  
    // Add days for months in given date 
    for (i=0; i<dt1.month - 1; i++) 
        n1 += days_in_months[i]; 
  
    // Since every leap year is of 366 days, 
    // Add a day for every leap year 
    n1 += count_leap_years(dt1); 
  
    // SIMILARLY, COUNT TOTAL NUMBER OF DAYS BEFORE 'dt2' 
  
    long int n2 = dt2.year*365 + dt2.day; 
    for (int i=0; i<dt2.month - 1; i++) 
        n2 += days_in_months[i]; 
    n2 += count_leap_years(dt2); 
  
    // return difference between two counts 
    return (n2 - n1); 
} 

int get_fine(date *final, date *initial) {
    int fine = 0;
    fine = get_difference(*final, *initial)*10;
    return fine;
}

void increase_count(char *temp_isbn, int qnty, book_node *books_head) {
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        if(cur->book_data->isbn == temp_isbn) {
            cur->book_data->quantity += qnty;
            return;
        }
        cur = cur->next;
    }
}

void decrease_count(char *temp_isbn, int qnty, book_node *books_head) {
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        if(cur->book_data->isbn == temp_isbn) {
            cur->book_data->quantity -= qnty;
            return;
        }
        cur = cur->next;
    }
}

/* return 1 if the requested attribute exists in book linked list else 0
    1 : isbn
    2 : title
    3 : author_id
*/
int search_book(char *req_attr, int type_attr, book_node *books_head) {
    if(books_head == NULL)
        return 0;
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        switch(type_attr) {
            case 1: if(strcmp(cur->book_data->isbn, req_attr) == 0)
                        return 1;
            case 2: if(strcmp(cur->book_data->title, req_attr) == 0)
                        return 1;
            case 3: if(strcmp(cur->book_data->author_id, req_attr) == 0)
                        return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* follow up function for search in book menu */
void fsearch_book(book_node *books_head) {
    system("cls");
    int search_type;
    char temp[20];
    printf("\nSearch by:-\n1.ISBN\n2.Title\nOption?");
    scanf("%d", &search_type);
    printf("\nValue of chosen option?");
    scanf("%s", temp);
    if(search_book(temp, search_type, books_head) == 1)
        printf("Book found.");
    else
        printf("Book not found.");
    getch();
}

// add book data to last node of books linked list.
void add_book(book_node *book_to_add, book_node *books_head) {
    book_node *cur;
    cur = books_head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = book_to_add;
    printf("\nBook added!");
}

// returns quantity of the books
int get_quantity(char *temp_isbn, book_node *books_head) {
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        if(strcmp(cur->book_data->isbn, temp_isbn) == 0) {
            return cur->book_data->quantity;
        }
        cur = cur->next;
    }
}

// deletes a book from book.bin
void delete_book(char *req_isbn, book_node **books_head) {
    book_node *cur, *prev;
    cur = *books_head;
    if(cur != NULL && cur->book_data->isbn == req_isbn) {
        *books_head = cur->next;
        free(cur);
        return;
    } 
    while(cur != NULL && cur->book_data->isbn != req_isbn) {
        prev = cur;
        cur = cur->next;
    }
    if(cur == NULL)
        return;
    prev->next = cur->next;
    free(cur);
}

void fdelete_book(book_node *books_head) {
    char temp_isbn[14];
    int qnty, act_qnty;
    system("cls");
    if(books_head == NULL) {
        printf("\nNo books found");
        getch();
        return;
    }
    printf("\nISBN?");
    scanf("%s", temp_isbn);
    if(search_book(temp_isbn, 1, books_head) == 0) {
        printf("\nNo such book found!");
        getch();
        return;
    }
    act_qnty = get_quantity(temp_isbn, books_head);
    printf("\nHow many?(%d present)");
    scanf("%d", &qnty);
    if(qnty > act_qnty) {
        printf("\nDeleting maximum possible books.");
        decrease_count(temp_isbn, act_qnty, books_head);
        getch();
        return;
    }
    printf("\nDeleting %d books.", qnty);
    decrease_count(temp_isbn, qnty, books_head);
    getch();
}

/* generates borrowing id
example :-
indices :       0 1 2 3 4 5 6 7 8 9 1 1 2 3 4 5 6 
isbn :          9 9 9 9 9 9 5 5 5 3 3 2 2 \0
user_id :       d h r u x x x x \0
issuing :       23 2 2019
bor_id :        2 0 1 9 0 2 2 3 x x x x 3 3 2 2 \0
*/
char *gen_bor_id(book_loaned *temp) {
    char *id, a[10], b[5], c[5];
    id = (char *)malloc(sizeof(char)*17);
    int i;
    sprintf(a, "%d", temp->issuing->year);
    if(temp->issuing->month < 10)
        sprintf(b, "0%d", temp->issuing->month);
    else
        sprintf(b, "%d", temp->issuing->month);
    if(temp->issuing->day < 10)
        sprintf(c, "0%d", temp->issuing->day);
    else
        sprintf(c, "%d", temp->issuing->day);
    for(i = 0; i < 4; i++)
        id[i] = a[i];
    for(i = 4; i < 6; i++)
        id[i] = b[i-4];
    for(i = 6; i < 8; i++)
        id[i] = c[i-6];
    for(i = 8; i < 12; i++)
        id[i] = temp->user_id[i - 8 + 4];
    for(i = 12; i < 16; i++)
        id[i] = temp->isbn[i - 12 + 9];
    id[16] = '\0';
    return id;
}

/* search for record in book ledger */
book_loaned *get_record(char *record_bid, book_ledgar *lg) {
    if(lg->size == 0)
        return NULL;
    book_loaned *temp;
    temp = (book_loaned *)malloc(sizeof(book_loaned));
    int count = 0;
    do {
        temp = dequeue(lg);
        if(temp->borrow_id == record_bid)
            return temp;
        enqueue(temp, lg);
        count++;
    } while(count < lg->size);
    return NULL;
}

// AUTHORS LINKED LIST EFFECTING FUNCTIONS
/* generates author_id 
example :-                0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
    author->first_name -  r o b e r t \0
    author->last_name -   f r o s t \0
then author->id -         r o b e f r o s \0
*/
char *gen_aid(author *data) {
    char *id;
    id = (char *)malloc(sizeof(char)*9);
    int i;
    for(i=0; i<4; i++)
        id[i] = data->first_name[i];
    for(i=4; i<8; i++)
        id[i] = data->last_name[i-4];
    id[8] = '\0';
    printf("\nAuthor ID generated.");
    getch();
    return id;
}

// add a single author data to author linked list
void add_author(author_node *author_to_add, author_node *authors_head) {
    author_node *cur;
    cur = authors_head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = author_to_add;
    printf("\nAuthor added!");
    getch();
}

/* return 1 if the requested attribute exists in author.bin else 0
    1 : author_id
    2 : first_name
    3 : last_name
*/
int search_author(char *req_attr, int type_attr, author_node *authors_head) {
    author_node *cur;
    if(authors_head == NULL)
        return 0;
    cur = authors_head;
    while(cur != NULL) {
        switch(type_attr) {
            case 1: if(strcmp(cur->author_data->author_id, req_attr) == 0)
                        return 1;
            case 2: if(strcmp(cur->author_data->first_name, req_attr) == 0)
                        return 1;
            case 3: if(strcmp(cur->author_data->last_name, req_attr) == 0)
                        return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* follow up function for menu option of author search */
void fsearch_author(author_node *authors_head) {
    system("cls");
    char first_name[15], last_name[15];
    printf("\nFirst name?");
    scanf("%s", first_name);
    printf("\nLast Name?");
    scanf("%s", last_name);
    if((search_author(first_name, 2, authors_head) == 1) && (search_author(last_name, 3, authors_head) == 1))
        printf("\nAuthor found.");
    else
        printf("\nAuthor not found.");
    getch();
}

/* printing all the book titles related to requested author */
void author_books(author_node *authors_head, book_node *books_head) {
    system("cls");
    book_node *cur;
    char temp_aid[9];
    printf("\nAuthor ID?");
    scanf("%s", temp_aid);
    if(search_author(temp_aid, 1, authors_head) == 1) {
        printf("\nAuthor found.");
        printf("\nFollowing Book titles available:- ");
        cur = books_head;
        while(cur != NULL) {
            if(cur->book_data->author_id == temp_aid)
                printf("\n%s", cur->book_data->title);
            cur = cur->next;
        }
    }
    else
        printf("\nAuthor not found.");
    getch();
}

/* follow up funtion of add book in book menu */
void fadd_book(book_node *books_head, author_node *authors_head) {
    system("cls");
    author *temp_author;
    author_node *new_author;
    book *data;
    book_node *new_book;
    data = (book *)malloc(sizeof(book));
    new_book = (book_node *)malloc(sizeof(book_node));
    printf("\nISBN?");
    scanf("%s", data->isbn);
    printf("\nQuantity?");
    scanf("%d", &data->quantity);
    if(search_book(data->isbn, 1, books_head) == 1) {
        increase_count(data->isbn, data->quantity, books_head);
        printf("\nBook added.");
        getch();
        return;
    }
    printf("\nBook Title?");
    scanf("%s", data->title);
    temp_author = (author *)malloc(sizeof(author));
    new_author = (author_node *)malloc(sizeof(author_node));
    printf("\nAuthor first name?");
    scanf("%s", temp_author->first_name);
    printf("\nAuthor Last name?");
    scanf("%s", temp_author->last_name);
    strcpy(temp_author->author_id, gen_aid(temp_author));
    if(search_author(temp_author->author_id, 1, authors_head) == 0) {
        new_author->author_data = temp_author;
        new_author->next = NULL;
        add_author(new_author, authors_head);
    }
    else
        printf("\nAuthor exists");
    strcpy(data->author_id, temp_author->author_id);
    new_book->book_data = data;
    new_book->next = NULL;
    add_book(new_book, books_head);
    getch();
}
/*---------------------------------------author funtions complete----------------------------------*/

// USER LINKED LIST EFFECTING FUNCTIONS
/* generates user id 
example :- 
    indices -       0 1 2 3 4 5 9 7 8 9 10
    user->name -    d h r u v \0
    user->phone -   x x x x x x y y y y \0
then user->id -     d h r u y y y y \0 
*/
char *gen_uid(user *stuff) {
    char *id;
    id = (char *)malloc(sizeof(char)*9);
    int i;
    for(i=0; i<4; i++)
        id[i] = stuff->name[i];
    for(i=4; i<8; i++)
        id[i] = stuff->phone[i+2];
    id[8] = '\0';
    return id;
}

/* search for user in users only by user_id, return 1 if found else 0 */
int search_user(char *req_user_id, user_node *users_head) {
    user_node *cur;
    if(users_head == NULL)
        return 0;
    cur = users_head;
    while(cur != NULL) {
        if(strcmp(cur->user_data->user_id, req_user_id) == 0) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* follow up function of option search user in menu user */ 
void fsearch_user(user_node *users_head) {
    system("cls");
    char temp_uid[9];
    printf("\nUser Id?");
    scanf("%s", temp_uid);
    if(search_user(temp_uid, users_head) == 1) {
        printf("\nUser found.");
    } else {
        printf("\nUser not found.");
    }
    getch();
}

/* add a user to the users linked list */
void add_user(user_node *user_to_add, user_node *users_head) {
    user_node *cur;
    cur = users_head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = user_to_add;
}

/* follow up function of option add user in menu user */
void fadd_user(user_node *users_head) {
    system("cls");
    user *data;
    user_node *new_user;
    data = (user *)malloc(sizeof(user));
    new_user = (user_node *)malloc(sizeof(user_node));
    printf("\nUser Name?");
    scanf("%s", data->name);
    printf("\nUser Phone Number?");
    scanf("%s", data->phone);
    strcpy(data->user_id, gen_uid(data));
    new_user->user_data = data;
    new_user->next = NULL;
    add_user(new_user, users_head);
    getch();
}

/* delete requested user node from users linked list */
void delete_user(char *req_uid, user_node **users_head) {
    user_node *cur, *prev;
    cur = *users_head;
    if(cur != NULL && cur->user_data->user_id == req_uid) {
        *users_head = cur->next;
        free(cur);
        return;
    } 
    while(cur != NULL && cur->user_data->user_id != req_uid) {
        prev = cur;
        cur = cur->next;
    }
    if(cur == NULL)
        return;
    prev->next = cur->next;
    free(cur);
}

/* follow up function of option delete user in menu user */
void fdelete_user(user_node *users_head) {
    system("cls");
    char temp_uid[9];
    printf("\nUser ID?");
    scanf("%s", temp_uid);
    if(search_user(temp_uid, users_head) == 1) {
        delete_user(temp_uid, &users_head);
        printf("\nUser deleted.");
    }
    else
        printf("\nUser Id not available.");
    getch();
}

/* writes the updated user linked list on user.bin */
void final_users(user_node *users_head) {
    file_opened = fopen("user.bin", "wb");
    user_node *cur;
    cur = users_head;
    while(cur != NULL) {
        fwrite(cur, sizeof(user_node), 1, file_opened);
        cur = cur->next;
    }
    fclose(file_opened);
}
/*--------------------------------------------------user functions complete--------------------------------------------*/

/* writes the updated book linked list on book.bin */
void final_books(book_node *books_head) {
    file_opened = fopen("book.bin", "wb");
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        fwrite(cur->book_data, sizeof(book), 1, file_opened);
        cur = cur->next;
    }
    fclose(file_opened);
}

/* writes the updated author linked list on author.bin */
void final_authors(author_node *authors_head) {
    file_opened = fopen("author.bin", "wb");
    author_node *cur;
    cur = authors_head;
    while(cur != NULL) {
        fwrite(cur, sizeof(author_node), 1, file_opened);
        cur = cur->next;
    }
    fclose(file_opened);
}


/* issue single book */
void issue_book(book_node *books_head, user_node *users_head, book_ledgar *lg) {
    system("cls");
    book_loaned *new_issue;
    new_issue = (book_loaned *)malloc(sizeof(book_loaned));
    printf("\nUser ID?");
    scanf("%s", new_issue->user_id);
    if(search_user(new_issue->user_id, users_head) == 0) {
        printf("\nInvalid User ID.");
        getch();
        return;
    }
    printf("\nISBN?");
    scanf("%s", new_issue->isbn);
    if(search_book(new_issue->isbn, 1, books_head) == 0) {
        printf("\nInvalid ISBN.");
        getch();
        return;
    }
    if(get_quantity(new_issue->isbn, books_head) == 0) {
        printf("\nBook not available");
        getch();
        return;
    }
    new_issue->issuing = get_date();
    new_issue->returning = get_date();
    new_issue->returning = get_return_date(new_issue->returning);
    strcpy(new_issue->borrow_id, gen_bor_id(new_issue));
    decrease_count(new_issue->isbn, 1, books_head);
    enqueue(new_issue, lg);
}

/* return book */
void return_book(book_node *books_head, book_ledgar *lg) {
    system("cls");
    book_loaned *record;
    char temp_id[17];
    int fine;
    printf("\nBorrow ID?");
    scanf("%s", temp_id);
    record = get_record(temp_id, lg);
    if(record == NULL) {
        printf("\nNo such record found.");
        getch();
        return;
    }
    record->returned = get_date();
    fine = get_fine(record->returned, record->returning);
    if(fine > 0) {
        printf("\nPlease pay %d ruppees fine.", fine);
    }
    printf("\nThank You.");
    getch();
}


// SUB MENU PRINTING FUNCTIONS
/* Sub Menu for Books */
void menu_books(book_node *books, author_node *authors, user_node *users, book_ledgar *ldg) {
    system("cls");
    int opt;
    printf("\n==================== BOOKS ====================");
    printf("\n1. Add Book");
    printf("\n2. Delete Book");
    printf("\n3. Search Book");
    printf("\n4. Issue Book");
    printf("\n5. Return Book");
    printf("\n6. Go Back");
    printf("\n===============================================");
    printf("\nEnter option:");
    scanf("%d", &opt);
    do {
        switch(opt) {
            default: printf("\nInvalid option!");break;
            case 1: fadd_book(books, authors);break;
            case 2: fdelete_book(books);break;
            case 3: fsearch_book(books);break;
            case 4: issue_book(books, users, ldg);break;
            case 5: return_book(books, ldg);break;
            case 6: return;
        }
    } while(opt > 6 || opt < 1);
}

/* Sub Menu For Authors */
void menu_authors(author_node *authors, book_node *books) {
    system("cls");
    int opt;
    printf("\n=================== AUTHORS ===================");
    printf("\n1. Search Author");
    printf("\n2. Author's Books");
    printf("\n3. Go Back");
    printf("\n===============================================");
    printf("\nEnter option:");
    scanf("%d", &opt);
    do {
        switch(opt) {
            case 1: fsearch_author(authors);break;
            case 2: author_books(authors, books);break;
            case 3: return;
        } 
    } while(opt > 3 || opt < 1);
}

/* Sub Menu for Users */
void menu_users(user_node *users) {
    system("cls");
    int opt;
    printf("\n==================== USERS ====================");
    printf("\n1. Search User");
    printf("\n2. Add User");
    printf("\n3. Delete User");
    //user history
    printf("\n4. Go Back");
    printf("\n===============================================");
    printf("\nEnter option:");
    scanf("%d", &opt);
    do {
        switch(opt) {
            case 1: fsearch_user(users);break;
            case 2: fadd_user(users);break;
            case 3: fdelete_user(users);break;
            case 4: return;
        }
    } while(opt > 4 || opt < 1);
}

void main() {
    int menu_opt;
    book_node *b_head;
    author_node *a_head;
    user_node *u_head;
    book_ledgar *ledger;
    
    // Initialinzing pointers for different linked lists
    b_head = init_book_list();
    printf("\nBooks initialized");
    fill_books(b_head);
    printf("\nBooks loaded");
    a_head = init_author_list();
    printf("\nAuthors initialized");
    fill_authors(a_head);
    printf("\nAuthors loaded");
    u_head = init_user_list();
    printf("\nUsers initialized");
    fill_users(u_head);
    printf("\nUsers loaded");
    ledger = create_ledger(50);
    printf("\nLedger created");
    fill_ledger(ledger);
    printf("\nLedger filled.");
    
    do {
        system("cls");
        printf("\n====================MAIN MENU===================");
        printf("\n1. Books");
        printf("\n2. Authors");
        printf("\n3. Users");
        printf("\n4. Close Application");
        printf("\n================================================");
        do {
            printf("\nEnter option:");
            scanf("%d", &menu_opt);
            switch(menu_opt) {
                default: printf("\nInvalid option!\n");break;
                case 1: menu_books(b_head, a_head, u_head, ledger);break;
                case 2: menu_authors(a_head, b_head);break;
                case 3: menu_users(u_head);break;
                case 4: break;
            }
        } while(menu_opt > 4 || menu_opt < 1);    
    } while(menu_opt != 4);

    final_ledger(ledger);
    final_books(b_head);
    final_users(u_head);
    final_authors(a_head);
    exit(0);
}