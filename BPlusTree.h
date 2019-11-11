#pragma once
//ÿ��Ҷ������ܴ洢���������
#define LEAF_ORDER 3
#define ORDER 4
#define MAX_NUM 5

#include <string>
using namespace std;

/*
��B+���У�ÿ����֧���Ĺؼ��ֺ�����֮��Ĺ�ϵ�ǣ�
��0�������Ĺؼ��ֶ�С�ڵ�0���ؼ��֣�
֮��������i��ʼ��i>=0��i+1�������Ĺؼ���
�����ڵ���i�Ĺؼ��֣�С��i+1�ؼ��֡�
ע��ؼ��ֺ��������������Ǵ�0��ʼ�ġ�
*/

struct internalNode{
    //��ֵ���飬ָ������
    int m_Keys[MAX_NUM];
    void * m_Pointers[MAX_NUM];
};



//���
typedef struct _tagNode
{
    //��������
	int m_nKeyNum;
	//��ֵ���飬ָ������
	int m_Keys[MAX_NUM];
	void * m_Pointers[MAX_NUM];
	_tagNode * m_pParent;
	//��ǰ����ǲ���Ҷ�ӽ��
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
		//��һ��
		m_pNext = NULL;
		//ǰһ��
		m_pPrev = NULL;
	}
}Node;

//ÿ���洢���ݵ���
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
    //�����
	Node * m_pRoot;
	Node * m_pFirst;
	Node * m_pLast;
public:
	bool Insert(int nKey, double number);
	bool Remove(int nKey);
	//�ֲ��ӡ
	void PrintLayerTree();
	void DestroyTree(Node * & pRoot);
	void DestroyTree();
	bool InsertKeyAndPointer(Node * pParent, Node * pOld, int nKey, Node * pNew);
	void PrintLeaves();
	//��ת��ӡҶ�ӽ���ֵ
	void ReversePrintLeaves();
};

