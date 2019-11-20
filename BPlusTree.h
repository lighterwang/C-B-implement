#pragma once
//每个叶结点所能存储的最大数量
#define LEAF_ORDER 3
#define ORDER 4
#define MAX_NUM 5

#include <string>
using namespace std;

/*
在B+树中，每个分支结点的关键字和子树之间的关系是：
第0颗子树的关键字都小于第0个关键字，
之后索引从i开始，i>=0，i+1的子树的关键字
都大于等于i的关键字，小于i+1关键字。
注意关键字和子树的索引都是从0开始的。
*/

//模拟40byte的内存单元
struct InternalNodeData{
    //键值数组，指针数组
    int m_Keys[ORDER];
    void * m_Pointers[MAX_NUM];
};

//模拟40字节的内存单元
struct LeafNodeData{
    int m_Keys[LEAF_ORDER];
    double m_number[LEAF_ORDER];
    void* m_next;
};

class Node{
public:
    //键的数量
    int m_nKeyNum;
    //是不是叶结点
    bool m_bLeaf;
    //父亲结点
    Node *m_pParent{};
    Node()
    {
        m_nKeyNum = 0;
        m_bLeaf = false;
    }
};

class InternalNode:public Node{
public:
    InternalNodeData* internalNodeData;
    InternalNode(){
        internalNodeData = new InternalNodeData;
        for(int & m_Key : internalNodeData->m_Keys){
            m_Key=0;
        }
        for(int i=0;i<LEAF_ORDER;i++){
            internalNodeData->m_Pointers[i]=nullptr;
        }
    };
};

class LeafNode:public Node{
public:
    LeafNodeData* leafNodeData;
    LeafNode(){
        leafNodeData = new LeafNodeData;
        for(int i=0;i<ORDER;i++){
            leafNodeData->m_Keys[i]=0;
        }
        for(int i=0;i<MAX_NUM;i++){
            leafNodeData->m_number[i]=0.0;
        }
        leafNodeData->m_next = nullptr;
    };
};
class BPlusTree
{
public:
	BPlusTree();
	~BPlusTree();
private:
    //根结点
	Node * m_pRoot;
    //第一个叶子结点
	Node * m_pFirst;
	Node * m_pLast;
public:
    //叶子结点插入
	bool Insert(int nKey, double number);
    //删除
	bool Remove(int nKey);
	//内部结点插入
	bool InsertKeyAndPointer(Node * pParent, Node * pOld, int nKey, Node * pNew);
	//打印叶子结点值
	void PrintLeaves();
	//查找某个值是否存在
	bool search(int nKey);
    //分层打印
    void PrintLayerTree();
};

