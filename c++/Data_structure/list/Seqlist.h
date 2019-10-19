#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 50

typedef void SeqList;
typedef void SeqListNode;
typedef unsigned int TSqlnode;// point add may be is unsigned typedef


class Seq_list
{
	public:
		Seq_list();
		SeqList* creat(int capcity);
		SeqList* get(SeqList *list,int pos);
		SeqList* Delete(SeqList *list,int pos);
		void destroy(SeqList *list);
		bool instert  (SeqList *list, int pos, TSqlnode node);
		int length(SeqList *list);
};
