#define INIT_SIZE 16
#define GROWTH_FACTOR 2
#define MAX_LOAD_FACTOR 1

typedef struct assoc_element
{
    struct assoc_element *next; /* Next element in case of a collision */
    void *value;                /* Pointer to the stored element */
    void *key;                  /* Key of the stored element */
} assoc_element;

typedef struct assoc
{
    unsigned int keysize;
    unsigned int size;     /*capacity of array*/
    unsigned int n;        /* current number of items */
    assoc_element **table; /* The table containaing elements */
} assoc;
