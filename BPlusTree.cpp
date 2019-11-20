#include "BPlusTree.h"
#include <queue>
using namespace std;


BPlusTree::BPlusTree()
{
	m_pRoot = nullptr;
	m_pFirst = nullptr;
	m_pLast = nullptr;
}


BPlusTree::~BPlusTree()
= default;

bool BPlusTree::Insert(int nKey, double number)
{
	int i = 0;
	int m = 0;

	//�����Ϊ��
	if (m_pRoot==nullptr)
	{
	    //�½������
        auto * pNew = new LeafNode();
		//����ռ�ʧ�ܣ�����false
        pNew->m_bLeaf = true;
		pNew->m_nKeyNum = 1;
        pNew->m_pParent = nullptr;
		pNew->leafNodeData->m_Keys[0]=nKey;
        pNew->leafNodeData->m_number[0]=number;
        m_pRoot = pNew;
		m_pFirst = pNew;
		return true;
	}

	//�ҵ������Ҷ�ڵ�
	Node * pTmp = m_pRoot;

	//��ǰ��㲻��Ҷ��㣬����Ѱ��
	while (!pTmp->m_bLeaf)
	{
		for (i = 0; i < pTmp->m_nKeyNum; i++)
		{

			if (nKey <  ((InternalNode*)pTmp)->internalNodeData->m_Keys[i])
				break;
		}
		if (i == 0)
			pTmp = (Node *) (((InternalNode*)pTmp)->internalNodeData->m_Pointers[0]);
		else
			pTmp = (Node *)(((InternalNode*)pTmp)->internalNodeData->m_Pointers[i]);
	}

	//�ж�Ҷ�ڵ����Ƿ����
	for (i = 0; i < ((LeafNode *)pTmp)->m_nKeyNum; i++)
	{
	    //��Ҫ�����Ҷ�ӽ���Ѿ�����
		if (nKey == ((LeafNode *)pTmp)->leafNodeData->m_number[i])
		{
			return false;
		}
	}

	//�ж�Ҷ�ӽڵ������
	if (((LeafNode *)pTmp)->m_nKeyNum < LEAF_ORDER)
	{
		for (i = 0; i < ((LeafNode *)pTmp)->m_nKeyNum; i++)
		{
			if (nKey < ((LeafNode *)pTmp)->leafNodeData->m_number[i])
            {
                break;
            }
		}


		//�ƶ�
		for (m = ((LeafNode *)pTmp)->m_nKeyNum - 1; m >= i; m--)
		{
            ((LeafNode *)pTmp)->leafNodeData->m_Keys[m + 1] = ((LeafNode *)pTmp)->leafNodeData->m_Keys[m];
            ((LeafNode *)pTmp)->leafNodeData->m_number[m + 1] = ((LeafNode *)pTmp)->leafNodeData->m_number[m];
		}
        ((LeafNode *)pTmp)->leafNodeData->m_Keys[i] = nKey;
        ((LeafNode *)pTmp)->leafNodeData->m_number[i] = number;
        double x1 = ((LeafNode *)pTmp)->leafNodeData->m_number[i];
        ((LeafNode *)pTmp)->m_nKeyNum++;
		return true;
	}

	//Ҷ�ӽڵ��Ѿ�����
	int nMid = (LEAF_ORDER + 1) / 2;

	int * pTmpKeys = new int[LEAF_ORDER + 1];
	auto * pTmpnumbers = new double[LEAF_ORDER + 1];


	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
		if (((LeafNode *)pTmp)->leafNodeData->m_number[i] > nKey)
			break;
	}
    //Ҷ�ӽ������²����ֵ
	for (m = pTmp->m_nKeyNum - 1; m >= i; m--)
	{
		pTmpKeys[m + 1] = ((LeafNode *)pTmp)->leafNodeData->m_Keys[m];
        pTmpnumbers[m + 1] = ((LeafNode *)pTmp)->leafNodeData->m_number[m];
	}

	for (m = 0; m < i; m++)
	{
		pTmpKeys[m] = ((LeafNode *)pTmp)->leafNodeData->m_Keys[m];
        pTmpnumbers[m] = ((LeafNode *)pTmp)->leafNodeData->m_number[m];
        double x = ((LeafNode *)pTmp)->leafNodeData->m_number[m];
        double y = pTmpnumbers[m];
	}
	pTmpKeys[i] = nKey;
    pTmpnumbers[i] = number;


    //�½�Ҷ�ӽ�㣬���ڷ���
    auto * pNew = new LeafNode;

	for (m = 0, i = nMid; i < LEAF_ORDER + 1; i++,m++)
	{
		pNew->leafNodeData->m_Keys[m] =pTmpKeys[i];
		pNew->leafNodeData->m_number[m] = pTmpnumbers[i];
		pNew->m_nKeyNum++;
	}
    pNew->m_pParent = pTmp->m_pParent;

	pNew->m_bLeaf = pTmp->m_bLeaf;

	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
        ((LeafNode*)pTmp)->leafNodeData->m_Keys[i] = 0;
        ((LeafNode*)pTmp)->leafNodeData->m_number[i] = 0.0;
	}

	pTmp->m_nKeyNum = 0;

	for (i = 0; i < nMid; i++)
	{
        ((LeafNode*)pTmp)->leafNodeData->m_Keys[i] = pTmpKeys[i];
        ((LeafNode*)pTmp)->leafNodeData->m_number[i] = pTmpnumbers[i];
		pTmp->m_nKeyNum++;
	}
	pNew->leafNodeData->m_next = ((LeafNode*)pTmp)->leafNodeData->m_next;
    ((LeafNode*)pTmp)->leafNodeData->m_next= pNew;


	if (!InsertKeyAndPointer(pTmp->m_pParent, pTmp, pTmpKeys[nMid], pNew))
	{
		return false;
	}
	return true;
}

void BPlusTree::PrintLeaves()
{
	int i = 0;
	if (!m_pFirst)
		return;
	LeafNode * pCur =(LeafNode*)m_pFirst;
	printf("------print leave-------\n");
	while(1){
        for(int i=0;i<pCur->m_nKeyNum;i++){
            printf("<key:%d, number:%lf>\n", pCur->leafNodeData->m_Keys[i], pCur->leafNodeData->m_number[i]);
        }
        if(pCur->leafNodeData->m_next==NULL){
            break;
        }else{
            pCur = static_cast<LeafNode *>(pCur->leafNodeData->m_next);
        }
        printf("\n------------------------------------------------------\n");
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

	//����㲻���ڣ����Ǹ����ĸ���
	if (pParent == nullptr)
	{
		//�����¸�
		auto * pNewRoot = new InternalNode;
		//û������ɹ�
        pNewRoot->m_bLeaf = false;
		pNewRoot->internalNodeData->m_Keys[0] = nKey;
		pNewRoot->m_nKeyNum = 1;
        pNewRoot->internalNodeData->m_Pointers[0] = pOld;
        pNewRoot->internalNodeData->m_Pointers[1] = pNew;
		pNewRoot->m_pParent = nullptr;
		pOld->m_pParent = pNewRoot;
        pNew->m_pParent = pNewRoot;
		m_pRoot = pNewRoot;
		return true;
	}

	//Ųһ��
	for (i = 0; i < pParent->m_nKeyNum; i++)
	{
	    //�������ظ������ͬ��ֵ
		if (nKey == ((InternalNode*)pParent)->internalNodeData->m_Keys[i])
			return false;
		if (nKey < ((InternalNode*)pParent)->internalNodeData->m_Keys[i])
			break;
	}

	//�жϸ�����Ƿ��Ѿ�����
	if (pParent->m_nKeyNum < ORDER)
	{
		//�ƶ��ؼ���
		for (m = pParent->m_nKeyNum - 1; m >= i; m--)
		{
            ((InternalNode*)pParent)->internalNodeData->m_Keys[m + 1] = ((InternalNode*)pParent)->internalNodeData->m_Keys[m];
		}

		//�ƶ�����
		for (m = pParent->m_nKeyNum; m > i; m--)
		{
            ((InternalNode*)pParent)->internalNodeData->m_Pointers[m + 1] = ((InternalNode*)pParent)->internalNodeData->m_Pointers[m];
		}

		//���ȥ
        ((InternalNode*)pParent)->internalNodeData->m_Keys[i] = nKey;
        ((InternalNode*)pParent)->internalNodeData->m_Pointers[i + 1] = pNew;
		pParent->m_nKeyNum++;

		pNew->m_pParent = pParent;
		return true;
	}

	//���׽���Ѿ�����
	int * pTmpKeys = new int[ORDER + 1];

	void ** pTmpPointers = new void *[ORDER + 2];

	//�ҵ�����ĵص�
	for (i = 0; i < pParent->m_nKeyNum; i++)
	{
		if (nKey < ((InternalNode*)pParent)->internalNodeData->m_Keys[i])
			break;
	}

	//������
	for (m = 0; m < i; m++)
	{
		pTmpKeys[m] = ((InternalNode*)pParent)->internalNodeData->m_Keys[m];
	}

	pTmpKeys[m] = nKey;
	m++;

	for (k = i; k < pParent->m_nKeyNum; k++,m++)
	{
		pTmpKeys[m] = ((InternalNode*)pParent)->internalNodeData->m_Keys[k];
	}

	//���Ӳ���
	for (m = 0; m <= i; m++)
	{
		pTmpPointers[m] = ((InternalNode*)pParent)->internalNodeData->m_Pointers[m];
	}

	pTmpPointers[m] = pNew;
	m++;

	for (k = i + 1; k <= pParent->m_nKeyNum; k++,m++)
	{
		pTmpPointers[m] = ((InternalNode*)pParent)->internalNodeData->m_Pointers[k];
	}

	//���׷��ѣ����ѳ������½���ʼ��
	Node * pNewSplit = new InternalNode;

	//�����ǲ���Ҷ�ӽ��
	pNewSplit->m_bLeaf = pParent->m_bLeaf;

	//�·��ѳ����Ľ��
	pNewSplit->m_pParent = pParent->m_pParent;

	//����ֿ�
	int nMid = (ORDER + 1) / 2;

	//�����ᵽ�ְ�����
	int nMidKey = pTmpKeys[nMid];

	//�ұߵĽ��
	for (m = 0, i = nMid + 1; i < ORDER + 1; i++, m++)
	{
        ((InternalNode*)pNewSplit)->internalNodeData->m_Keys[m] = pTmpKeys[i];
        ((InternalNode*)pNewSplit)->internalNodeData->m_Pointers[m] = pTmpPointers[i];
		if (!pNewSplit->m_bLeaf)
		{
			Node * pCur = static_cast<Node *>(((InternalNode*)pNewSplit)->internalNodeData->m_Pointers[m]);
			if (pCur)
			{
				pCur->m_pParent = pNewSplit;
			}
		}
		pNewSplit->m_nKeyNum++;
	}

    ((InternalNode*)pNewSplit)->internalNodeData->m_Pointers[m] = pTmpPointers[i];

	if (!pNewSplit->m_bLeaf)
	{
		Node * pCur = static_cast<Node *>(((InternalNode*)pNewSplit)->internalNodeData->m_Pointers[m]);
		if (pCur)
		{
			pCur->m_pParent = pNewSplit;
		}
	}

	pParent->m_nKeyNum = 0;


	//������
	for (i = 0; i < nMid; i++)
	{
        ((InternalNode*)pParent)->internalNodeData->m_Keys[i] = pTmpKeys[i];
        ((InternalNode*)pParent)->m_nKeyNum++;
	}

	for (i = 0; i <= nMid; i++)
	{
        ((InternalNode*)pParent)->internalNodeData->m_Pointers[i] = pTmpPointers[i];
	}

    delete[] pTmpKeys;

    delete[] pTmpPointers;

	return InsertKeyAndPointer(pParent->m_pParent, pParent, nMidKey, pNewSplit);
}


//����ĳ��ֵ�Ƿ����
bool BPlusTree::search(int nKey){
    //����ֱ���ҵ�Ҷ�ӽ��

    int i=0;

    //����û�н��
    if (!m_pRoot)
        return false;

    Node * pTmp = m_pRoot;
    while (!pTmp->m_bLeaf)
    {
        for (i = 0; i < pTmp->m_nKeyNum; i++)
        {
            if (nKey < ((InternalNode*)pTmp)->internalNodeData->m_Keys[i])
            {
                break;
            }
        }

        pTmp = (Node*)(((InternalNode*)pTmp)->internalNodeData->m_Pointers[i]);
    }

    auto* leafNode = (LeafNode *)pTmp;
    for(int i=0;i<leafNode->m_nKeyNum;i++){
        if(nKey == leafNode->leafNodeData->m_Keys[i]){
            return true;
        }
    }
    return false;
}


void BPlusTree::PrintLayerTree()
{
    int i = 0;

    queue<Node *> q;

    if (m_pRoot == NULL)
    {
        printf("b+tree is null.\n");
        return;
    }

    Node* node;

    node = m_pRoot;
    q.push(node);

    while (q.empty() == false)
    {
        Node * nodeTmp = q.front();

        if (nodeTmp->m_bLeaf == false)
        {
            for (i = 0; i < nodeTmp->m_nKeyNum; i++)
            {
                printf("%d",((InternalNode*)nodeTmp) ->internalNodeData->m_Keys[i]);
                q.push((Node*)((InternalNode*)nodeTmp) ->internalNodeData->m_Pointers[i]);
                printf(" ");
            }
            q.push((Node*)((InternalNode*)nodeTmp) ->internalNodeData->m_Pointers[i]);
        }
        else
        {
            for (i = 0; i < nodeTmp->m_nKeyNum; i++)
            {
                printf("%d", ((LeafNode*)nodeTmp)->leafNodeData->m_Keys[i]);
                printf(" ");
            }
        }

        printf("\n");

        printf("-------------------------------------------------\n");

        q.pop();
    }

    return;
}

bool BPlusTree::Remove(int nKey)
{
    //����û�н��
	if (!m_pRoot)
		return false;

	int i = 0;

	int j=-1;

	int m = 0;

	Node * pTmp = m_pRoot;

	//����ֱ���ҵ�Ҷ�ӽ��
	while (!pTmp->m_bLeaf)
	{
	    bool flag=false;
		for (i = 0; i < pTmp->m_nKeyNum; i++)
		{
            if (nKey < ((InternalNode*)pTmp)->internalNodeData->m_Keys[i])
            {
                flag = true;
                //j��ʾ��ǰ������ڸ�����е�λ��
                j=i;
                break;
            }
		}
		if(flag==false){
		    j = pTmp->m_nKeyNum;
		}

        pTmp = (Node*)(((InternalNode*)pTmp)->internalNodeData->m_Pointers[i]);
	}

    //�ҵ�Ӧ�ñ�ɾ���Ľ��
	for (i = 0; i < pTmp->m_nKeyNum; i++)
	{
		if (nKey == ((LeafNode*)pTmp)->leafNodeData->m_Keys[i]) {
            break;
        }
	}

	//û���ҵ�Ӧ�ñ�ɾ���Ľ��
	if (i == pTmp->m_nKeyNum) {
	    printf("Wrong! %d node not exists",nKey);
        return false;
    }

	auto * pCur = (LeafNode *)pTmp;


	//ʣ�µĽ����ǰŲ������ɾ���Ľ��
	for (m = i + 1; m < pTmp->m_nKeyNum; m++)
	{
        pCur->leafNodeData->m_Keys[m-1] = pCur->leafNodeData->m_Keys[m];
        pCur->leafNodeData->m_number[m-1] = pCur->leafNodeData->m_number[m];
	}

	pTmp->m_nKeyNum--;

	//���ڵ����Ŀ����С���ֵʱ����Ӧ�ý���֮��Ĳ�����
	int nLowNum = (LEAF_ORDER+1)  / 2;

	if (pTmp->m_nKeyNum >= nLowNum)
	{
		return true;
	}

	//�����

	Node * pParent = pTmp->m_pParent;

	//�����ǿյģ�˵����ǰҶ�ӽ����Ǹ��ڵ�
	if (!pParent) {
        //����㣬ɾ����
        if (pTmp->m_nKeyNum < 1) {
            m_pRoot = NULL;
            delete pTmp;
            m_pFirst = m_pLast = NULL;
        }
        return true;
    }

	Node * pNeighbor = NULL;
	int nNeighbor = -1;
	int nIndex = -1;

	//��ǰ���λ��Ϊͷ�ڵ㣬�ھӽ��Ϊ1�Ž��
	if (j == 0)
	{
		pNeighbor = (Node*)(((InternalNode *)pParent)->internalNodeData->m_Pointers[1]);
		nNeighbor = 1;
		nIndex = 0;
	}
	else
	{
        pNeighbor = (Node*)(((InternalNode *)pParent)->internalNodeData->m_Pointers[j-1]);
		nNeighbor = j - 1;
		nIndex = j;
	}

    //���ֵܽ��key�и��ࣨ����Math.ceil(m-1)/2 �C 1�������ֵܽ���һ����¼��ͬʱ�ý赽��key�滻���ᣨָ��ǰ�����ֵܽ�㹲ͬ�ĸ���㣩���е�key
	if (pNeighbor->m_nKeyNum > nLowNum)
	{
		//�����ھӽ�һ�����
		if (nNeighbor < nIndex)
		{
		    //���ھӽ��������Ľ�㣬���������׽��
            ((InternalNode*)pParent)->internalNodeData->m_Keys[nNeighbor] = ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[pNeighbor->m_nKeyNum - 1];
            //�ý��Ϊ�ӹ����Ľ��Ų�ط�
			for (i = pTmp->m_nKeyNum - 1; i >= 0; i--)
			{
                ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[i + 1] = ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[i];
                ((LeafNode*)pNeighbor)->leafNodeData->m_number[i + 1] = ((LeafNode*)pNeighbor)->leafNodeData->m_number[i];
			}
            ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[i + 1] = ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[pNeighbor->m_nKeyNum - 1];
            ((LeafNode*)pNeighbor)->leafNodeData->m_number[0] = ((LeafNode*)pNeighbor)->leafNodeData->m_number[pNeighbor->m_nKeyNum - 1];
			pTmp->m_nKeyNum++;
			pNeighbor->m_nKeyNum--;
		}
		//�����ھӽ�һ�����
		else
		{
            ((InternalNode*)pParent)->internalNodeData->m_Keys[nIndex] = ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[1];
            ((LeafNode*)pTmp)->leafNodeData->m_Keys[pTmp->m_nKeyNum] = ((LeafNode*)pTmp)->leafNodeData->m_Keys[0];
            ((LeafNode*)pTmp)->leafNodeData->m_number[pTmp->m_nKeyNum] = ((LeafNode*)pTmp)->leafNodeData->m_number[0];
			pTmp->m_nKeyNum++;
			for (i = 1; i <= pNeighbor->m_nKeyNum - 1; i++)
			{
                ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[i - 1] = ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[i];
                ((LeafNode*)pNeighbor)->leafNodeData->m_number[i - 1] = ((LeafNode*)pNeighbor)->leafNodeData->m_number[i];
			}
			pNeighbor->m_nKeyNum--;
		}
		return true;
	}

	//3�����ֵܽ����û�и����key,��ǰ�����ֵܽ��ϲ���һ���µ�Ҷ�ӽ�㣬��ɾ��������е�key��������е����key���ߵĺ���ָ��ͱ����һ��ָ�룬����ָ������µ�Ҷ�ӽ�㣩��
	// ����ǰ���ָ�򸸽�㣨��Ϊ������㣩��
	else
	{
		//�ϲ����ھӺ��Լ�
		if (nNeighbor < nIndex)
            {
			for (i = 0; i < pTmp->m_nKeyNum; i++)
			{
                ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[pNeighbor->m_nKeyNum] = ((LeafNode*)pTmp)->leafNodeData->m_Keys[i];
                ((LeafNode*)pNeighbor)->leafNodeData->m_number[pNeighbor->m_nKeyNum] = ((LeafNode*)pTmp)->leafNodeData->m_number[i];
				pNeighbor->m_nKeyNum++;
			}

			//ɾ�����ڵ��е�һ����
			for (i = nIndex; i < pParent->m_nKeyNum; i++)
			{
                ((InternalNode*)pParent)->internalNodeData->m_Keys[i - 1] = ((InternalNode*)pParent)->internalNodeData->m_Keys[i];
			}

			for (i = nIndex + 1; i <= pParent->m_nKeyNum; i++)
			{
                ((InternalNode*)pParent)->internalNodeData->m_Pointers[i - 1] = ((InternalNode*)pParent)->internalNodeData->m_Pointers[i];
			}

			pParent->m_nKeyNum--;

            ((LeafNode*)pNeighbor)->leafNodeData->m_next = ((LeafNode*)pTmp)->leafNodeData->m_next;

			delete pTmp;

		}
		//�ϲ����ھӺ��Լ�
		else
		{
			for (i = 0; i < pNeighbor->m_nKeyNum; i++)
			{
                ((LeafNode*)pTmp)->leafNodeData->m_Keys[pTmp->m_nKeyNum] = ((LeafNode*)pNeighbor)->leafNodeData->m_Keys[i];
                ((LeafNode*)pTmp)->leafNodeData->m_number[pTmp->m_nKeyNum] = ((LeafNode*)pNeighbor)->leafNodeData->m_number[i];
				pTmp->m_nKeyNum++;
			}

			for (i = nNeighbor; i < pParent->m_nKeyNum; i++)
			{
                ((InternalNode*)pParent)->internalNodeData->m_Keys[i - 1] = ((InternalNode*)pParent)->internalNodeData->m_Keys[i];
			}
			for (i = nNeighbor + 1; i <= pParent->m_nKeyNum; i++)
			{
                ((InternalNode*)pParent)->internalNodeData->m_Pointers[i - 1] = ((InternalNode*)pParent)->internalNodeData->m_Pointers[i];
			}

			pParent->m_nKeyNum--;
            ((LeafNode*)pTmp)->leafNodeData->m_next = ((LeafNode*)pNeighbor)->leafNodeData->m_next;
			delete pNeighbor;
		}

		//��ǰ�ڵ�ָ�򸸽ڵ�
		Node * pCurTmp = pParent;
		int nInternalLowNum = (ORDER + 1) / 2;

		//��֧����ظ�����
		while (pCurTmp)
		{
		    //����������key�ĸ������ڵ���Math.ceil(m-1)/2 �C 1����ɾ������������
			if (pCurTmp->m_nKeyNum >= nInternalLowNum)
			{
				break;
			}

			//����ߺϲ�
			Node * pCurParent = pCurTmp->m_pParent;

			Node * pCurNeighbor = NULL;

			int nCurIndex = 0;

			int nNeighborIndex = 0;

			int nTmp = 0;

			//��ǰ�ڵ�Ϊ���ڵ�
			if (!pCurParent)
			{
				//�����
				if (pCurTmp->m_nKeyNum < 1)
				{
                    ((Node*)(((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[0]))->m_pParent= NULL;
					m_pRoot = (Node *)(((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[0]);
					delete pCurTmp;
				}
				break;
			}
			else
			{
				//�Ǹ����
				//�ҵ���ǰ����λ��
				for (i = 0; i <= pCurParent->m_nKeyNum; i++)
				{
					if (pCurTmp == ((InternalNode*)pCurParent)->internalNodeData->m_Pointers[i])
					{
						break;
					}
				}

				if (i == 0)
				{
					pCurNeighbor = (Node *)((InternalNode*)pCurParent)->internalNodeData->m_Pointers[1];
					nCurIndex = 0;
					nNeighborIndex = 1;
				}
				else
				{
					pCurNeighbor = (Node *)((InternalNode*)pCurParent)->internalNodeData->m_Pointers[i-1];
					nCurIndex = i;
					nNeighborIndex = i-1;
				}

				//�ֵܽ����������
				if (pCurNeighbor->m_nKeyNum > nInternalLowNum)
				{
					//��
					if (nNeighborIndex < nCurIndex)
					{
						//���
						nTmp = ((InternalNode*)pCurParent)->internalNodeData->m_Keys[nNeighborIndex];
						//���׽���ֵ����
                        ((InternalNode*)pCurParent)->internalNodeData->m_Keys[nNeighborIndex] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[pCurNeighbor->m_nKeyNum - 1];
						for (i = pCurTmp->m_nKeyNum - 1; i >= 0; i--)
						{
                            ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[i + 1] = ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[i];
						}
						for (i = pCurTmp->m_nKeyNum; i >= 0; i--)
						{
						    ((InternalNode *) pCurTmp)->internalNodeData->m_Pointers[i + 1] = ((InternalNode *) pCurTmp)->internalNodeData->m_Pointers[i];
						}
                        ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[0] = nTmp;
						if (((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum])
						{

                            static_cast<Node *>(((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum])->m_pParent = pCurTmp;
						}
                        ((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[0] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum];
						pCurTmp->m_nKeyNum++;
						pCurNeighbor->m_nKeyNum--;
					}
					else
					{
						//�ҽ�
						nTmp =  ((InternalNode*)pCurParent)->internalNodeData->m_Keys[nCurIndex];
                        ((InternalNode*)pCurParent)->internalNodeData->m_Keys[nCurIndex] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[0];
                        ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[pCurTmp->m_nKeyNum] = nTmp;

                        //�����㲻�Ǹ��ս��
						if (((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[0])
						{
							static_cast<Node *>(((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[0])->m_pParent = pCurTmp;
						}

                        ((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[pCurTmp->m_nKeyNum + 1] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[0];
						pCurTmp->m_nKeyNum++;

						for (i = 1; i < pCurNeighbor->m_nKeyNum; i++)
						{
                            ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[i - 1] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[i];
						}
						for (i = 1; i <= pCurNeighbor->m_nKeyNum; i++)
						{
                            ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[i-1] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[i];
						}

						pCurNeighbor->m_nKeyNum--;
					}
					break;
				}
				//�ֵܽ��û�������裬��ǰ�����ֵܽ�㼰���������key�ϲ���һ���µĽ�㡣����ǰ���ָ�򸸽�㣬�ظ���4����
				else
				{
				    //���費��
					if (nNeighborIndex < nCurIndex)
					{
						//��ϲ�

						//�ϲ���һ���µĽ��
                        ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[pCurNeighbor->m_nKeyNum] = ((InternalNode*)pCurParent)->internalNodeData->m_Keys[nNeighborIndex];
						pCurNeighbor->m_nKeyNum++;
						for (i = 0; i < pCurTmp->m_nKeyNum; i++)
						{
                            ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[pCurNeighbor->m_nKeyNum] = ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[i];
                            ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum] = ((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[i];
							Node * pChild = (Node *)((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum];
							if (pChild)
								pChild->m_pParent = pCurNeighbor;
							pCurNeighbor->m_nKeyNum++;
						}

                        ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum] = ((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[i];

						if ( ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum])
						{
							static_cast<Node *>( ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[pCurNeighbor->m_nKeyNum])->m_pParent = pCurNeighbor;
						}

						//��������
						for (i = nNeighborIndex + 1; i < pCurParent->m_nKeyNum; i++)
						{
                            ((InternalNode*)pCurParent)->internalNodeData->m_Keys[i - 1] = ((InternalNode*)pCurParent)->internalNodeData->m_Keys[i];
						}

						for (i = nCurIndex + 1; i <= pCurParent->m_nKeyNum; i++)
						{
                            ((InternalNode*)pCurParent)->internalNodeData->m_Pointers[i - 1] = ((InternalNode*)pCurParent)->internalNodeData->m_Pointers[i];
						}

						pCurParent->m_nKeyNum--;

						delete pCurTmp;
					}
					//�ҽ�費��
					else
					{
						//�Һϲ�
						//�γɺϲ��ڵ�
                        ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[pCurTmp->m_nKeyNum] = ((InternalNode*)pCurParent)->internalNodeData->m_Keys[nCurIndex];
						pCurTmp->m_nKeyNum++;

						for (i = 0; i < pCurNeighbor->m_nKeyNum; i++)
						{
                            ((InternalNode*)pCurTmp)->internalNodeData->m_Keys[pCurTmp->m_nKeyNum] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Keys[i];
                            ((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[pCurTmp->m_nKeyNum] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[i];
							Node * pChild = (Node *)(((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[pCurTmp->m_nKeyNum]);
							if (pChild)
								pChild->m_pParent = pCurTmp;
							pCurTmp->m_nKeyNum++;
						}

                        ((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[pCurTmp->m_nKeyNum] = ((InternalNode*)pCurNeighbor)->internalNodeData->m_Pointers[i];
						if (((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[pCurTmp->m_nKeyNum])
						{
							((Node *)(((InternalNode*)pCurTmp)->internalNodeData->m_Pointers[pCurTmp->m_nKeyNum]))->m_pParent = pCurTmp;
						}

						//��������
						for (i = nCurIndex + 1; i < pCurParent->m_nKeyNum; i++)
						{
							((InternalNode*)pCurParent)->internalNodeData->m_Keys[i - 1] = ((InternalNode*)pCurParent)->internalNodeData->m_Keys[i];
						}

						for (i = nNeighborIndex + 1; i <= pCurParent->m_nKeyNum; i++)
						{
							((InternalNode*)pCurParent)->internalNodeData->m_Pointers[i - 1] = ((InternalNode*)pCurParent)->internalNodeData->m_Pointers[i];
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
