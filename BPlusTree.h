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

struct internalNode{
    //键值数组，指针数组
    int m_Keys[MAX_NUM];
    void * m_Pointers[MAX_NUM];
};



//结点
typedef struct _tagNode
{
    //键的数量
	int m_nKeyNum;
	//键值数组，指针数组
	int m_Keys[MAX_NUM];
	void * m_Pointers[MAX_NUM];
	_tagNode * m_pParent;
	//当前结点是不是叶子结点
	bool m_bLeaf;
	_tagNode * m_pNext;
	_tagNode * m_pPrev;
	_tagNode()
	{
		m_nKeyNum = 0;
		int i = 0;
		for (i = 0; i < MAX_NUM; i++)
		{
			m_Keys[i] = 0;
			m_Pointers[i] = NULL;
		}
		m_pParent = NULL;
		m_bLeaf = false;
		//下一个
		m_pNext = NULL;
		//前一个
		m_pPrev = NULL;
	}
}Node;

//每个存储数据的域
typedef struct _tagLeafNode
{
	int m_nKey;
	double m_number;
	_tagLeafNode()
	{
		m_nKey = 0;
        m_number = 0.0;
	}
}LeafNode;



class BPlusTree
{
public:
	BPlusTree();
	~BPlusTree();
private:
    //根结点
	Node * m_pRoot;
	Node * m_pFirst;
	Node * m_pLast;
public:
	bool Insert(int nKey, double number);
	bool Remove(int nKey);
	//分层打印
	void PrintLayerTree();
	void DestroyTree(Node * & pRoot);
	void DestroyTree();
	bool InsertKeyAndPointer(Node * pParent, Node * pOld, int nKey, Node * pNew);
	void PrintLeaves();
	//翻转打印叶子结点的值
	void ReversePrintLeaves();
};

