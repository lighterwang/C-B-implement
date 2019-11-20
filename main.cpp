#include <iostream>
#include <windows.h>
#include "BPlusTree.h"

#define M 50
#define N 100
int main() {

    BPlusTree bt;
    DWORD t1,t2;
    t1 = GetTickCount();
   //插入测试
    printf("----------------------insert test------------------\n");
    for (int i = 1; i <= M; i++)
    {
        int m = i;
        Sleep(20);
        bt.Insert(m, (double)m);
    }
    t2 = GetTickCount();
    printf("Insert for %d times:%d ms,Average:%lf ms\n",M,t2-t1,(t2-t1)*1.0/M);
    bt.PrintLeaves();

    //查询测试
    printf("----------------------search test------------------\n");
    t1 = GetTickCount();
    for(int i=1;i<=N;i++){
        Sleep(20);
        if(bt.search(i)){
            printf("B+ exsit:%d\n",i);
        }else{
            printf("B+ not exsit:%d\n",i);
        }
    }
    t2 = GetTickCount();
    printf("search for %d times:%d ms,Average:%lf ms\n",N,t2-t1,(t2-t1)*1.0/N);
    //删除测试
    printf("----------------------delete test------------------\n");

    t1 = GetTickCount();
    for (int i = 1; i <= M; i++)
    {
        int n = i;
        bt.Remove(n);
        Sleep(20);
        //bt.PrintLeaves();
    }
    t2 = GetTickCount();
    printf("delete for %d times:%d ms,,Average:%lf ms\n\n",M,t2-t1,(t2-t1)*1.0/M);

//    //反向删除测试
//    printf("反向删除测试\n");
//    for (int i = 50; i >= 1; i--)
//    {
//        int n = i;
//        bt.Remove(n);
//        Sleep(1000);
//        bt.PrintLeaves();
//    }
//
//    //中间删除测试
//    for(int i =25;i>=1;i--){
//        int n = i;
//        bt.Remove(n);
//        Sleep(20);
//        bt.PrintLeaves();
//    }
//    for(int i=26;i<=50;i++){
//        int n = i;
//        bt.Remove(n);
//        Sleep(20);
//        bt.PrintLeaves();
//    }


    return 0;
}