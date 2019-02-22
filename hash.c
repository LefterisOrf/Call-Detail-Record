#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "hash.h"
#include "heap.h"

#define RECNUMBER 10
#define BUCKETSIZE 12

struct Hash* HashInit(int numberofbuckets)
{
	int i;
	struct Hash* temp = malloc(sizeof(struct Hash));
	temp->bucketcapacity = BUCKETSIZE;
	temp->entries = 0;
	temp->numofbuckets = numberofbuckets;
	temp->buckets = malloc((temp->numofbuckets)* sizeof(struct listnode*));
	for (int i = 0; i < temp->numofbuckets; ++i) temp->buckets[i] = NULL;
	return temp;
}

int HashFunction(char* id,int numofbuckets)
{
	int pos = (3319 * ((int)id[12] + (int)id[13] )) % numofbuckets;
	return pos;
}

struct Hash* HashInsertion(char* id,struct Hash* table, struct CDR* item,int datanode)
{
	int pos = HashFunction(id,table->numofbuckets);
	//printf("HashFunction returned: %d\n",pos);
	if(table->buckets[pos] == NULL) 
	{
		//printf("Creating listnode in the hashtable\n");
		int i;
		table->buckets[pos]=malloc(sizeof(struct listnode)); 
		table->buckets[pos]->next = NULL;
		table->buckets[pos]->datapernode = datanode;
		table->buckets[pos]->listdata = malloc(table->buckets[pos]->datapernode * sizeof(struct set*));
		for (int i = 0; i < datanode; ++i)table->buckets[pos]->listdata[i] = NULL;
	}
	//printf("\n\nHash[%d] value is: %p. And datapernode is : %d\n\n\n\n",pos, table->buckets[pos], table->buckets[pos]->datapernode );
	ListInsertion(id,table->buckets[pos], item, datanode);// ListInsertion must be created 
	table->entries++;
	return table;
}

void DeleteHash(struct Hash* table)
{
	int i;
	for (i = 0; i < table->numofbuckets; ++i)
	{
		if(table->buckets[i] != NULL)
			{
				//printf("In DeleteHash about to delete buckets[%d] with value %p\n",i,table->buckets[i] );
				DeleteList(table->buckets[i]);
			}
	}
	free(table->buckets);
	free(table);
}

void DeleteHashwoCDR(struct Hash* table)
{
	int i;
	for (i = 0; i < table->numofbuckets; ++i)
	{
		if(table->buckets[i] != NULL)
			{
				//printf("In DeleteHashwoCDR about to delete buckets[%d] with value %p\n",i,table->buckets[i] );
				DeleteListwoCDR(table->buckets[i]);
			}
	}
	free(table->buckets);
	free(table);
}

int SearchHash(struct Hash* table,char* caller,char* id)
{
	int pos = HashFunction(caller,(table->numofbuckets));
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPos is :%d\nNumofbuckets : %d\n\n\n\n\n\n\n\n\n\n\n\n\n",pos,table->numofbuckets);

	if (table->buckets[pos] == NULL)
	{
		printf("In SearchHash invalid caller number\n");
		return -1;
	}
	else return pos;
}

void PrintHash(struct Hash* table)
{
	printf("\n\n----------PrintHash---------\n");
	int i;
	for (int i = 0; i < table->numofbuckets; ++i)
	{
		printf("Hash[%d] value is : %p \n",i,table->buckets[i]);
		if(table->buckets[i] != NULL)PrintList(table->buckets[i]);
	}
	printf("\n\n---------PrintHash Completed---------\n\n");
}

int DeleteNode(struct Hash* table,char* caller,char* id)
{
	struct listnode* temp;
	int hashpos = SearchHash(table,caller,id);
	int listpos;
	int recpos;
	if (hashpos != -1)
	{
		listpos = SearchList(table->buckets[hashpos],caller,id);
		if (listpos != -1)
		{
			temp=table->buckets[hashpos];
			recpos = SearchRecords(table->buckets[hashpos]->listdata[listpos]->pointer,caller,id);
			if (recpos == -1)// if it isnt in the first node of the list then chech the others
			{
				temp = table->buckets[hashpos]->next;
				while(temp != NULL)
				{
					recpos = SearchRecords(temp->listdata[listpos]->pointer,caller,id);
					if (recpos >= 0)break;
					temp = temp->next;
				}
				if(recpos == -1)return -1;
			}
		}
		else 
		{
			printf("SearchList returned -1\n");
			return -1;
		}
	}
	else 
	{
		printf("SearchHash returned -1\n");
		return -1;
	}
	temp->listdata[listpos]->pointer->Recs[recpos] = NULL;
	if (IsRecordEmpty(table->buckets[hashpos]->listdata[listpos]->pointer) == 1)
	{
		DeleteRecordwoCDR(table->buckets[hashpos]->listdata[listpos]->pointer);
		free(table->buckets[hashpos]->listdata[listpos]);
		table->buckets[hashpos]->listdata[listpos] = NULL;
	}
	if (IsListEmpty(table->buckets[hashpos]) == 1)
	{
		free(table->buckets[hashpos]->listdata);
		free(table->buckets[hashpos]);
		table->buckets[hashpos] = NULL;
	}
}

void FindCaller(struct Hash* table, char* caller,char* time1,char* year1,char* time2,char* year2)
{
	int pos = HashFunction(caller,table->numofbuckets);
	FindCallerList(table->buckets[pos],caller,time1,year1,time2,year2);
}

void FindTopDest(struct Hash* table, char* caller)
{
	int pos = HashFunction(caller,table->numofbuckets);
	FindTopDestList(table->buckets[pos],caller);
}