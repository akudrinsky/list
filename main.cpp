#include <stdio.h>
#include "list.cpp"

int main() {
    List_t lst = {};
    Lt_construct (lst)
    insert_after (&lst, 0, 11);
    insert_after (&lst, 1, 22);
    list_graph (&lst, "pict1.png");

    insert_after (&lst, 1, 33);
    list_graph (&lst, "pict2.png");
    insert_after (&lst, 1, 44);
    insert_after (&lst, 1, 55);

    list_graph (&lst, "pict3.png");

    delete_after (&lst, 1);

    list_graph (&lst, "pict4.png");

    Lt_destruct (&lst);

    return 0;
}
