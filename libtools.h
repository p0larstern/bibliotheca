typedef struct issuer
{
    int id[6];
    int num_of_books;
    char books = (char *)malloc(sizeof(char)*num_of_books);
};
