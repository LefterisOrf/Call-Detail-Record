struct CDR 
{
	char* cdr_uniq_id;// i have to allocate mem  
	char originator_number[15];
	char destination_number[15];
	char date[9];
	char init_time[6];
	int duration;
	short int type;
	float tarrif;
	short int fault_condition;
};

struct Records
{
	int recnum;
	struct CDR** Recs;
	struct Records* next;

};

struct set
{
	char number_id[15];
	struct Records* pointer;
};

struct listnode
{
	int datapernode;
	struct set** listdata;
	struct listnode* next;
};

struct topdest
{
	char code[4];
	int count;
};

void PrintCDR(struct CDR* );

void PrintRecords(struct Records* );

void PrintList(struct listnode* );

int InsertRecord(struct Records* , struct CDR*  );

int SearchRecords(struct Records* ,char* , char* );

int ListInsertion(char* ,struct listnode* , struct CDR* ,int datanode);

int SearchList(struct listnode* ,char* ,char* );

int IsRecordEmpty(struct Records* );

int IsListEmpty(struct listnode* );

void DeleteList(struct listnode* );

void DeleteRecordwoCDR(struct Records* );

void DeleteListwoCDR(struct listnode* );

void DeleteRecord(struct Records* );

void FindCallerRecords(struct Records* ,char* ,char* ,char* ,char* ,char* );

void FindCallerList(struct listnode* ,char* ,char* ,char* ,char* ,char* );

void FindTopDestRecords(struct Records* );

void FindTopDestList(struct listnode* ,char* );