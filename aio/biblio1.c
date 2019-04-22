#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

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
    char title[20];
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

// Making the pointers for various linked lists global for easy usage
book_node *books_head;
author_node *authors_head;
user_node *users_head;
book_ledgar *lg;

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

// functions to work book ledger (book loan queue)
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
int is_full() {
    if((lg->front == lg->rear + 1) || (lg->front == 0 && lg->rear == lg->capacity-1)) {
        return 1;
    }
    return 0;
}

// check whether the ledger is empty or not
int is_empty() {
    if(lg->front == -1){
        return 1;
    }
    return 0;
}

void enqueue(book_loaned *element) {
    if(lg->front == -1) {
        lg->front = 0;
    }
    lg->rear = (lg->rear + 1) % lg->capacity;
    lg->elements[lg->rear] = *element;
}

book_loaned *dequeue() {
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
void fill_ledger() {
    book_loaned temp;
    file_opened = fopen("book_loaned.bin", "rb");
    
    while(fread(&temp, sizeof(book_loaned), 1, file_opened)) {
        enqueue(&temp);
    }
    fclose(file_opened);
}

// updates book_loaned.bin a/c to final ledger
void finalize_ledger() {
    file_opened = fopen("book_loaned.bin", "wb");
    book_loaned *temp;

    while(!is_empty(lg)) {
        temp = dequeue(lg);
        fwrite(&temp, sizeof(book_loaned), 1, file_opened);
    }
}

book_node *init_book_list() {
    book *temp;
    book_node *head, *cur, *temp2;
    file_opened = fopen("book.bin", "rb");
    temp = (book *)malloc(sizeof(book));
    cur = head;
    while(fread(&temp, sizeof(book), 1, file_opened)) {
        temp2 = (book_node *)malloc(sizeof(book_node));
        temp2->book_data = temp;
        if(head == NULL) {
            head = temp2;
        } else {
            cur->next = temp2;
            cur = cur->next;
        }
    }
    return head;
}

user_node *init_user_list() {
    user temp;
    user_node *head, *cur, *temp2;
    file_opened = fopen("user.bin", "rb");
    
    cur = head;
    while(fread(&temp, sizeof(user), 1, file_opened)) {
        temp2 = (user_node *)malloc(sizeof(user_node));
        temp2->user_data = &temp;
        if(head == NULL) {
            head = temp2;
        } else {
            cur->next = temp2;
            cur = cur->next;
        }
    }
    return head;
}

author_node *init_author_list() {
    author temp;
    author_node *head, *cur, *temp2;
    file_opened = fopen("author.bin", "rb");
    
    cur = head;
    while(fread(&temp, sizeof(author), 1, file_opened)) {
        temp2 = (author_node *)malloc(sizeof(author_node));
        temp2->author_data = &temp;
        if(head == NULL) {
            head = temp2;
        } else {
            cur->next = temp2;
            cur = cur->next;
        }
    }
    return head;
}

// BOOKS LINKED LIST EFFECTING FUNCTIONS

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

void increase_count(char *temp_isbn, int qnty) {
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

void decrease_count(char *temp_isbn, int qnty) {
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
int search_book(char *req_attr, int type_attr) {
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
void fsearch_book() {
    system("cls");
    int search_type;
    char temp[20];
    printf("\nSearch by:-\n1.ISBN\n2.Title\n?");
    scanf("%d", &search_type);
    printf("\nValue?");
    scanf("%s", temp);
    if(search_book(temp, search_type))
        printf("Book found.");
    else
        printf("Book not found");
}

// add book data to last node of books linked list.
void add_book(book *book_to_add) {
    book_node *cur, *temp;
    temp = (book_node *)malloc(sizeof(book_node));
    temp->book_data = book_to_add;
    cur = books_head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    temp->next = NULL;
    cur->next = temp;
    printf("\nBook added!");
}

/* follow up funtion of add book in book menu */
void fadd_book() {
    system("cls");
    book new_book;
    printf("\nISBN?");
    scanf("%s", new_book.isbn);
    printf("\nQuantity?");
    scanf("%d", &new_book.quantity);
    if(!search_book(new_book.isbn, 1)) {
        printf("\nBook Title?");
        scanf("%s", new_book.title);
    } else {
            increase_count(new_book.isbn, new_book.quantity);
    }
}

// deletes a book from book.bin
void delete_book(char *req_isbn) {
    book_node *cur, *cur2;
    cur = books_head;
    if(cur->book_data->isbn == req_isbn) {
        books_head = cur->next;
        free(cur);
        return;
    } else {
        while(cur != NULL) {
            if(cur->book_data->isbn == req_isbn) {
                if(cur->next == NULL) {
                    free(cur);
                } else {
                    cur2->next = cur->next;
                    free(cur);
                }
                return;
            }
            cur2 = cur;
            cur = cur->next;
        }
    }
}

void fdelete_book() {
    system("cls");
    char temp_isbn[14];
    int qnty;
    printf("\nISBN?");
    scanf("%s", temp_isbn);
    if(search_book(temp_isbn, 1)) {
        printf("\nHow many?");
        scanf("%d", &qnty);
        delete_book(temp_isbn);
    } else {
        printf("\nNo such book found!");
    }
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
    id[16] = "\0";
    return id;
}

/* search for record in book ledger */
book_loaned *get_record(char *record_bid) {
    book_loaned *temp;
    temp = (book_loaned *)malloc(sizeof(book_loaned));
    int count = 0;
    do {
        temp = dequeue();
        if(temp->borrow_id == record_bid)
            return temp;
        enqueue(temp);
        count++;
    } while(count < lg->size);
    return NULL;
}

/* issue single book */
void issue_book() {
    system("cls");
    book_loaned *new_issue;
    new_issue = (book_loaned *)malloc(sizeof(book_loaned));
    printf("\nUser ID?");
    scanf("%s", new_issue->user_id);
    printf("\nISBN?");
    scanf("%s", new_issue->isbn);
    new_issue->issuing = get_date();
    new_issue->returning = get_date();
    new_issue->returning = get_return_date(new_issue->returning);
    strcpy(new_issue->borrow_id,gen_bor_id(new_issue));
    decrease_count(new_issue->isbn, 1);
    enqueue(new_issue);
}

/* return book */
void return_book() {
    system("cls");
    book_loaned *record;
    char temp_id[17];
    int fine;
    printf("\nBorrow ID?");
    scanf("%s", temp_id);
    record = get_record(temp_id);
    if(record == NULL) {
        printf("\nNo such record found.");
        return;
    } else {
        record->returned = get_date();
        fine = get_fine(record->returned, record->returning);
        if(fine > 0) {
            printf("\nPlease pay %d ruppees fine.", fine);
        }
        printf("\nThank You.");
    }
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
    id[8] = "\0";
    return id;
}

// add a single author data to author linked list
void add_author(author *author_to_add) {
    author_node *cur, *temp;
    temp = (author_node *)malloc(sizeof(author_node));
    temp->author_data = author_to_add;
    cur = authors_head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    temp->next = NULL;
    cur->next = temp;
    printf("\nAuthor added!");}

/* return 1 if the requested attribute exists in author.bin else 0
    1 : author_id
    2 : first_name
    3 : last_name
*/
int search_author(char *req_attr, int type_attr) {
    author_node *cur;
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
void fsearch_author() {
    system("cls");
    char first_name[15], last_name[15];
    printf("\nFirst name?");
    scanf("%s", first_name);
    printf("\nLast Name?");
    scanf("%s", last_name);
    if(search_author(first_name, 2) && search_author(last_name, 3))
        printf("\nAuthor found.");
    else
        printf("\nAuthor not found.");
}

/* printing all the book titles related to requested author */
void author_books() {
    system("cls");
    book_node *cur;
    char temp_aid[9];
    printf("\nAuthor ID?");
    scanf("%s", temp_aid);
    if(search_author(temp_aid, 1)) {
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
    id[8] = "\0";
    return id;
}

/* search for user in users only by user_id, return 1 if found else 0 */
int search_user(char *req_user_id) {
    user_node *cur;
    cur = users_head;
    while(cur != NULL) {
        if(strcmp(cur->user_data->user_id,req_user_id) == 0) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* follow up function of option search user in menu user */ 
void fsearch_user() {
    system("cls");
    char temp_uid[11];
    printf("\nUser Id?");
    scanf("%s", temp_uid);
    if(search_user(temp_uid)) {
        printf("\nUser found.");
    } else {
        printf("\nUser not found.");
    }
}

/* add a user to the users linked list */
void add_user(user_node *user_to_add) {
    user_node *cur;
    cur = users_head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = user_to_add;
}

/* follow up function of option add user in menu user */
void fadd_user() {
    system("cls");
    user *temp_user_data;
    user_node *new_user;
    temp_user_data = (user *)malloc(sizeof(user));
    
    printf("\nUser Name?");
    scanf("%s", temp_user_data->name);
    printf("\nUser Phone Number?");
    scanf("%s", temp_user_data->phone);
    strcpy(temp_user_data->user_id, gen_uid(temp_user_data));

    new_user->user_data = temp_user_data;
    new_user->next = NULL;
    add_user(new_user);
}

/* delete requested user node from users linked list */
void delete_user(char *req_uid) {
    user_node *cur, *cur2;
    cur = users_head;
    if(strcpy(users_head->user_data->user_id, req_uid) == 0) {
        users_head = cur->next;
        cur->next = NULL;
        return;
    } else {
        cur2 = cur;
        cur = cur->next;
        while(cur != NULL) {
            if(cur->user_data->user_id == req_uid) {
                if(cur->next == NULL) {
                    free(cur);
                } else {
                    cur2->next = cur->next;
                    free(cur);
                }
                return;
            }
            cur2 = cur;
            cur = cur->next;
        }
    }

}

/* follow up function of option delete user in menu user */
void fdelete_user() {
    system("cls");
    char temp_uid[9];
    printf("\nUser ID?");
    scanf("%s", temp_uid);
    if(search_user(temp_uid)) {
        delete_user(temp_uid);
    } else {
        printf("\nUser Id not available.");
    }
}

/* writes the updated user linked list on user.bin */
void final_users() {
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

void final_books() {
    file_opened = fopen("book.bin", "wb");
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        fwrite(cur, sizeof(book_node), 1, file_opened);
        cur = cur->next;
    }
    fclose(file_opened);
}

void final_authors() {
    file_opened = fopen("author.bin", "wb");
    author_node *cur;
    cur = authors_head;
    while(cur != NULL) {
        fwrite(cur, sizeof(author_node), 1, file_opened);
        cur = cur->next;
    }
    fclose(file_opened);
}

// SUB MENU PRINTING FUNCTIONS
/* Sub Menu for Books */
void menu_books() {
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
            case 1: fadd_book();break;
            case 2: fdelete_book();break;
            case 3: fsearch_book();break;
            case 4: issue_book();break;
            case 5: return_book();break;
            case 6: return;
        }
    } while(opt > 6 || opt < 1);
}

/* Sub Menu For Authors */
void menu_authors() {
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
            case 1: fsearch_author();break;
            case 2: author_books();break;
            case 3: return;
        } 
    } while(opt > 3 || opt < 1);
}

/* Sub Menu for Users */
void menu_users() {
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
            case 1: fsearch_user();break;
            case 2: fadd_user();break;
            case 3: fdelete_user();break;
            case 4: return;
        }
    } while(opt > 4 || opt < 1);
}

void main() {
    int menu_opt;

    // Initialinzing pointers for different linked lists
    books_head = init_book_list();
    authors_head = init_author_list();
    users_head = init_user_list();
    lg = create_ledger(50);
    fill_ledger();

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
                case 1: menu_books();break;
                case 2: menu_authors();break;
                case 3: menu_users();break;
                case 4: break;
            }
        } while(menu_opt > 4 || menu_opt < 1);    
    } while(menu_opt != 4);

    finalize_ledger();
    final_books();
    final_users();
    final_authors();
    exit(0);
}