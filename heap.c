#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "heap.h"
#include "hash.h"


int HeapTraverse(struct heap* root, struct CDR* item)
{
	int done = 0;
	if(root == NULL)return -1;
	if( strcmp(root->number , item->originator_number) == 0 )
	{
		root->totalsum += item->tarrif ; 
		return 1;
	}
	done = HeapTraverse(root->left , item);
	if (done == 1)return 1;
	done = HeapTraverse(root->right , item);
	if(done == 1)return 1;
	else return 0;
}

int bitcounter(int number)
{
	int counter = 0;
	while(1)
	{
		if(number != 0)counter++;
		else return counter;
		number  >>= 1;
	}
}

struct heap* HeapInsertion(struct heap* root, struct CDR* item)
{
	int bits,i,num,temp,j;
	struct heap* data;
	if(root == NULL)
	{
		//printf("Allocated first heap node\n");
		root = malloc(sizeof(struct heap));
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;
		root->totalsum = item->tarrif;
		root->nodenum = 1;
		strcpy(root->number , item->originator_number);
	}
	else
	{
		/*************** Search the heap for the cdr_uniq_id. If it exists sum up the totalsum of the user ********/
		if(HeapTraverse(root , item) != 1)//printf("\t\t\t\t\t\tItem with originator_number: %s wont allocate new node\n",item->originator_number); // the totalsum has been updated
		{
			/*********** Else insert the node to the last node and heapify ***********/
			root->nodenum ++;
			num = root->nodenum;
			bits = bitcounter(num);
			//printf("Number is: %d and numofbits is: %d\n",num ,bits );
			data = root;
			for ( i = 0; i < bits-1; ++i)
			{
				temp = num;
				for (j = 0; j < (bits - i - 2); ++j) temp >>=1;
				temp &= 1;
				//printf("temp is:%d\n",temp );
				if (temp == 0)
				{
					if (data->left != NULL) data = data->left;
					else 
					{
						//printf("Allocated another heapnode\n");
						data->left = malloc(sizeof(struct heap));
						data->left->parent = data;
						data = data->left;
						data->left = NULL;
						data->right = NULL;
						data->nodenum = root->nodenum; // we wont use it because it wont be updated
						data->totalsum = item->tarrif;
						strcpy(data->number , item->originator_number);
					}
				}
				else if (temp == 1)
				{
					if (data->right != NULL)data = data->right;
					else
					{
						//printf("\tAllocated another heap node\n");
						data->right = malloc(sizeof(struct heap));
						data->right->parent = data;
						data = data->right;
						data->left = NULL;
						data->right = NULL;
						data->nodenum = root->nodenum; // we wont use it because it wont be updated
						data->totalsum = item->tarrif;
						strcpy(data->number , item->originator_number);
					}
				}
			}
		}
	}
	//preorder(root);
	Heapify(root,NULL);
	return root;
}

void swap(struct heap* node,struct heap* father)
{
	float tempsum = father->totalsum;
	char tempid[15];
	strcpy(tempid,father->number);
	father->totalsum = node->totalsum;
	strcpy(father->number,node->number);
	node->totalsum = tempsum;
	strcpy(node->number,tempid);
}

void Heapify(struct heap* node, struct heap* father)
{
	if (node == NULL)return;
	Heapify(node->left,node);
	Heapify(node->right,node);
	if (father != NULL)
	{
		if (node->totalsum > father->totalsum)swap(node,father);
	}
}

void preorder(struct heap* node)
{
	if (node == NULL)return;
	Heapify(node,NULL);
	preorder(node->left);
	preorder(node->right);
}

void PrintHeap(struct heap* root)
{
	struct heap* temp;
	if (root == NULL)return;
	printf("uniq_id: %s with totalsum: %.2f\n",root->number, root->totalsum);
	PrintHeap(root->left);
	PrintHeap(root->right);
}

void DeleteHeap(struct heap* root)
{	
	if (root == NULL)return;
	if(root->left != NULL)DeleteHeap(root->left);
	if(root->right != NULL)DeleteHeap(root->right);
	free(root);
}