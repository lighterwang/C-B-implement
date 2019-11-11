#include <iostream>
#include "BPlusTree.h"
int main() {
    BPlusTree bt;

    srand(rand());

    for (int i = 1; i <= 50; i++)
    {
        //int m = rand();
        int m = i;
        bt.Insert(m, m);
    }

    printf("remove before.\n");
    bt.PrintLeaves();


    for (int i = 0; i < 30; i++)
    {
       // int n = rand();
       int n = i;
        bt.Remove(n);
    }

    printf("remove after.\n");

    //bt.PrintLayerTree();

    bt.PrintLeaves();

    //bt.ReversePrintLeaves();

    return 0;
}