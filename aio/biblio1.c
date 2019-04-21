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
    char borrow_id[10];
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
    struct bn *next;
};
typedef struct un user_node;

struct an {
    author *author_data;
    struct an *next;
};
typedef struct an author_node;

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

book_node *init_book_list() {
    book temp;
    book_node *head, *cur, *temp2;
    file_opened = fopen("book.bin", "rb");
    
    cur = head;
    while(fread(&temp, sizeof(book), 1, file_opened)) {
        temp2 = (book_node *)malloc(sizeof(book_node));
        temp2->book_data = &temp;
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
/* generates book id */
char gen_bid(book *details) {
    
}

void add_books() {
    book new_book;
    printf("\nISBN?");
    scanf("%s", new_book.isbn);
    printf("\nQuantity?");
    scanf("%d", &new_book.quantity);
    if(!search_book(&new_book.isbn, 1)) {
        printf("\nBook Title?");
        scanf("%s", new_book.title);
        new_book.deleted = 0;
    } else {
            increase_count(&new_book.isbn, new_book.quantity, start);
        }
    }
}

void delete_books(book_node *start) {
    int n, i, qnty;
    char temp_isbn[14];
    printf("\nNumber of different books to delete?");
    scanf("%d", &n);

    for(i=0; i<n; i++) {
        printf("\nBook #%d", i+1);
        printf("\nISBN?");
        scanf("%s", temp_isbn);
        if(search_book(temp_isbn, 1, start)) {
            printf("\nHow many?");
            scanf("%d", &qnty);
            delete_book(temp_isbn, start);
        } else {
            printf("\nNo such book found!");
        }
    }
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

// deletes a book from book.bin
void delete_book(char *req_isbn) {
    book_node *cur, *cur2;
    cur = books_head;
    if(cur->book_data->isbn == req_isbn) {
        start = cur->next;
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

/* return 1 if the requested attribute exists in book.bin else 0
    1 : isbn
    2 : title
    3 : author_id
*/
int search_book(char req_attr[], int type_attr) {
    book_node *cur;
    cur = books_head;
    while(cur != NULL) {
        switch(type_attr) {
            case 1: if(cur->book_data->isbn == req_attr)
                        return 1;
            case 2: if(cur->book_data->title == req_attr)
                        return 1;
            case 3: if(cur->book_data->author_id == req_attr)
                        return 1;
        }
        cur = cur->next;
    }
    return 0;
}


// AUTHORS LINKED LIST EFFECTING FUNCTIONS
/* generates author_id 
example :-                0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
    author->first_name -  r o b e r t \0
    author->last_name -   f r o s t \0
then author->id -         r o b e f r o s \0
*/
char gen_aid(author *data) {
    char id[9];
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
            case 1: if(cur->author_data->author_id == req_attr)
                        return 1;
            case 2: if(cur->author_data->first_name == req_attr)
                        return 1;
            case 3: if(cur->author_data->last_name == req_attr)
                        return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* follow up function for menu option of author search */
void fsearch_author() {
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
char gen_uid(user *stuff) {
    char id[9];
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

void enqueue(book_ledgar *lg, book_loaned *element) {
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

// updates book_loaned.bin a/c to final ledger
void finalize_ledger(book_ledgar *lg) {
    file_opened = fopen("book_loaned.bin", "wb");
    book_loaned *temp;

    while(!is_empty(lg)) {
        temp = dequeue(lg);
        fwrite(&temp, sizeof(book_loaned), 1, file_opened);
    }
}

// SUB MENU PRINTING FUNCTIONS
/* Sub Menu for Books */
void menu_books() {
    int opt;
    printf("\n==================== BOOKS ====================");
    printf("\n1. Add Book");
    printf("\n2. Delete Book");
    printf("\n3. Search Book");
    printf("\n4. Issue Package");
    printf("\n5. Return Package");
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
            case 4: issue_package();break;
            case 5: return_package();break;
            case 6: return;
        }
    } while(opt > 6 || opt < 1);
}

/* Sub Menu For Authors */
void menu_authors() {
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

// Making the pointers for various linked lists global for easy usage
book_ledgar *ledger;
book_node *books_head;
author_node *authors_head;
user_node *users_head;

void main() {
    int menu_opt;

    // Initialinzing pointers for different linked lists
    books_head = init_book_list();
    authors_head = init_author_list();
    users_head = init_user_list();
    ledger = create_ledger(50);

    do {
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

    finalize_ledger(ledger);
    exit(0);
}