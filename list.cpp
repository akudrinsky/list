//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of stack
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef LIST_WAS_DEFINED                           //flag that list was defined only once in all project
#define LIST_WAS_DEFINED

#include <stdio.h>
#include <stdlib.h>

#define Lt_construct(lst) List_Construct(&lst, #lst);

#define List_OK(lst)                                   \
    List_okey (&lst);                                  \
    Dump_f(&lst, __FILE__, __LINE__, __PRETTY_FUNCTION__);

typedef int Elem_t;

enum lt_errors_c {
    ok,
    nulldata,
    size_out_of_max,
    canary_t,
    canary_b,
    bad_hash,
    destructed
};

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! List structure
//!
//! @param [in] name - name of list
//! @param [in] data - array of list values
//! @param [in] next - array, where you can understand which physical number is after a given one
//! @param [in] prev - array, where you can understand which physical number is before a given one
//! @param [in] head - physical number of first (logically) element
//! @param [in] tail - physical number of last (logically) element
//! @param [in] size - number of elements in stack
//! @param [in] is_sorted - if list physical numbers correspond to logical ones
//! @param [in] is_valid - if it is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#pragma pack(push, 1)
struct List_t {
    char* name;
    Elem_t* data;
    Elem_t* next;
    Elem_t* prev;
    int head;
    int tail;
    int free;
    int size;
    int max_size;
    bool is_sorted;
    lt_errors_c is_valid;
};
#pragma pack(pop)

void List_Construct (List_t* lst, char* lst_name = "lst");
void Lt_destruct (List_t* lst);
void Dump_f (List_t* lst, const char* file, int line,
             const char* func_name, char* info = "assert",
             bool is_silent = false, bool no_corr = false,
             const char* file_name = "list_dump.txt");
bool List_okey (List_t* lst);



//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! List construct
//!
//! @param [in] lst - pointer to list
//! @param [in] lst_name - name of list (if you don't need it, it will be "lst" by default)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void List_Construct (List_t* lst, char* lst_name) {
    const int starting_size = 10;

    lst->name = lst_name;
    lst->data = (Elem_t *)calloc (starting_size, sizeof(Elem_t));
    lst->next = (Elem_t *)calloc (starting_size, sizeof(Elem_t));

    for (int i = 1; i < starting_size - 1; ++i) {
        lst->next[i] = i + 1;
    }
    lst->next[starting_size - 1] = 0;

    lst->prev = (Elem_t *)calloc (starting_size, sizeof(Elem_t));
    lst->head = 0;
    lst->tail = 0;
    lst->free = 1;
    lst->size = 0;
    lst->max_size = starting_size;
    lst->is_sorted = true;
    lst->is_valid = ok;

    List_OK (*lst);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Stack desctruct
//!
//! @param [in] lst - pointer to stack
//! @param [in] lst_name - name of stack (if you don't need it, it will be "lst" by default)
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Lt_destruct (List_t* lst) {

    lst->name = nullptr;
    free (lst->data);
    free (lst->next);
    free (lst->prev);
    lst->head = 0;
    lst->tail = 0;
    lst->free = 0;
    lst->size = 0;
    lst->max_size = 0;
    lst->is_sorted = true;
    lst->is_valid = destructed;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! @param [in] lst - pointer to stack
//! @param [in] file - file where this Dump_f was called
//! @param [in] line - line in file where this Dump_f was called
//! @param [in] func_name - name of function in file where this Dump_f was called
//! @param [in] is_silent - if you need to stop the program if Stack is not ok
//! @param [in] no_corr - if you need to print info about valid stacks or not
//! @param [in] file_name - where to print info about stacks
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Dump_f (List_t* lst, const char* file,
             int line, const char* func_name, char* info,
             bool is_silent, bool no_corr, const char* file_name) {
    FILE* pFile = fopen (file_name, "ab");
    /*
    const char* status = "ok";
    if (lst->is_valid != ok)                                                             //int or Elem_t????
        status = "NOT OK";

    if (!no_corr || lst->is_valid != 0) {
        fprintf (pFile, "%s from %s in line %d (file: %s)\n", info, func_name, line, file);
        fprintf (pFile, "Stack_t <%s> [%x] (%s)\n", lst->name, lst, status);
        fprintf (pFile, "{\n\tsize = %d\n\tdata[%d] = %x\n", lst->size, lst->max_size, lst->data);
        fprintf (pFile, "\t{\n");

        for (int i = 0; i < lst->size; ++i) {
            fprintf (pFile, "\t* [%d] = %d\n", i, lst->data[i]);
        }

        for (int i = lst->size; i < lst->max_size; ++i) {
            fprintf (pFile, "\t  [%d] = %d\n", i, lst->data[i]);
        }
        fprintf(pFile, "\t}\n");

        switch (lst->is_valid) {
            case ok:
                break;
            case nulldata:
                fprintf (pFile, "Pointer to data is nullptr\n");
                break;
            case pop_last:
                fprintf (pFile, "Trying to pop non-existing element\n");
                break;
            case size_out_of_max:
                fprintf (pFile, "Number of elements is bigger than max value\n");
                break;
            case destructed:
                fprintf (pFile, "Stack was destructed\n");
                break;
                ON_DEBUG (case canary_t:
                fprintf (pFile, "Top canary has been changed\n");
                break;
            case canary_b:
                fprintf (pFile, "Bottom canary has been changed\n");
                break;
            case bad_hash:
                fprintf (pFile, "Hash is not good\n");
                break;
                )
        }

        ON_DEBUG (fprintf (pFile, "hash is now: %d\n", lst->hash);)
        fprintf (pFile, "}\n\n\n");
    }

    if (!is_silent && lst->is_valid != 0) {
        printf ("Abort was made. For info look in stack_dump.txt\n");
        abort ();
    }
     */
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Answers if list is valid or not
//!
//! @param [in] lst - pointer to list
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool List_okey (List_t* lst) {
    if (lst == nullptr) {
        return false;
    }

    if (lst->data == nullptr) {
        lst->is_valid = nulldata;
        return false;
    }

    if (lst->size > lst->max_size) {
        lst->is_valid = size_out_of_max;
        return false;
    }

    if (lst->max_size < 10) {
        return false;
    }

    if (lst->size < 0) {
        return false;
    }

    if (lst->is_valid != ok) {
        return false;
    }
    return true;
}

#endif