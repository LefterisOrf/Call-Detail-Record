#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "hash.h"
#include "heap.h"

int main(int argc, char* argv[])
{
	int j=0,inserts=0,flag=0;
	int hash1,hash2,BucketSize,i;
	char file[35],command[18],id[40];
	size_t bufsize = 250;
	char* buffer = malloc(bufsize * sizeof(char));
	FILE* input = NULL;
	FILE* config = NULL;
	for ( i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i],"-o") == 0)
		{
			input = fopen(argv[i+1],"rb");
			strcpy(file,argv[i+1]);
		}
		else if (strcmp(argv[i],"-s") == 0)
		{
			BucketSize= atoi(argv[i+1]);
			if(BucketSize <= 0)
			{
				fprintf(stderr,"Invalid BucketSize!\n");
				exit(-1);
			}
		}
		else if (strcmp(argv[i] , "-h1") == 0)
		{
			hash1= atoi(argv[i+1]);
			if (hash1 <= 0)
			{
				fprintf(stderr,"Invalid Hashtable1 number of entries!\n");
				exit(-1);
			}
		}
		else if (strcmp(argv[i] , "-h2") == 0)
		{
			hash2= atoi(argv[i+1]);
			if (hash2 <= 0)
			{
				fprintf(stderr,"Invalid Hashtable2 number of entries!\n");
				exit(-1);
			}
		}
		else if (strcmp(argv[i],"-c") == 0)
		{
			config = fopen(argv[i+1],"r");
		}
	}
	// Calculate the bucket size
	int datanode = (BucketSize / sizeof(struct set));
	printf("DATANODE is : %d \n",datanode );
	if(input == NULL)
	{
		printf("No input file was given, so the program will take commands from stdin\n");
		input = stdin;
	}
	struct Hash* hashtable1 = HashInit(hash1);
	struct Hash* hashtable2 = HashInit(hash2);
	struct heap* root = NULL;
	while(1)
	{
		if(getline(&buffer,&bufsize,input) == -1)
		{
			fclose(input);
			input = stdin;
			printf("Switched from file to stdin\n");
			getline(&buffer,&bufsize,input);
		}
		if ((strlen(buffer) -1) > 0)buffer[(strlen(buffer) - 1)] = '\0'; // the getline() returns a string with a \n in the end , which we dont need
		strcpy(command,strtok(buffer," ;"));
		//printf("%s\n",command );
		if (strcmp(command,"insert") == 0)
		{
			inserts++;
			struct CDR* temp = malloc(sizeof(struct CDR));
			strcpy(id,strtok(NULL,";"));
			temp->cdr_uniq_id = malloc((strlen(id) + 1) * sizeof(char));
			strcpy(temp->cdr_uniq_id , id);
			printf("cdr_uniq_id : %s",temp->cdr_uniq_id);
			strcpy(temp->originator_number,strtok(NULL,";"));
			printf(" and originator_number: %s\n",temp->originator_number );
			strcpy(temp->destination_number,strtok(NULL,";"));
			strcpy(temp->date,strtok(NULL,";"));
			strcpy(temp->init_time,strtok(NULL,";"));
			temp->duration = atoi(strtok(NULL,";"));
			temp->type = atoi(strtok(NULL,";"));
			temp->tarrif = atof(strtok(NULL,";"));
			temp->fault_condition= atoi(strtok(NULL,";"));
			hashtable1 = HashInsertion(temp->originator_number,hashtable1,temp,datanode);
			hashtable2 = HashInsertion(temp->destination_number,hashtable2,temp,datanode);
			root = HeapInsertion(root , temp);
		}
		else if (strcmp(command,"find") == 0)
		{
			int j;
			int i=0;
			char *callid;
			char *temparg[4];
			for (i = 0; i < 4; ++i)temparg[i] = NULL;
			callid = strtok(NULL," ");
			i=0;
			while( (temparg[i] = strtok(NULL," ")) != NULL )i++;
			for (j = 0; j < 4; ++j)	if (temparg[j] != NULL)printf("%s\n",temparg[j] );
			printf("-------------Find-------------\n%d\n",i);
			if (i == 2) // we have only 1 set of optional arguments
			{
				if (strlen(temparg[0]) > 6) // then the given arguments are date
				{
					FindCaller(hashtable1,callid,NULL,temparg[0],NULL,temparg[1]);
				}
				else FindCaller(hashtable1,callid,temparg[0],NULL,temparg[1],NULL);
			}
			else if (i == 4)FindCaller(hashtable1,callid,temparg[0],temparg[1],temparg[2],temparg[3]);// we know it is time1 year1 time2 year2
			else if (i == 0)FindCaller(hashtable1,callid,NULL,NULL,NULL,NULL);// we know we have to print the whole bucket of cdrs
			else printf("Wrong arguments in find\n");
			printf("---------Find just ended------------\n");
		}
		else if (strcmp(command,"top") == 0)
		{
			/* code */
		}
		else if (strcmp(command,"delete") == 0)
		{
			char caller[15];
			char tempid[30];
			strcpy(caller,strtok(NULL," "));
			strcpy(tempid,strtok(NULL," "));
			printf("Caller is: %s and CDR : %s\n",caller,tempid );
			if (DeleteNode(hashtable1,caller,tempid) == -1)printf("Error in DeleteNode , something was -1\n");
			else printf("Succesfully deleted a node\n\n");
		}
		else if (strcmp(command,"lookup") == 0)
		{
			int j;
			int i=0;
			char *callid;
			char *temparg[4];
			for (i = 0; i < 4; ++i)temparg[i] = NULL;
			callid = strtok(NULL," ");
			i=0;
			while( (temparg[i] = strtok(NULL," ")) != NULL )i++;
			for (j = 0; j < 4; ++j)
			{
				printf("In find loop\n");
				if (temparg[j] != NULL)printf("%s\n",temparg[j] );
			}
			printf("-------------Lookup-------------\n%d\n",i);
			if (i == 2) // we have only 1 set of optional arguments
			{
				if (strlen(temparg[0]) > 6) // then the given arguments are date
				{
					FindCaller(hashtable2,callid,NULL,temparg[0],NULL,temparg[1]);
				}
				else FindCaller(hashtable2,callid,temparg[0],NULL,temparg[1],NULL);
			}
			else if (i == 4)FindCaller(hashtable2,callid,temparg[0],temparg[1],temparg[2],temparg[3]);// we know it is time1 year1 time2 year2
			else if (i == 0)FindCaller(hashtable2,callid,NULL,NULL,NULL,NULL);// we know we have to print the whole bucket of cdrs
			else printf("Wrong arguments in find\n");
			printf("---------Lookup just ended------------\n");
		}
		else if (strcmp(command,"topdest") == 0)
		{
			char *callid;
			callid = strtok(NULL," ");
			FindTopDest(hashtable1,callid);
		}
		else if (strcmp(command,"print") == 0)
		{
			char temp[20];
			strcpy(temp,strtok(NULL," "));
			if ((strcmp(temp,"hashtable1") == 0) || (strcmp(temp,"hashtable1\n") == 0))
			{
				PrintHash(hashtable1);
				printf("Just printed hashtable1\n");
			}
			else 
			{
				PrintHash(hashtable2);
				printf("Just printed hashtable2\n");
			}
		}
		else if (strcmp(command,"indist") == 0)
		{
			/* code */
		}
		else if ((strcmp(buffer,"bye\n") == 0) || (strcmp(buffer,"bye") == 0))
		{
			DeleteHeap(root);
			DeleteHash(hashtable2);
			DeleteHashwoCDR(hashtable1);
			hashtable1 = HashInit(hash1);
			hashtable2 = HashInit(hash2);
			root = NULL;
		}
		else if ((strcmp(buffer,"exit") == 0) || (strcmp(buffer,"exit\n") == 0))
		{
			input= NULL;
			printf("About to exit\n");
			break;
		}
	}
	printf("Heap---------------\n");
	PrintHeap(root);
	printf("Heap---------------\n");
	DeleteHash(hashtable2);
	DeleteHashwoCDR(hashtable1);
	DeleteHeap(root);
	free(buffer);
	if(input != NULL)fclose(input);
	if(config != NULL)fclose(config);
	printf("Inserts are : %d\n",inserts );
	return 0;
}