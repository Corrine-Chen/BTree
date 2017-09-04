#include<iostream>

using namespace std;



template<class K, size_t M>//M代表M叉树
struct BTreeNode
{
	//K _keys[M-1];    // 关键字集合
	// 关键字集合给成M：简化分裂的逻辑
	K _keys[M];                  // 关键字集合
	//BTreeNode<K, M> *_pSub[M];   // 孩子的集合
	BTreeNode<K, M> *_pSub[M+1];   // 孩子的集合
	BTreeNode<K, M> *_pParent;   // 双亲
	size_t _size;     // 有效元素的个数

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
		//数为空，则直接插入
		if(_pRoot == NULL)
		{
			_pRoot = new Node();
			_pRoot->_keys[0] = key;
			_pRoot->_size = 1;
			return true;
		}

		//找到插入的位置
		
		 pair<Node*,int> ret = Find(key);
         if(ret.second > -1)//表示key已经存在
            return false;

        Node* pCur = ret.first;
    
		Node* pSub = NULL;
		while(true)
		{
			//保证key插入节点中
			_InsertKey(pCur,key,pSub);//插入key
			if(pCur->_size >= M)//插入关键字个数超过数组元素最大个数，则进行分裂
			{
				//分裂
				int mid = pCur->_size/2;
				K k = pCur->_keys[mid];

				Node* temp = new Node();

				//先拷贝关键字
				size_t i=0;
				for(; i+mid < pCur->_size-1; ++i)
				{
					temp->_keys[i] = pCur->_keys[mid+i+1];
					temp->_size++;
				}

				//再拷贝孩子指针域
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
	void InOrder()//用来检测所建立的树是否正确
	{
		_InOrder(_pRoot);
	}

private:
	void _InsertKey(Node* pCur, const K& key, Node* pSub)
	{
		//搬移key、搬移孩子
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