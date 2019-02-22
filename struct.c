#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "hash.h"
#include "heap.h"
#include "linked.h"

#define RECNUMBER 10

int InsertRecord(struct Records* head , struct CDR* item ) // na balw thn periptwsh poy den xwraei allo arxeio ... na kanei allocate allo ena node
{
	int i;
	int insertion = 0;
	struct Records* temp = head;
	while((temp != NULL) && (insertion == 0))
	{
		for (i = 0; i < temp->recnum; ++i)
		{
			if (temp->Recs[i] == NULL)
			{
				//temp->Recs[i]= malloc(sizeof(struct CDR));
				temp->Recs[i] = item ; 
				insertion= 1;
				break;
			}
		}
		temp = temp->next;
	}
	if (insertion == 0)
	{
		while(head->next != NULL)head = head->next;
		head->next = malloc(sizeof(struct Records));
		head->next->recnum = RECNUMBER;
		head->next->next = NULL;
		head->next->Recs = malloc((head->next->recnum) * sizeof(struct CDR*));
		for (int i = 0; i < head->next->recnum; ++i)head->next->Recs[i] = NULL;
		InsertRecord(head->next, item);
	}
	return 0;
}

int SearchRecords(struct Records* head,char* caller, char* id)
{
	int i;
	while( head != NULL)
	{
		for ( i = 0; i < head->recnum; ++i)
		{
			if (head->Recs[i] != NULL)
			{
				if (strcmp(head->Recs[i]->cdr_uniq_id , id) == 0)
				{
					return i;
				}
			}
		}
		head = head->next;
	}
	return -1;
}

int ListInsertion(char* id,struct listnode* head , struct CDR* item,int datanode)
{
	//check if the list has the caller or callee number first
	int i,j;
	int flag=0,nodenum = -1, iters=0,insertion = 0;
	struct listnode* temp = head;
	while( (temp != NULL) && (flag == 0))
	{
		for (i = 0; i < temp->datapernode; ++i)
		{
			if (temp->listdata[i] != NULL)
			{
				if (strcmp(temp->listdata[i]->number_id , id ) == 0) // check for originator or destination number
				{
					//printf("\n\t\tFound a node in the list with the same number in listdata[%d]\n\n",i);
					InsertRecord(temp->listdata[i]->pointer , item);
					flag=1;
					break;
				}
			}
			else nodenum = iters;
		}
		temp= temp->next;
		iters++;
	}
	temp = head;
	//printf("In ListInsertion the iters value is: %d\n", iters);
	if (flag == 0) // an h flag einai 0 shmainei oti den brethike eggrafh sthn lista me ayto ton arithmo
	{
		//printf("ListInsertion head->datapernode is %d.... It must be 5\n",head->datapernode );
		for ( i = 0; i < iters-1; ++i) temp= temp->next; // go to the node where we last encountered null
		//printf("\n\t\tAfter the iters forloop the value of the head is %p\n\n",head );
		for (i = 0; i < temp->datapernode; ++i)
		{
			if (temp->listdata[i] == NULL)
			{
				temp->listdata[i] = malloc(sizeof(struct set));
				strcpy(temp->listdata[i]->number_id, id); // check for originator or destination number
				temp->listdata[i]->pointer = malloc(sizeof(struct Records));
				temp->listdata[i]->pointer->next = NULL;
				temp->listdata[i]->pointer->recnum = RECNUMBER;
				temp->listdata[i]->pointer->Recs = malloc(RECNUMBER * sizeof(struct CDR*));
				for (j = 0; j < RECNUMBER; ++j)temp->listdata[i]->pointer->Recs[j] = NULL;
				//printf("\nRecord is about to be inserted in temp->listdata[%d]->pointer: %p\n",i,temp->listdata[i]->pointer );
				InsertRecord(temp->listdata[i]->pointer , item);
				insertion = 1;
				break;
			}
		}
		if(insertion == 0)
		{
			while(head->next != NULL) head= head->next;
			head->next = malloc(sizeof(struct listnode));
			head->next->datapernode = datanode;
			head->next->next = NULL;
			head->next->listdata = malloc(head->next->datapernode * sizeof(struct set*));
			for (int i = 0; i < head->next->datapernode ; ++i)head->next->listdata[i] = NULL;
			ListInsertion(id,head->next , item , datanode);
		}
	}
	return 0;
}



// na kanw delete node an einai adeios kai na kanw shift ton pointer
// kai edw kai katw
int IsRecordEmpty(struct Records* head)
{
	int i;
	int flag=0;
	if (head == NULL)return -1;
	while(head != NULL)
	{
		for (i = 0; i < head->recnum; ++i)
		{
			if (head->Recs[i] != NULL)return 0;
		}
		head= head->next;
	}
	return 1;
}

int IsListEmpty(struct listnode* head)
{
	int i;
	int flag=0;
	if(head == NULL)return -1;
	while(head != NULL)
	{
		for (i = 0; i < head->datapernode; ++i)
		{
			if (head->listdata[i] != NULL)return 0;
		}
		head= head->next;
	}
	return 1;
}

int SearchList(struct listnode* head,char* caller,char* id)
{
	int i;
	while(head != NULL)
	{
		for (i = 0; i < head->datapernode; ++i)
		{
			if (head->listdata[i] != NULL)
			{
				printf("\n\n\n\nIn head->listdata[%d] with callerid: %s| and caller: %s|\n\n\n\n",i,head->listdata[i]->number_id,caller);
				if ( strcmp(head->listdata[i]->number_id , caller) == 0)
				{
					printf("Search list about to return i\n");
					return i;
				}
			}
		}
		head = head->next;
	}
	return -1;
}

void DeleteRecord(struct Records* data)
{
	int i;
	for (i = 0; i < data->recnum; ++i)
	{
		if(data->Recs[i] != NULL)
		{
			//printf("Diagrafei ena record\n");
			free(data->Recs[i]->cdr_uniq_id);
			free(data->Recs[i]);
		}
	}
	free(data->Recs);
	//printf("\t\t\tData->next = %p \n",data->next );
	if(data->next != NULL) DeleteRecord(data->next);
	free(data);
}

void DeleteList(struct listnode* head)
{
	int i;
	//printf("\tIn DeleteList datapernode is : %d\n",head->datapernode );
	for (i = 0; i < head->datapernode; ++i)
	{
		//printf("\tIn DeleteList head->listdata[%d] value is %p\n",i,head->listdata[i] );
		if (head->listdata[i] != NULL)
		{
			//printf("\t\tIn DeleteList head->listdata[%d]->number_id is : %p\n",i, head->listdata[i]->number_id);
			if(head->listdata[i]->pointer != NULL)
			{
				DeleteRecord(head->listdata[i]->pointer);
				//free(head->listdata[i]->pointer);
			}
			free(head->listdata[i]);
		}
	}
	free(head->listdata);
	//printf("\t\tHead->next = %p \n",head->next );
	if(head->next != NULL) DeleteList(head->next);
	free(head);
}

void DeleteRecordwoCDR(struct Records* data)
{
	free(data->Recs);
	if(data->next != NULL) DeleteRecordwoCDR(data->next);
	free(data);
}

void DeleteListwoCDR(struct listnode* head)
{
	int i;
	for (i = 0; i < head->datapernode; ++i)
	{
		//printf("\tIn DeleteListwoCDR head->listdata[%d] value is %p\n",i,head->listdata[i] );
		if (head->listdata[i] != NULL)
		{
			//printf("\t\tIn DeleteList head->listdata[%d]->number_id is : %p\n",i, head->listdata[i]->number_id);
			if(head->listdata[i]->pointer != NULL)
			{
				DeleteRecordwoCDR(head->listdata[i]->pointer);
			}
			free(head->listdata[i]);
		}
	}
	free(head->listdata);
	if(head->next != NULL) DeleteListwoCDR(head->next);
	free(head);
}

void PrintRecords(struct Records* header)
{
	int i;
	for (int i = 0; i < header->recnum; ++i)
	{
		//printf("\t\t\tRecords->Recs[%d] value is: %p\n",i,header->Recs[i] );
		if (header->Recs[i]!=NULL)PrintCDR(header->Recs[i]);
	}
	if (header->next != NULL)
	{
		printf("\t\t\tWill print the next node with value %p \n",header->next );
		PrintRecords(header->next);
	}
}

void PrintList(struct listnode* header)
{
	int i;
	for ( i = 0; i < header->datapernode; ++i)
	{
		printf("\t\tList->listdata[%d] value is: %p\n",i,header->listdata[i] );
		if(header->listdata[i] != NULL)PrintRecords(header->listdata[i]->pointer);
	}
	if (header->next != NULL)
	{
		printf("\t\tWill print the next node with value %p \n",header->next );
		PrintList(header->next);
	}
}

void PrintCDR(struct CDR* data)
{
	printf("********CDR********\n");
	printf("%s\n",data->cdr_uniq_id);
	printf("Orig: %s\n",data->originator_number);
	printf("Dest: %s\n",data->destination_number);
	printf("Date: %s\n",data->date);
	printf("Time: %s\n",data->init_time);
	printf("Dur : %d\n",data->duration);
	printf("Type: %d\n",data->type);
	printf("Tarr: %f\n",data->tarrif);
	printf("FCon: %d\n",data->fault_condition);
	printf("*******************\n");
}

struct Records* SearchListNode(struct listnode* head, char* caller)
{
	int i;
	while(head != NULL)
	{
		for (i = 0; i < head->datapernode; ++i)
		{
			if (head->listdata[i] != NULL)
			{
				if ( strcmp(head->listdata[i]->number_id , caller) == 0)return head->listdata[i]->pointer;
			}
		}
		head = head->next;
	}
	return NULL;
}

void FindCallerRecords(struct Records* header,char* caller,char* time1,char* year1,char* time2,char* year2)
{
	int i;
	struct CDR* temp;
	char day1[3],month1[3],etos1[5],day2[3],month2[3],etos2[5];
	struct tm tm1,tm2,tmtemp;
	memset(&tm1, 0, sizeof(struct tm));
	memset(&tm2, 0, sizeof(struct tm));
	memset(&tmtemp, 0, sizeof(struct tm));
	time_t t1 = 0;//mktime(&tm1);
	time_t t2 = 0;//mktime(&tm2);
	time_t ttemp = 0;
	double diff = 0;
	printf("Year1: %s Year2: %s\n",year1,year2 );
	if ((time1 == NULL) && (year1 == NULL))PrintRecords(header);
	else if(time1 == NULL)// then we just check the year
	{
		strptime(year1,"%d%m%Y",&tm1);
		strptime(year2,"%d%m%Y",&tm2);
		t1 = mktime(&tm1);
		t2 = mktime(&tm2);
		while(header != NULL)
		{
			for (i = 0; i < header->recnum; ++i)
			{
				if (header->Recs[i] != NULL)
				{
					memset(&tmtemp, 0, sizeof(struct tm));
					strptime(header->Recs[i]->date,"%d%m%Y",&tmtemp);
					ttemp = mktime(&tmtemp);
					diff = difftime(t1,ttemp);
					if (diff < 0)
					{
						diff = difftime(t2,ttemp);
						if (diff > 0)
						{
							PrintCDR(header->Recs[i]);
						}
					}
				}
			}
			header = header->next;
		}
	}
	else if(year1 == NULL)// we just check the time
	{
		printf("Time1: %s Time2: %s\n",time1,time2 );
		strptime(time1,"%H:%M",&tm1);
		strptime(time2,"%H:%M",&tm2);
		t1 = mktime(&tm1);
		t2 = mktime(&tm2);
		while(header != NULL)
		{
			for (i = 0; i < header->recnum; ++i)
			{
				if (header->Recs[i] != NULL)
				{
					memset(&tmtemp, 0, sizeof(struct tm));
					strptime(header->Recs[i]->init_time,"%H:%M",&tmtemp);
					ttemp = mktime(&tmtemp);
					diff = difftime(t1,ttemp);
					if (diff < 0)
					{
						diff = difftime(t2,ttemp);
						if (diff > 0)PrintCDR(header->Recs[i]);
					}
				}
			}
			header = header->next;
		}
	}
	else // we must chech both year and time
	{
		strptime(year1,"%d%m%Y",&tm1);
		strptime(year2,"%d%m%Y",&tm2);
		strptime(time1,"%H:%M",&tm1);
		strptime(time2,"%H:%M",&tm2);
		t1 = mktime(&tm1);
		t2 = mktime(&tm2);
		while(header != NULL)
		{
			for (i = 0; i < header->recnum; ++i)
			{
				if (header->Recs[i] != NULL)
				{
					memset(&tmtemp, 0, sizeof(struct tm));
					strptime(header->Recs[i]->init_time,"%H:%M",&tmtemp);
					strptime(header->Recs[i]->date,"%d%m%Y",&tmtemp);
					ttemp = mktime(&tmtemp);
					diff = difftime(t1,ttemp);
					if (diff < 0)
					{
						diff = difftime(t2,ttemp);
						if (diff > 0)PrintCDR(header->Recs[i]);
					}
				}
			}
			header = header->next;
		}
	}
}

void FindCallerList(struct listnode* head,char* caller,char* time1,char* year1,char* time2,char* year2)
{
	struct Records* temp = SearchListNode(head,caller);
	if(temp != NULL)FindCallerRecords(temp,caller,time1,year1,time2,year2);
}


void FindTopDestRecords(struct Records* head)
{
	int i,j,size = 10,flag;
	char temp[4];
	temp[3] = '\0';
	struct linked* list = NULL;
	while(head != NULL)
	{
		for (i = 0; i < head->recnum; ++i)
		{
			flag = 0;
			if (head->Recs[i] != NULL)
			{
				for (j = 0; j < 3; ++j)temp[j] =  head->Recs[i]->destination_number[j];
				flag = SearchLinked(list,temp);
				if (flag == -1)list = InsertLinked(list,temp);
			}
		}
		head = head->next;
	}
	struct linked* tempnode = FindMax(list);
	if (temp != NULL)
	{
		printf("---------Topdest---------\n");
		printf("Topdest is : %s with %d calls\n",tempnode->code,tempnode->counter );
		printf("---------Topdest ended---\n");
	}
	else printf("Error in topdest\n");
	DeleteLinked(list);
}

void FindTopDestList(struct listnode* head,char* caller)
{
	struct Records* temp = SearchListNode(head,caller);
	if (temp != NULL) FindTopDestRecords(temp);
}