#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "hash.h"
#include "heap.h"
#include "linked.h"


struct linked* InsertLinked(struct linked* head, char* id)
{
	struct linked* temp = head;
	if (head == NULL)
	{
		head = malloc(sizeof(struct linked));
		strcpy(head->code,id);
		head->counter = 1;
		head->next = NULL;
		return head;
	}
	while( temp->next != NULL)temp=temp->next;
	temp->next = malloc(sizeof(struct linked));
	temp = temp->next;
	strcpy(temp->code,id);
	temp->counter = 1;
	temp->next = NULL;
	return head;
}

int SearchLinked(struct linked* head, char* id)
{
	while(head != NULL)
	{
		if(strcmp(id,head->code) == 0)
		{
			head->counter++;
			return 1;
		}
		head = head->next;
	}
	return -1;
}

void DeleteLinked(struct linked* head)
{
	if (head->next != NULL) DeleteLinked(head->next);
	free(head);
}

struct linked* FindMax(struct linked* head)
{
	struct linked* temp = head;
	while(head != NULL)
	{
		if (head->counter > temp->counter)temp = head;
		head = head->next;
	}
	return temp;
}