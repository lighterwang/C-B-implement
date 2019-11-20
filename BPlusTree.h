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

//ģ��40byte���ڴ浥Ԫ
struct InternalNodeData{
    //��ֵ���飬ָ������
    int m_Keys[ORDER];
    void * m_Pointers[MAX_NUM];
};

//ģ��40�ֽڵ��ڴ浥Ԫ
struct LeafNodeData{
    int m_Keys[LEAF_ORDER];
    double m_number[LEAF_ORDER];
    void* m_next;
};

class Node{
public:
    //��������
    int m_nKeyNum;
    //�ǲ���Ҷ���
    bool m_bLeaf;
    //���׽��
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
    //�����
	Node * m_pRoot;
    //��һ��Ҷ�ӽ��
	Node * m_pFirst;
	Node * m_pLast;
public:
    //Ҷ�ӽ�����
	bool Insert(int nKey, double number);
    //ɾ��
	bool Remove(int nKey);
	//�ڲ�������
	bool InsertKeyAndPointer(Node * pParent, Node * pOld, int nKey, Node * pNew);
	//��ӡҶ�ӽ��ֵ
	void PrintLeaves();
	//����ĳ��ֵ�Ƿ����
	bool search(int nKey);
    //�ֲ��ӡ
    void PrintLayerTree();
};

