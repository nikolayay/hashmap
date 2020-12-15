#define START_SIZE 16

typedef struct assoc_element
{
    struct assoc_element *next; /* Next element in case of a collision */
    void *data;                 /* Pointer to the stored element */
    void *key;                  /* Key of the stored element */
} assoc_element;

typedef struct assoc
{
    unsigned int size;     /* size of key datatype */
    unsigned int keysize;  /* size of key datatype */
    unsigned int e_num;    /* Number of element currently stored in the hashtable */
    assoc_element **table; /* The table containaing elements */
} assoc;

typedef struct
{
    assoc *t;            /* The hashtable on which we iterate */
    unsigned int index;  /* Current index in the table */
    assoc_element *elem; /* Curent element in the list */
} assoc_it;
