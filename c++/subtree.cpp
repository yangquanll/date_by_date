#include <iostream>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

typedef char ElemType;

typedef struct Treenode {
	ElemType data;
	struct Treenode *lchild,*rchild;
	Treenode(int val)
	{
		data = val;
		lchild =NULL;
		rchild =NULL;

	}
}Tnptr;


class Subtree
{
	public:

		Subtree(){

			size =0;
			m_root =NULL;
		}
		~Subtree(){

		}
		void creat(Tnptr* &btr);
		//void destroy();
		int getsize();
		int getheight();
		int sumnode();

	private:
		int size;
		Tnptr *m_root;
//		Tnptr creat();
		int getheight(Tnptr* &btr);
		//int destroy(Tnptr* &btr);
		int sumnode(Tnptr* &btr);
};

void Subtree :: creat(Tnptr* &btr)
{
	ElemType va;
	static int count = 0;
	cin >> va;

	if(va == '#')
	{
		btr =NULL;
	}else
	{
		btr =new Tnptr(count);// 可以在struct里面这样用
		Subtree::size++; //只用 size++ 无效
		btr->data = va;
		creat(btr->lchild);
		creat(btr->rchild);

	}
	count++;
	if(count >5)
	{
			return;
	}
	cout<< "cnt = "<<count<<endl;
	cout<<"size = "<<size<<endl;
	Subtree::m_root =btr;
	//memcpy(Subtree::m_root,btr,sizeof(btr)); //不能成功
}
/*
void Subtree::creat()
{
	size =0;
	m_root = creat();
}
*/
/*
int  Subtree :: destroy(Tnptr &root)
{
	if(root == NULL)
	{
		return NULL;
	}else{
		destroy(root ->lchild);
		destroy(root ->rchild)
		delete root;
		root = NULL;
		size = 0;
	}
}
*/
int Subtree::getsize()
{
	return size;
}

int Subtree ::getheight(Tnptr* &root)
{
	if(root == NULL)
		return 0;
	int left_l =getheight(root->lchild);
	int right_l =getheight(root->rchild);

	cout<<"left_l ="<<left_l<<"right_l = "<<right_l<<endl;
	return (left_l>right_l)? (left_l+1):(right_l+1);
}

int Subtree::getheight()
{
	return getheight(m_root);
}

int Subtree::sumnode(Tnptr* &root)
{
	if(root ==NULL)
	{
		return 0;
	}
	else
	{
		return sumnode(root->lchild)+sumnode(root->rchild)+1;
	}

}

int Subtree::sumnode()
{
	return	sumnode(m_root);
}

int main()
{
	Subtree btree;
	Tnptr *BR;
	btree.creat(BR);
	cout<<"tree size ="<<btree.getsize()<<endl;
	cout<<"tree height =" <<btree.getheight()<<endl;
		cout<<"tree sumnode =" <<btree.sumnode()<<endl;
	return 0;
}
