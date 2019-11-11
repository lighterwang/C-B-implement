#include "BPlusTree.h"
#include <queue>
using namespace std;


BPlusTree::BPlusTree()
{
	m_pRoot = NULL;
	m_pFirst = NULL;
	m_pLast = NULL;
}


BPlusTree::~BPlusTree()
{
	DestroyTree();
}

bool BPlusTree::Insert(int nKey, double number)
{
	int i = 0;
	int m = 0;
	
	LeafNode * pLeafNode = new LeafNode();

	//申请空间失败
	if (!pLeafNode)
	{
		return false;
	}

	pLeafNode->m_nKey = nKey;
	pLeafNode->m_number = number;

	if (!m_pRoot)
	{
		//树空
		Node * pNew = new Node;
		if (!pNew)
		{
			if (pLeafNode)
				delete pLeafNode;
			return false;
		}

		pNew->m_Keys[0] = nKey;
		pNew->m_bLeaf = true;
		pNew->m_Pointers[0] = pLeafNode;
		pNew->m_nKeyNum = 1;
		pNew->m_pParent = NULL;
		m_pRoot = pNew;
		m_pFirst = pNew;
		m_pLast = pNew;
		return true;
	}

	//找到插入的叶节点
	Node * pTmp = m_pRoot;

	//当前结点不是叶结点，向下寻找
	while (pTmp->m_bLeaf == false)
	{
		for (i = 0; i < pTmp->m_nKeyNum; i++)
		{

			if (nKey < pTmp->m_Keys[i])
				break;
		}
		if (i == 0)
			pTmp = (Node *)pTmp->m_Pointers[0];
		else if (i == pTmp->m_nKeyNum)
			pTmp = (Node *)pTmp->m_Pointers[i];
		else
			pTmp = (Node *)pTmp->m_Pointers[i];
	}

	//判断叶节点中是否存在
	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
	    //想要插入的叶子结点已经存在
		if (nKey == pTmp->m_Keys[i])
		{
			if (pLeafNode)
				delete pLeafNode;

			return false;
		}
	}

	//判断叶子节点的数量
	if (pTmp->m_nKeyNum < LEAF_ORDER)
	{
		for (i = 0; i < pTmp->m_nKeyNum; i++)
		{
			if (nKey < pTmp->m_Keys[i])
			{
				break;
			}
		}

		//移动
		for (m = pTmp->m_nKeyNum - 1; m >= i; m--)
		{
			pTmp->m_Keys[m + 1] = pTmp->m_Keys[m];
			pTmp->m_Pointers[m + 1] = pTmp->m_Pointers[m];
		}
		pTmp->m_Keys[i] = nKey;
		pTmp->m_Pointers[i] = pLeafNode;
		pTmp->m_nKeyNum++;
		return true;
	}

	//叶子节点已经满了
	int nMid = (LEAF_ORDER + 1) / 2;

	int * pTmpKeys = new int[LEAF_ORDER + 1];
	void ** pTmpPointers = new void * [LEAF_ORDER + 1];

	if (!pTmpKeys || !pTmpPointers)
	{
		if (pTmpKeys)
			delete[] pTmpKeys;

		if (pTmpPointers)
			delete[] pTmpPointers;

		if (pLeafNode)
			delete pLeafNode;

		return false;
	}

	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
		if (pTmp->m_Keys[i] > nKey)
			break;
	}

	for (m = pTmp->m_nKeyNum - 1; m >= i; m--)
	{
		pTmpKeys[m + 1] = pTmp->m_Keys[m];
		pTmpPointers[m + 1] = pTmp->m_Pointers[m];
	}

	for (m = 0; m < i; m++)
	{
		pTmpKeys[m] = pTmp->m_Keys[m];
		pTmpPointers[m] = pTmp->m_Pointers[m];
	}

	pTmpKeys[i] = nKey;
	pTmpPointers[i] = pLeafNode;

	Node * pNew = new Node;
	if (!pNew)
	{
		if (pLeafNode)
			delete pLeafNode;

		if (pTmpKeys)
			delete[] pTmpKeys;

		if (pTmpPointers)
			delete[] pTmpPointers;

		return false;
	}

	for (m = 0, i = nMid; i < LEAF_ORDER + 1; i++,m++)
	{
		pNew->m_Keys[m] = pTmpKeys[i];
		pNew->m_Pointers[m] = pTmpPointers[i];
		pNew->m_nKeyNum++;
	}

	pNew->m_pParent = pTmp->m_pParent;

	pNew->m_bLeaf = pTmp->m_bLeaf;

	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
		pTmp->m_Keys[i] = 0;
		pTmp->m_Pointers[i] = NULL;
	}

	pTmp->m_nKeyNum = 0;

	for (i = 0; i < nMid; i++)
	{
		pTmp->m_Keys[i] = pTmpKeys[i];
		pTmp->m_Pointers[i] = pTmpPointers[i];
		pTmp->m_nKeyNum++;
	}

	pNew->m_pPrev = pTmp;
	pNew->m_pNext = pTmp->m_pNext;
	if (pTmp->m_pNext)
		pTmp->m_pNext->m_pPrev = pNew;
	pTmp->m_pNext = pNew;

	if (pNew->m_pNext == NULL)
	{
		m_pLast = pNew;
	}

	if (InsertKeyAndPointer(pTmp->m_pParent, pTmp, pTmpKeys[nMid], pNew) == false)
	{
		if (pLeafNode)
			delete pLeafNode;

		if (pTmpKeys)
			delete[] pTmpKeys;

		if (pTmpPointers)
			delete[] pTmpPointers;

		if (pNew)
			delete pNew;

		return false;
	}

	if (pTmpKeys)
		delete[] pTmpKeys;

	if (pTmpPointers)
		delete[] pTmpPointers;

	return true;
}

void BPlusTree::PrintLeaves()
{
	int i = 0;
	if (!m_pFirst)
		return;
	Node * pCur = m_pFirst;
	printf("b+tree leaves:\n\n");
	while (pCur)
	{
		for (i = 0; i < pCur->m_nKeyNum; i++)
		{
			LeafNode * pTmp = (LeafNode *)pCur->m_Pointers[i];
			if (pTmp)
			{
				printf("<key:%d, number:%lf>", pTmp->m_nKey, pTmp->m_number);
			}
		}
		printf("\n------------------------------------------------------\n");
		pCur = pCur->m_pNext;
	}
}

void BPlusTree::ReversePrintLeaves()
{
	int i = 0;
	if (!m_pLast)
		return;
	Node * pCur = m_pLast;
	printf("reverse print b+tree leaves:\n\n");
	while (pCur)
	{
		for (i = pCur->m_nKeyNum - 1; i >= 0;  i--)
		{
			LeafNode * pTmp = (LeafNode *)pCur->m_Pointers[i];
			if (pTmp)
			{
				printf("<key:%d, name:%s>", pTmp->m_nKey, pTmp->m_number);
			}
		}
		printf("\n------------------------------------------------------\n");
		pCur = pCur->m_pPrev;
	}
}

bool BPlusTree::InsertKeyAndPointer(Node * pParent, Node * pOld, int nKey, Node * pNew)
{
	if (!pOld)
		return false;

	if (!pNew)
		return false;

	int i = 0;

	int m = 0;

	int k = 0;

	//父结点不存在，就是根结点的父亲
	if (pParent == NULL)
	{
		//产生新根
		Node * pNewRoot = new Node;
		if (!pNewRoot)
			return false;
		pNewRoot->m_bLeaf = false;
		pNewRoot->m_Keys[0] = nKey;
		pNewRoot->m_nKeyNum = 1;
		pNewRoot->m_Pointers[0] = pOld;
		pNewRoot->m_Pointers[1] = pNew;
		pNewRoot->m_pParent = NULL;
		pOld->m_pParent = pNewRoot;
		pNew->m_pParent = pNewRoot;
		m_pRoot = pNewRoot;
		return true;
	}

	for (i = 0; i < pParent->m_nKeyNum; i++)
	{
		if (nKey == pParent->m_Keys[i])
			return false;
		if (nKey < pParent->m_Keys[i])
			break;
	}

	//判断父结点是否已经满了
	if (pParent->m_nKeyNum < ORDER)
	{
		//移动关键字
		for (m = pParent->m_nKeyNum - 1; m >= i; m--)
		{
			pParent->m_Keys[m + 1] = pParent->m_Keys[m];
		}

		//移动孩子
		for (m = pParent->m_nKeyNum; m > i; m--)
		{
			pParent->m_Pointers[m + 1] = pParent->m_Pointers[m];
		}

		pParent->m_Keys[i] = nKey;
		pParent->m_Pointers[i + 1] = pNew;
		pParent->m_nKeyNum++;

		pNew->m_pParent = pParent;

		return true;
	}

	//父亲结点已经满了

	int * pTmpKeys = new int[ORDER + 1];
	if (!pTmpKeys)
		return false;

	void ** pTmpPointers = new void *[ORDER + 2];
	if (!pTmpPointers)
	{
		if (pTmpKeys)
			delete[] pTmpKeys;
		return false;
	}

	for (i = 0; i < pParent->m_nKeyNum; i++)
	{
		if (nKey < pParent->m_Keys[i])
			break;
	}

	for (m = 0; m < i; m++)
	{
		pTmpKeys[m] = pParent->m_Keys[m];
	}

	pTmpKeys[m] = nKey;
	m++;

	for (k = i; k < pParent->m_nKeyNum; k++,m++)
	{
		pTmpKeys[m] = pParent->m_Keys[k];
	}

	for (m = 0; m <= i; m++)
	{
		pTmpPointers[m] = pParent->m_Pointers[m];
	}

	pTmpPointers[m] = pNew;

	m++;

	for (k = i + 1; k <= pParent->m_nKeyNum; k++,m++)
	{
		pTmpPointers[m] = pParent->m_Pointers[k];
	}

	Node * pNewSplit = new Node;

	if (!pNewSplit)
	{
		if (pTmpKeys)
			delete[] pTmpKeys;
		if (pTmpPointers)
			delete[] pTmpPointers;
		return false;
	}

	pNewSplit->m_bLeaf = pParent->m_bLeaf;

	pNewSplit->m_pParent = pParent->m_pParent;

	int nMid = (ORDER + 1) / 2;

	int nMidKey = pTmpKeys[nMid];

	for (m = 0, i = nMid + 1; i < ORDER + 1; i++, m++)
	{
		pNewSplit->m_Keys[m] = pTmpKeys[i];
		pNewSplit->m_Pointers[m] = pTmpPointers[i];
		if (pNewSplit->m_bLeaf == false)
		{
			Node * pCur = static_cast<Node *>(pNewSplit->m_Pointers[m]);
			if (pCur)
			{
				pCur->m_pParent = pNewSplit;
			}
		}
		pNewSplit->m_nKeyNum++;
	}

	pNewSplit->m_Pointers[m] = pTmpPointers[i];

	if (pNewSplit->m_bLeaf == false)
	{
		Node * pCur = static_cast<Node *>(pNewSplit->m_Pointers[m]);
		if (pCur)
		{
			pCur->m_pParent = pNewSplit;
		}
	}

	pParent->m_nKeyNum = 0;

	for (i = 0; i < nMid; i++)
	{
		pParent->m_Keys[i] = pTmpKeys[i];
		pParent->m_nKeyNum++;
	}

	for (i = 0; i <= nMid; i++)
	{
		pParent->m_Pointers[i] = pTmpPointers[i];
	}

	if (pTmpKeys)
		delete[] pTmpKeys;

	if (pTmpPointers)
		delete[] pTmpPointers;

	return InsertKeyAndPointer(pParent->m_pParent, pParent, nMidKey, pNewSplit);
}

bool BPlusTree::Remove(int nKey)
{
	if (!m_pRoot)
		return false;

	int i = 0;

	int m = 0;

	Node * pTmp = m_pRoot;

	while (pTmp->m_bLeaf == false)
	{
		for (i = 0; i < pTmp->m_nKeyNum; i++)
		{
			if (nKey < pTmp->m_Keys[i])
			{
				break;
			}
		}

		pTmp = (Node *)pTmp->m_Pointers[i];
	}

	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
		if (nKey == pTmp->m_Keys[i])
			break;
	}

	if (i == pTmp->m_nKeyNum)
		return false;

	LeafNode * pCur = (LeafNode *)pTmp->m_Pointers[i];

	if (pCur)
		delete pCur;

	for (m = i + 1; m < pTmp->m_nKeyNum; m++)
	{
		pTmp->m_Keys[m-1] = pTmp->m_Keys[m];
		pTmp->m_Pointers[m-1] = pTmp->m_Pointers[m];
	}

	pTmp->m_nKeyNum--;

	int nLowNum = (LEAF_ORDER + 1) / 2;

	if (pTmp->m_nKeyNum >= nLowNum)
	{
		return true;
	}

	//下溢出

	Node * pParent = pTmp->m_pParent;

	if (!pParent)
	{
		//根结点
		if (pTmp->m_nKeyNum < 1)
		{
			m_pRoot = NULL;
			delete pTmp;
			m_pFirst = m_pLast = NULL;
		}
		return true;
	}

	for (i = 0; i <= pParent->m_nKeyNum; i++)
	{
		if (pTmp == pParent->m_Pointers[i])
		{
			break;
		}
	}

	Node * pNeighbor = NULL;
	int nNeighbor = -1;
	int nIndex = -1;

	if (i == 0)
	{
		pNeighbor = (Node *)pParent->m_Pointers[1];
		nNeighbor = 1;
		nIndex = 0;
	}
	else
	{
		pNeighbor = (Node *)pParent->m_Pointers[i - 1];
		nNeighbor = i - 1;
		nIndex = i;
	}

	if (pNeighbor->m_nKeyNum > nLowNum)
	{
		//借一个元素
		if (nNeighbor < nIndex)
		{
			pParent->m_Keys[nNeighbor] = pNeighbor->m_Keys[pNeighbor->m_nKeyNum - 1];
			for (i = pTmp->m_nKeyNum - 1; i >= 0; i--)
			{
				pTmp->m_Keys[i + 1] = pTmp->m_Keys[i];
				pTmp->m_Pointers[i + 1] = pTmp->m_Pointers[i];
			}
			pTmp->m_Keys[0] = pNeighbor->m_Keys[pNeighbor->m_nKeyNum - 1];
			pTmp->m_Pointers[0] = pNeighbor->m_Pointers[pNeighbor->m_nKeyNum - 1];
			pTmp->m_nKeyNum++;
			pNeighbor->m_nKeyNum--;
		}
		else
		{
			pParent->m_Keys[nIndex] = pNeighbor->m_Keys[1];
			pTmp->m_Keys[pTmp->m_nKeyNum] = pNeighbor->m_Keys[0];
			pTmp->m_Pointers[pTmp->m_nKeyNum] = pNeighbor->m_Pointers[0];
			pTmp->m_nKeyNum++;
			for (i = 1; i <= pNeighbor->m_nKeyNum - 1; i++)
			{
				pNeighbor->m_Keys[i - 1] = pNeighbor->m_Keys[i];
				pNeighbor->m_Pointers[i - 1] = pNeighbor->m_Pointers[i];
			}
			pNeighbor->m_nKeyNum--;
		}
		return true;
	}
	else
	{
		//合并邻居和自己
		if (nNeighbor < nIndex)
		{
			for (i = 0; i < pTmp->m_nKeyNum; i++)
			{
				pNeighbor->m_Keys[pNeighbor->m_nKeyNum] = pTmp->m_Keys[i];
				pNeighbor->m_Pointers[pNeighbor->m_nKeyNum] = pTmp->m_Pointers[i];
				pNeighbor->m_nKeyNum++;
			}

			for (i = nIndex; i < pParent->m_nKeyNum; i++)
			{
				pParent->m_Keys[i - 1] = pParent->m_Keys[i];
			}

			for (i = nIndex + 1; i <= pParent->m_nKeyNum; i++)
			{
				pParent->m_Pointers[i - 1] = pParent->m_Pointers[i];
			}

			pParent->m_nKeyNum--;

			if (pTmp->m_pPrev)
				pTmp->m_pPrev->m_pNext = pTmp->m_pNext;
			else
			{
				pTmp->m_pNext->m_pPrev = NULL;
				m_pFirst = pTmp->m_pNext;
			}

			if (pTmp->m_pNext)
				pTmp->m_pNext->m_pPrev = pTmp->m_pPrev;
			else
			{
				pTmp->m_pPrev->m_pNext = NULL;
				m_pLast = pTmp->m_pPrev;
			}

			delete pTmp;

		}
		else
		{
			for (i = 0; i < pNeighbor->m_nKeyNum; i++)
			{
				pTmp->m_Keys[pTmp->m_nKeyNum] = pNeighbor->m_Keys[i];
				pTmp->m_Pointers[pTmp->m_nKeyNum] = pNeighbor->m_Pointers[i];
				pTmp->m_nKeyNum++;
			}

			for (i = nNeighbor; i < pParent->m_nKeyNum; i++)
			{
				pParent->m_Keys[i - 1] = pParent->m_Keys[i];
			}

			for (i = nNeighbor + 1; i <= pParent->m_nKeyNum; i++)
			{
				pParent->m_Pointers[i - 1] = pParent->m_Pointers[i];
			}

			pParent->m_nKeyNum--;

			if (pNeighbor->m_pPrev)
				pNeighbor->m_pPrev->m_pNext = pNeighbor->m_pNext;
			else
			{
				pNeighbor->m_pNext->m_pPrev = NULL;
				m_pFirst = pNeighbor->m_pNext;
			}

			if (pNeighbor->m_pNext)
				pNeighbor->m_pNext->m_pPrev = pNeighbor->m_pPrev;
			else
			{
				pNeighbor->m_pPrev->m_pNext = NULL;
				m_pLast = pNeighbor->m_pPrev;
			}

			delete pNeighbor;
		}

		Node * pCurTmp = pParent;
		int nInternalLowNum = (ORDER + 1) / 2;

		//分支结点

		while (pCurTmp)
		{
			if (pCurTmp->m_nKeyNum >= nInternalLowNum)
			{
				break;
			}

			//借或者合并
			Node * pCurParent = pCurTmp->m_pParent;

			Node * pCurNeighbor = NULL;

			int nCurIndex = 0;

			int nNeighborIndex = 0;

			int nTmp = 0;

			if (!pCurParent)
			{
				//根结点
				if (pCurTmp->m_nKeyNum < 1)
				{
					((Node *)pCurTmp->m_Pointers[0])->m_pParent = NULL;
					m_pRoot = (Node *)pCurTmp->m_Pointers[0];
					delete pCurTmp;
				}
				break;
			}
			else
			{
				//非根结点
				for (i = 0; i <= pCurParent->m_nKeyNum; i++)
				{
					if (pCurTmp == pCurParent->m_Pointers[i])
					{
						break;
					}
				}

				if (i == 0)
				{
					pCurNeighbor = (Node *)pCurParent->m_Pointers[1];
					nCurIndex = 0;
					nNeighborIndex = 1;
				}
				else
				{
					pCurNeighbor = (Node *)pCurParent->m_Pointers[i-1];
					nCurIndex = i;
					nNeighborIndex = i-1;
				}

				if (pCurNeighbor->m_nKeyNum > nInternalLowNum)
				{
					//借
					if (nNeighborIndex < nCurIndex)
					{
						//左借
						nTmp = pCurParent->m_Keys[nNeighborIndex];
						pCurParent->m_Keys[nNeighborIndex] = pCurNeighbor->m_Keys[pCurNeighbor->m_nKeyNum - 1];
						for (i = pCurTmp->m_nKeyNum - 1; i >= 0; i--)
						{
							pCurTmp->m_Keys[i + 1] = pCurTmp->m_Keys[i];
						}
						for (i = pCurTmp->m_nKeyNum; i >= 0; i--)
						{
							pCurTmp->m_Pointers[i + 1] = pCurTmp->m_Pointers[i];
						}
						pCurTmp->m_Keys[0] = nTmp;
						if (pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum])
						{
							static_cast<Node *>(pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum])->m_pParent = pCurTmp;
						}
						pCurTmp->m_Pointers[0] = pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum];
						pCurTmp->m_nKeyNum++;
						pCurNeighbor->m_nKeyNum--;
					}
					else
					{
						//右借
						nTmp = pCurParent->m_Keys[nCurIndex];
						pCurParent->m_Keys[nCurIndex] = pCurNeighbor->m_Keys[0];
						pCurTmp->m_Keys[pCurTmp->m_nKeyNum] = nTmp;
						if (pCurNeighbor->m_Pointers[0])
						{
							static_cast<Node *>(pCurNeighbor->m_Pointers[0])->m_pParent = pCurTmp;
						}

						pCurTmp->m_Pointers[pCurTmp->m_nKeyNum + 1] = pCurNeighbor->m_Pointers[0];
						pCurTmp->m_nKeyNum++;
					
						for (i = 1; i < pCurNeighbor->m_nKeyNum; i++)
						{
							pCurNeighbor->m_Keys[i - 1] = pCurNeighbor->m_Keys[i];
						}
						for (i = 1; i <= pCurNeighbor->m_nKeyNum; i++)
						{
							pCurNeighbor->m_Pointers[i-1] = pCurNeighbor->m_Pointers[i];
						}

						pCurNeighbor->m_nKeyNum--;
					}
					break;
				}
				else
				{					
					if (nNeighborIndex < nCurIndex)
					{
						//左合并
						pCurNeighbor->m_Keys[pCurNeighbor->m_nKeyNum] = pCurParent->m_Keys[nNeighborIndex];
						pCurNeighbor->m_nKeyNum++;

						for (i = 0; i < pCurTmp->m_nKeyNum; i++)
						{
							pCurNeighbor->m_Keys[pCurNeighbor->m_nKeyNum] = pCurTmp->m_Keys[i];
							pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum] = pCurTmp->m_Pointers[i];
							Node * pChild = (Node *)pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum];
							if (pChild)
								pChild->m_pParent = pCurNeighbor;
							pCurNeighbor->m_nKeyNum++;
						}

						pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum] = pCurTmp->m_Pointers[i];

						if (pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum])
						{
							static_cast<Node *>(pCurNeighbor->m_Pointers[pCurNeighbor->m_nKeyNum])->m_pParent = pCurNeighbor;
						}

						for (i = nNeighborIndex + 1; i < pCurParent->m_nKeyNum; i++)
						{
							pCurParent->m_Keys[i - 1] = pCurParent->m_Keys[i];
						}

						for (i = nCurIndex + 1; i <= pCurParent->m_nKeyNum; i++)
						{
							pCurParent->m_Pointers[i - 1] = pCurParent->m_Pointers[i];
						}
						
						pCurParent->m_nKeyNum--;

						delete pCurTmp;
					}
					else
					{
						//右合并

						pCurTmp->m_Keys[pCurTmp->m_nKeyNum] = pCurParent->m_Keys[nCurIndex];
						pCurTmp->m_nKeyNum++;

						for (i = 0; i < pCurNeighbor->m_nKeyNum; i++)
						{
							pCurTmp->m_Keys[pCurTmp->m_nKeyNum] = pCurNeighbor->m_Keys[i];
							pCurTmp->m_Pointers[pCurTmp->m_nKeyNum] = pCurNeighbor->m_Pointers[i];
							Node * pChild = (Node *)(pCurTmp->m_Pointers[pCurTmp->m_nKeyNum]);
							if (pChild)
								pChild->m_pParent = pCurTmp;
							pCurTmp->m_nKeyNum++;
						}

						pCurTmp->m_Pointers[pCurTmp->m_nKeyNum] = pCurNeighbor->m_Pointers[i];
						if (pCurTmp->m_Pointers[pCurTmp->m_nKeyNum])
						{
							((Node *)(pCurTmp->m_Pointers[pCurTmp->m_nKeyNum]))->m_pParent = pCurTmp;
						}

						for (i = nCurIndex + 1; i < pCurParent->m_nKeyNum; i++)
						{
							pCurParent->m_Keys[i - 1] = pCurParent->m_Keys[i];
						}

						for (i = nNeighborIndex + 1; i <= pCurParent->m_nKeyNum; i++)
						{
							pCurParent->m_Pointers[i - 1] = pCurParent->m_Pointers[i];
						}

						pCurParent->m_nKeyNum--;

						delete pCurNeighbor;
					}

					pCurTmp = pCurParent;
				}
			}
		}

		return true;
	}
}

void BPlusTree::PrintLayerTree()
{
	int i = 0;

	typedef struct _tagPNode
	{
		void * m_pNode;
		int m_nLayer;
		_tagPNode()
		{
			m_pNode = NULL;
			m_nLayer = 0;
		}
	}PNode;

	queue<PNode> q;

	if (m_pRoot == NULL)
	{
		printf("b+tree is null.\n");
		return;
	}

	PNode node;
	node.m_nLayer = 1;
	node.m_pNode = m_pRoot;

	q.push(node);

	while (q.empty() == false)
	{
		PNode nodeTmp = q.front();

		printf("节点信息：\n");

		printf("节点层数：%d\n", nodeTmp.m_nLayer);

		Node * pCurNode = static_cast<Node *>(nodeTmp.m_pNode);

		if (!pCurNode)
		{
			printf("error!\n");
			break;
		}

		printf("关键字个数:%d\n", pCurNode->m_nKeyNum);

		printf("关键字:\n");

		if (pCurNode->m_bLeaf == false)
		{
			for (i = 0; i < pCurNode->m_nKeyNum; i++)
			{
				printf("internal key:%d", pCurNode->m_Keys[i]);
				printf(" ");
			}
		}
		else
		{
			LeafNode * pCurLeaf = NULL;

			for (i = 0; i < pCurNode->m_nKeyNum; i++)
			{
				pCurLeaf = static_cast<LeafNode *>(pCurNode->m_Pointers[i]);
				if (!pCurLeaf)
				{
					printf("error!");
					break;
				}
				printf("<%d,%s>", pCurLeaf->m_nKey, pCurLeaf->m_number);
				printf(" ");
			}
		}

		printf("\n");

		printf("-------------------------------------------------\n");

		if (static_cast<Node *>(nodeTmp.m_pNode)->m_bLeaf == false)
		{
			for (i = 0; i <= static_cast<Node *>(nodeTmp.m_pNode)->m_nKeyNum; i++)
			{
				if (static_cast<Node *>(nodeTmp.m_pNode)->m_Pointers[i])
				{
					PNode node2;
					node2.m_nLayer = nodeTmp.m_nLayer + 1;
					node2.m_pNode = static_cast<Node *>(nodeTmp.m_pNode)->m_Pointers[i];
					q.push(node2);
				}
			}
		}

		q.pop();
	}

	return;
}

void BPlusTree::DestroyTree()
{
	if (!m_pRoot)
		return;
	DestroyTree(m_pRoot);
}

void BPlusTree::DestroyTree(Node * & pRoot)
{
	int i = 0;

	if (!pRoot)
		return;
	if (pRoot->m_bLeaf)
	{
		for (i = 0; i < pRoot->m_nKeyNum; i++)
		{
			LeafNode * pTmp = (LeafNode *)pRoot->m_Pointers[i];
			delete pTmp;
		}
	}
	else
	{
		for (i = 0; i <= pRoot->m_nKeyNum; i++)
		{
			Node * pTmp = (Node *)pRoot->m_Pointers[i];
			DestroyTree(pTmp);
		}
	}

	delete pRoot;

	pRoot = NULL;

	return;
}
