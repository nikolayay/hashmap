#include "assoc.h"

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/

static unsigned int hash(void *key)
{
    return 0;
}

static unsigned int hash_str(char *key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
    {

        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

assoc *assoc_init(int keysize)
{

    int i;
    assoc *t = malloc(sizeof(assoc));

    if (!t)
        return NULL;

    if ((t->table = malloc(START_SIZE * sizeof(assoc_element *))) == NULL)
    {
        free(t->table);
        return NULL;
    }

    t->keysize = keysize;
    t->e_num = 0;
    t->size = START_SIZE;

    for (i = 0; i < START_SIZE; i++)
        t->table[i] = NULL;

    return t;
}

void assoc_insert_string(assoc **a, char *key, void *data)
{
    assoc *t = *a;

    unsigned int h = hash_str(key) % t->size;
    assoc_element *e = t->table[h];

    while (e != NULL)
    {
        if (!strcmp(e->key, key))
        {
            e->data = data;
        }
        e = e->next;
    }

    /* the key doesn't already exist */
    e = malloc(sizeof(assoc_element));
    e->key = malloc(strlen(key) + 1);

    strcpy(e->key, key);
    e->data = data;

    /* Add the element at the beginning of the linked list */
    e->next = t->table[h];
    t->table[h] = e;
    t->e_num++;

    /* todo realloc here */
    if (t->e_num + 1 >= t->size)
    {
    }
}

void assoc_insert_bytes(assoc **a, void *key, void *data)
{
    assoc *t = *a;
}

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc **a, void *key, void *data)
{
    assoc *t = *a;

    /* key is a string */
    if (t->keysize == 0)
    {
        assoc_insert_string(a, key, data);
    }
    else
    {
        assoc_insert_bytes(a, key, data);
    }
}

/*
   Returns the number of key/data pairs
   currently stored in the table
*/
unsigned int assoc_count(assoc *a)
{
    return a->e_num;
}

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void *assoc_lookup(assoc *a, void *key)
{
    return NULL;
}

/* Free up all allocated space from 'a' */
void assoc_free(assoc *a)
{
    free(a);
}
