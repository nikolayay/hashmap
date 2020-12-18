#include "assoc.h"

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/

static assoc *internal_assoc_init(int size)
{

    int i;
    assoc *t = malloc(sizeof(assoc));

    if (!t)
        return NULL;

    t->n = 0;
    t->size = size;

    t->table = malloc(t->size * sizeof(assoc_element *));

    assert(t->table != NULL);

    for (i = 0; i < (int)t->size; i++)
        t->table[i] = NULL;

    return t;
}

static unsigned int hash(const char *key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
    {

        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

static int compare_keys(assoc *a, void *k1, void *k2)
{
    return a->keysize == 0 ? !strcmp(k1, k2) : !memcmp(k1, k2, (size_t)a->keysize);
}

static void grow(assoc *a)
{
    assoc *a2;
    assoc swap;

    unsigned int i;
    assoc_element *e;

    a2 = internal_assoc_init(a->size * GROWTH_FACTOR);

    assert(a2->size == a->size * GROWTH_FACTOR);

    for (i = 0; i < a->size; i++)
    {
        for (e = a->table[i]; e != 0; e = e->next)
        {
            assoc_insert(&a2, e->key, e->value);
        }
    }

    swap = *a;
    *a = *a2;
    *a2 = swap;

    assoc_free(a2);
}

assoc *assoc_init(int keysize)
{
    assoc *a = internal_assoc_init(INIT_SIZE);
    a->keysize = keysize;
    return a;
}

void assoc_insert(assoc **a, void *key, void *value)
{

    assoc *t = *a;
    assoc_element *e;
    unsigned int h;

    assert(key);
    //assert(value);
    e = malloc(sizeof(*e));
    assert(e);

    h = hash(key) % t->size;

    assoc_element *el;

    for (el = t->table[h]; el != 0; el = el->next)
    {
        if (compare_keys(t, el->key, key))
        {
            return;
        }
    }

    if (t->keysize == 0)
    {
        e->key = strdup(key);
        e->value = malloc(sizeof(void *));
        memcpy(e->key, key, strlen(key));
    }
    else
    {
        e->key = malloc((size_t)t->keysize);
        e->value = malloc(sizeof(void *));
        memcpy(e->key, key, t->keysize);
    }

    if (value)
    {
        memcpy(e->value, value, sizeof(void *));
    }

    e->next = t->table[h];
    t->table[h] = e;

    t->n++;

    if (t->n >= t->size * MAX_LOAD_FACTOR)
    {
        grow(t);
    }
}

/*
   Returns the number of key/data pairs
   currently stored in the table
*/
unsigned int assoc_count(assoc *a)
{
    return a->n;
}

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void *assoc_lookup(assoc *a, void *key)
{
    assoc_element *e;

    for (e = a->table[hash(key) % a->size]; e != 0; e = e->next)
    {
        // same string
        if (compare_keys(a, e->key, key))
        {
            return e->value;
        }
    }

    return 0;
}

/* Free up all allocated space from 'a' */
void assoc_free(assoc *a)
{
    unsigned int i;
    assoc_element *e;
    assoc_element *next;

    for (i = 0; i < a->size; i++)
    {
        for (e = a->table[i]; e != NULL; e = next)
        {
            next = e->next;

            free(e->key);
            free(e->value);
            free(e);
        }
    }

    free(a->table);
    free(a);
}

void test()
{
    assert(hash("farandoles") == 1561596836);

    printf("all unit tests pass\n");
}
