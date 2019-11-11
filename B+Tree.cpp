// B+Tree.cpp : 定义控制台应用程序的入口点。
//
#include "BPlusTree.h"

int main()
{
	BPlusTree bt;

	srand(rand());

	for (int i = 1; i <= 50; i++)
	{
		char str[256] = { 0 };
		int m = rand();
		sprintf(str, "name%d", m);
		string strName;
		strName = str;
		bt.Insert(m, strName);
	}		
	
	for (int i = 0; i < 30; i++)
	{
		int n = rand();
		bt.Remove(n);
	}	

	printf("remove after.\n");

	printf("print b+tree:\n");

	bt.PrintLayerTree();

	bt.PrintLeaves();

	bt.ReversePrintLeaves();

	return 0;
}

