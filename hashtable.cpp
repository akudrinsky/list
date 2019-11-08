//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of hashtables
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef HASHTABLE_WAS_DEFINED                           //flag that list was defined only once in all project
#define HASHTABLE_WAS_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.cpp"
#include <math.h>

#define LOUD_HASTABLE
#ifdef LOUD_HASTABLE
#define hast_OK(hast)\
    if (hast_ok(&(hast)) != true) {\
        ASSERT ((hast).status == ok);\
}
#else
#define hast_OK(hast) ;
#endif

#define Lt_construct(lst) List_Construct(&lst, #lst);

#define List_OK(lst)                                   \
    List_okey (&lst);                                  \
    Dump_f(&lst, __FILE__, __LINE__, __PRETTY_FUNCTION__);

const int hash_seed_for_hast = 144;

#pragma pack(push, 1)
struct hashtable {
    List_t* rows;
    int size;
    int max_size;
    int status;
};
#pragma pack(pop)

void hast_construct (hashtable* hast, const int hast_maxsize = 30);
void hast_destruct (hashtable* hast, const int hast_maxsize = 30);

int hash_it (void* stk, const int number, const int seed);

void add_element (hashtable* hast, char* str, Elem_t value);
Elem_t* search_by_str (hashtable* hast, char* str, int* size);
void delete_element (hashtable* hast, char* str);

bool hast_ok (hashtable* hast);

//void hast_graph (hashtable* hast, const char* pict_name = "hast.png", const char* pict_type = "png");

void hast_test_1 ();

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Hash-table construct
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] hast_maxsize - max elements which it can theoretically store without intersections
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void hast_construct (hashtable* hast, const int hast_maxsize) {
    ASSERT (hast != nullptr)

    hast->rows = (List_t*)calloc (hast_maxsize, sizeof (List_t));
    if (hast->rows == nullptr) {
        printf ("We have no memory left!\n");
        exit (no_memory);
    }
    for (int i = 0; i < hast_maxsize; ++i) {
        Lt_construct (hast->rows[i])
        if (hast->rows[i].status != ok) {
            printf ("One of lists in hash table was not allocated\n");
            exit (no_memory);
        }
    }
    hast->size = 0;
    hast->max_size = hast_maxsize;
    hast->status = ok;

    hast_OK (*hast)
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Hash-table destruct
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] hast_maxsize - max elements which it can theoretically store without intersections
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void hast_destruct (hashtable* hast, const int hast_maxsize) {
    ASSERT (hast != nullptr)

    if (hast->status == destructed) {
        printf ("Trying to destruct hash table which had been destructed\n");
        return;
    }
    for (int i = 0; i < hast_maxsize; ++i) {
        Lt_destruct (&hast->rows[i]);
    }
    free (hast->rows);
    hast->size = 0;
    hast->max_size = 0;
    hast->status = destructed;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes hash of a structure
//!
//! @param [in] stk - pointer to first element
//! @param [in] seed - to make different results
//! @return hash of structure
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int hash_it (void* stk, const int number, const int seed) {
    void* ptr = stk;
    int hash = seed;

    for (int i = 0; i < number; i++) {
        hash =  (hash * 0x239b961b) ^ (int)*((char* )ptr + i);
    }

    return hash;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Adds new element to hash-table
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to add
//! @param [in] value - value to add
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void add_element (hashtable* hast, char* str, Elem_t value) {
    ASSERT (hast != nullptr)
    ASSERT (str != nullptr)

    int str_hash = hash_it (str, strlen (str), hash_seed_for_hast);
    insert_after (&hast->rows[abs (str_hash) % hast->max_size], hast->rows[abs (str_hash) % hast->max_size].tail, value);
    hast->size++;

    hast_OK (*hast)
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Searches for value (or several) by str
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to search by
//! @param [in] size - how many elements were by that exact string
//! @return pointer to first element with corresponding value
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
Elem_t* search_by_str (hashtable* hast, char* str, int* size) {
    ASSERT (hast != nullptr)
    ASSERT (str != nullptr)
    ASSERT (size != nullptr)

    int str_hash = abs (hash_it (str, strlen (str), hash_seed_for_hast));

    if (!hast->rows[str_hash % hast->max_size].is_sorted) {
        phys_match_log(&hast->rows[str_hash % hast->max_size]);
    }

    *size = hast->rows[str_hash % hast->max_size].size;

    hast_OK (*hast)

    return hast->rows[str_hash % hast->max_size].data + 1;                //because first row is for me, now for users
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes element in hash-table
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to delete
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void delete_element (hashtable* hast, char* str) {
    ASSERT (hast != nullptr)
    ASSERT (str != nullptr)

    int str_hash = abs (hash_it (str, strlen (str), hash_seed_for_hast)) % hast->max_size;

    List_t* lst = &hast->rows[str_hash];

    int starting_size = 10;
    for (int i = 1; i < starting_size; ++i) {
        lst->data[i] = 0;
        lst->next[i] = i + 1;
        lst->prev[i] = -1;
    }
    lst->next[starting_size - 1] = 0;

    lst->head = 0;
    lst->tail = 0;
    lst->free = 1;
    lst->size = 0;
    lst->max_size = starting_size;
    lst->is_sorted = true;
    lst->status = ok;

    hast_OK (*hast)
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Answers if hast is valid or not
//!
//! @param [in] hast - pointer to hash table
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool hast_ok (hashtable* hast) {
    if (hast == nullptr) {
        return false;
    }

    if (hast->rows == nullptr) {
        hast->status = nulldata;
        return false;
    }

    if (hast->size > hast->max_size) {
        hast->status = size_out_of_max;
        return false;
    }

    if (hast->max_size < 10) {
        hast->status = size_out_of_max;
        return false;
    }

    if (hast->size < 0) {
        hast->status = size_out_of_max;
        return false;
    }

    if (hast->status != ok) {
        return false;
    }

    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes picture of a hash-table
//!
//! @param [in] hast - pointer to hash table
//! @param [in] pict_name - name of picture (hast.png by default)
//! @param [in] pict_type - type of picture (png by default)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
/*
void hast_graph (hashtable* hast, const char* pict_name, const char* pict_type) {
    FILE* pFile = fopen ("list_graph.dot", "w");
    ASSERT (pFile != nullptr)

    fprintf (pFile, "digraph G{\nedge[color=\"chartreuse4\",fontcolor=\"blue\",fontsize=12];\nrankdir=LR;\nnode[shape=\"rectangle\",fontsize=15];\n");
    fprintf (pFile, "%d[fillcolor=\"palegreen1\"];\n", lst->head);

    int pos = lst->head;
    while (lst->next[pos] != 0) {
        fprintf (pFile, "%d->%d;\n", pos, lst->next[pos]);
        pos = lst->next[pos];
    }

    fprintf (pFile, "edge[color=\"cyan3\",fontcolor=\"blue\",fontsize=12];\n");
    pos = lst->tail;
    while (lst->prev[pos] != 0) {
        fprintf (pFile, "%d->%d;\n", pos, lst->prev[pos]);
        pos = lst->prev[pos];
    }

    fprintf (pFile, "edge[color=\"palevioletred4\",fontcolor=\"blue\",fontsize=12];\n");
    pos = lst->free;
    while (lst->next[pos] != 0) {
        fprintf (pFile, "%d->%d;\n", pos, lst->next[pos]);
        pos = lst->next[pos];
    }

    fprintf (pFile, "}");
    fclose (pFile);

    //executing command in terminal
    const int max_cmd_size = 50;
    char command[max_cmd_size] = "dot list_graph.dot -T ";
    strcat (command, pict_type);
    strcat (command, " -o ");
    strcat (command, pict_name);

    system (command);

}
 */

void hast_test_1 () {
    hashtable hast = {};
    hast_construct (&hast);

    add_element (&hast, "abcd", 133);
    add_element (&hast, "abcd", 233);
    add_element (&hast, "jhvh", 333);

    int size = 0;
    Elem_t* elements = search_by_str (&hast, "abcd", &size);

    if (elements[0] != 133) {
        printf ("hast_text_1 is not ok (1): %d\n", elements[0]);
    }
    if (elements[1] != 233) {
        printf ("hast_text_1 is not ok (2): %d\n", elements[1]);
    }

    delete_element (&hast, "abcd");

    elements = search_by_str (&hast, "abcd", &size);
    if (size != 0) {
        printf ("hast_text_1 is not ok (3): %d\n", size);
    }

    add_element (&hast, "abcd", 131);
    add_element (&hast, "abcd", 2331);
    add_element (&hast, "jhvh", 33312);

    elements = search_by_str (&hast, "abcd", &size);

    if (elements[0] != 131) {
        printf ("hast_text_1 is not ok (4): %d\n", elements[2]);
    }
    if (elements[1] != 2331) {
        printf ("hast_text_1 is not ok (5): %d\n", elements[3]);
    }

    hast_destruct (&hast);
}

#endif