/*
 *平衡二叉树 AVL 树，二叉树满足平衡二叉树的条件是什么，就是对于每一个节点，它的右子树深度减去左子树的深度的绝对值必须是小于2才行
 */

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>

#define EH 0 // 等高
#define RH -1 // 右高
#define LH 1 // 左高

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

		string s;
		void creat(Tnptr* &btr,string s);
		int getsize();
		int getheight();
		int sumnode();
		//遍历
		void perorder(Tnptr* &btr);//前序
		void inorder(Tnptr* &btr);//中序
		void lastorder(Tnptr* &btr);//后序
		
		void destroy(Tnptr* &root);//销毁 tree
		bool search(char key,Tnptr* &btr); ////______________ ____________tree
		bool is_AVLbinarytree(Tnptr* &btr,int &depth);
	

	private:
		int size;
		Tnptr *m_root;
		int getheight(Tnptr* &btr);
		int sumnode(Tnptr* &btr);
};

void Subtree :: creat(Tnptr* &btr,string st)
{
	cout<<st<<endl;
	ElemType va;
	static int count = 0;
	cin >> va;
	if(search(va,btr))
	{
		cout<<" this data already installed "<<endl;	
	}else{

	if(va == '#')
	{
		btr =NULL;// if to # that is yezhi node  lift and right  set 0
	}else
	{
		btr =new Tnptr(count++);// 可以在struct里面这样用
		if(btr == NULL)
			exit(1);
		Subtree::size++; //只用 size++ 无效
		btr->data = va;
		creat(btr->lchild,"creat lchild");
		creat(btr->rchild,"creat rchild");

	}
	Subtree::m_root =btr;
	//memcpy(Subtree::m_root,btr,sizeof(btr)); //不能成功
	}
}

void  Subtree::destroy(Tnptr* &root)
{
	if(root == NULL)
	{
		
	}else{
		destroy(root ->lchild);
		destroy(root ->rchild);
		delete root;
		root = NULL;
		size = 0;
	}
}

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

//	cout<<"left_l ="<<left_l<<"right_l = "<<right_l<<endl;
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
		return 1+sumnode(root->lchild)+sumnode(root->rchild);
	}

}

int Subtree::sumnode()
{
	return	sumnode(m_root);
}

void Subtree::perorder(Tnptr* &btr)
{
	if(btr ==NULL)
	{
		
	}else
	{
		cout<<btr->data<<' ';
		perorder(btr->lchild);
		perorder(btr->rchild);
	}
}

void Subtree:: inorder(Tnptr* &btr)
{
	if(btr)
	{
	
		inorder(btr->lchild);
		cout<<btr->data<<' ';
		inorder(btr->rchild);
	}
}

void Subtree::lastorder(Tnptr* &btr)
{
	if(btr)
	{
		lastorder(btr->lchild);
		lastorder(btr->rchild);	
		cout<<btr->data<<' ';
	}
}

bool Subtree :: search(char key,Tnptr* &btr)
{
	Tnptr* T;
	Tnptr*per;
	if(!btr)
	{
		T =per;
		return false;
	}
	else if(key == btr->data )
	{
		T = btr;
		cout <<"find data is ="<<btr ->data<<endl;
		return true;

	}
	
	if(key < btr->data)
	{
		cout<< "key < data"<<endl;
		search(key,btr->lchild);
	}
	else{
		cout<< "key >data"<<endl;
		search(key,btr->rchild);
	}

}

bool is_Balence(const int &bf)
{
//	cout <<"is Balance bf "<<bf<<endl;
	return abs(bf)< 2;
}

bool Subtree :: is_AVLbinarytree(Tnptr* &btr,int &depth)
{
	if(btr == NULL)
	{
		cout <<"binary tree is NULL "<<endl;
		return true;
	}
	int right = 0;
	int left = 0;
	if(is_AVLbinarytree(btr->lchild,left) && is_AVLbinarytree(btr->rchild,right))
	{
		cout<<"right = "<<right<<" left = "<<left<<endl;
		int bf =right - left;
		cout<<"bf ="<<bf<<endl;
		if(is_Balence(bf))
		{
			depth = (right >left? right : left)+1;
			cout<<"depth ="<<depth<<endl;
			return true;
		}
		else{
			return false;
		}
	}
}

int main(int argc, char *argv[])
{
	Subtree btree;
	Tnptr *BR =NULL;
	btree.creat(BR,"main caall");
	cout<<"tree size ="<<btree.getsize()<<endl;
	cout<<"tree height =" <<btree.getheight()<<endl;
	cout<<"tree sumnode =" <<btree.sumnode()<<endl;
	cout<<"PreOrder:"<<endl;
	btree.perorder(BR);
	cout<<endl;
	cout<<"Inorder:"<<endl;
	btree.inorder(BR);
	cout<<endl;
	cout<<"lastorder:"<<endl;
	btree.lastorder(BR);
	cout<<endl;	

	cout<<"step : is_banlence_AVL_tree:"<<endl;
	int tr= 0;
	btree.is_AVLbinarytree(BR,tr);
	if(argc < 2)
	{
		cout<<"please input need search num"<<endl;
	}

	cout<<"search num:"<<argv<<endl;
	char input = (char) argv;
	btree.search(input,BR);
	return 0;
}

