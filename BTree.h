#include<iostream>

using namespace std;



template<class K, size_t M>//M����M����
struct BTreeNode
{
	//K _keys[M-1];    // �ؼ��ּ���
	// �ؼ��ּ��ϸ���M���򻯷��ѵ��߼�
	K _keys[M];                  // �ؼ��ּ���
	//BTreeNode<K, M> *_pSub[M];   // ���ӵļ���
	BTreeNode<K, M> *_pSub[M+1];   // ���ӵļ���
	BTreeNode<K, M> *_pParent;   // ˫��
	size_t _size;     // ��ЧԪ�صĸ���

	BTreeNode()
		: _size(0)
		, _pParent(NULL)
	{
		for (size_t idx = 0; idx < M; ++idx)
			_pSub[idx] = NULL;
	}
};


template<class K, size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;

public:
	BTree()
		: _pRoot(NULL)
	{}

	std::pair<Node*, int> Find(const K& key)
	{
		Node* pCur = _pRoot;
		Node* pParent = NULL;
		//for(int i=0; i<M; ++i)
		//{
	
	///	}

		//int size = pCur->_size;
 		while(/*size&&*/pCur)
		{
			size_t index =0;
			while(index  < pCur->_size)
			{
				if(key < pCur->_keys[index])
				{
					break;
				}
				else if(key > pCur->_keys[index])
				{
					index++;
				}
				else
				{
					 return pair<Node*,int>(pCur,index);
				}
			}
			pParent = pCur;
		//	--size;
		//	if(size==0)
		//	{
				pCur = pCur->_pSub[index];
		//		if(pCur != NULL)
		//			size = pCur->_size;
		//	}
		}
		return pair<Node*,int>(pParent,-1);
	}
	bool Insert(const K& key)
	{
		//��Ϊ�գ���ֱ�Ӳ���
		if(_pRoot == NULL)
		{
			_pRoot = new Node();
			_pRoot->_keys[0] = key;
			_pRoot->_size = 1;
			return true;
		}

		//�ҵ������λ��
		
		 pair<Node*,int> ret = Find(key);
         if(ret.second > -1)//��ʾkey�Ѿ�����
            return false;

        Node* pCur = ret.first;
    
		Node* pSub = NULL;
		while(true)
		{
			//��֤key����ڵ���
			_InsertKey(pCur,key,pSub);//����key
			if(pCur->_size >= M)//����ؼ��ָ�����������Ԫ��������������з���
			{
				//����
				int mid = pCur->_size/2;
				K k = pCur->_keys[mid];

				Node* temp = new Node();

				//�ȿ����ؼ���
				size_t i=0;
				for(; i+mid < pCur->_size-1; ++i)
				{
					temp->_keys[i] = pCur->_keys[mid+i+1];
					temp->_size++;
				}

				//�ٿ�������ָ����
				i=0;
				for(; i+mid<pCur->_size; ++i)
				{
					temp->_pSub[i] = pCur->_pSub[mid+i+1];
				}
				pCur->_size/=2;
				//pCur = pCur->_pParent;
				pSub = temp;
				if(pCur->_pParent == NULL)
				{
					temp = new Node();
					temp->_keys[0] = k;
					temp->_pSub[0] = _pRoot ;
					//_pRoot->_pSub[1] = pSub;
					temp->_pSub[1] = pSub;
					temp->_size++;
					pSub->_pParent = temp;
					_pRoot->_pParent = temp;
					_pRoot = temp;
					return true;
				}
				else
				{
					pCur = pCur->_pParent;
					pCur->_keys[1] = key;
				//	pCur->_pSub[mid] = _pRoot;
					pCur->_pSub[mid+1] = pSub; 
					pCur->_size++;
				}
			}
			break;
		}
	}
	void InOrder()//������������������Ƿ���ȷ
	{
		_InOrder(_pRoot);
	}

private:
	void _InsertKey(Node* pCur, const K& key, Node* pSub)
	{
		//����key�����ƺ���
		int index = pCur->_size-1;
		while(index>=0 && key < pCur->_keys[index])
		{
			pCur->_keys[index+1] = pCur->_keys[index];
			pCur->_pSub[index+2] = pCur->_pSub[index+1];
			index--;
		}
		pCur->_keys[index+1] = key;
		pCur->_pSub[index+2] = pSub;
	   if(pSub)
	   {
		   pSub->_pParent = pCur;
	   }
		pCur->_size++;
	}
	/*void _InOrder(Node* pRoot)
	{
		if(pRoot)
		{
			int i=0;
			for(; i < _pRoot->_size; ++i)
			{
				_InOrder(pRoot->_pSub[i]);
				cout<<pRoot->_keys<<" ";
			}
			_InOrder(pRoot->_pSub[i]);
		} 
	}*/

private:
	Node* _pRoot;
};


void Test()
{
	int a[] = {12,23,45,2,32,35,67,43};
	BTree<int,3> n;
 	for(int i=0; i < sizeof(a)/sizeof(a[0]); ++i)
	{
		n.Insert(a[i]);
	}
	//n.InOrder();
}