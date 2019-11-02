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
    insert_after (&lst, 2, 44);

    list_graph (&lst, "pict3.png");

    delete_before (&lst, 2);

    list_graph (&lst, "pict4.png");

    phys_match_log (&lst);

    list_graph (&lst, "pict5.png");

    Lt_destruct (&lst);

    return 0;
}
