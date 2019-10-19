#include "Seqlist.h"

using namespace std;



typedef struct 
{
	int length;
	int capcity;
	TSqlnode *node;// dynamic get memery use point
}sqlist;

SeqList* Seq_list :: creat(int capcity)
{
	sqlist *list = NULL;

	if (capcity > 0)
	{
		list = (sqlist *)sizeof(sqlist) + sizeof(TSqlnode) *capcity;
	}
	if (list != NULL)
	{
		list -> length =0;
		list ->capcity = capcity;
		list ->node = (TSqlnode*)list +1;// point to [sizeof(TSqlnode) *capcity] of memery
	}
}

void Seq_list :: destroy(SeqList *list)
{
	sqlist *slist = (sqlist*)list; 
	free(list);
	if(list != NULL)
	{
		list = NULL;
	}
}

int  Seq_list :: length(SeqList *list)
{
	 sqlist *slist = (sqlist*)list;
	 return slist ->length;

}

bool Seq_list :: instert(SeqList *list, int pos, TSqlnode node)
{ 
	sqlist *slist =(sqlist*)list;
	int i =0;

	if((pos >0) && (pos < slist -> length+1) &&  (slist ->length <= slist ->capcity))
	{
		if(pos >= slist -> length)
		{
			pos = slist -> length;
		}

		for(i =slist -> length;i > pos ;i--)
		{
			slist -> node[i] = slist -> node[i-1];
		}

		cout<< "insterr i = "<<endl;

		slist ->node[i] = node;

		slist -> length++;

		return true;

	}

	return false;

}

SeqList* Seq_list :: get(SeqList *list,int pos)
{

	sqlist* sList = (sqlist*)list;
    SeqListNode* ret = NULL;
    
    if( (sList != NULL) && (0 <= pos) && (pos < sList->length) )
    {
        ret = (SeqListNode*)(sList->node[pos]);
    }
    
    return ret;
}


int main ()
{
	sqlist *ulist;

	



}
