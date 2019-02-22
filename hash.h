struct Hash
{
	struct listnode** buckets;
	int numofbuckets;
	int bucketcapacity;
	int entries;
};

struct Hash* HashInit(int );//the argument is for the function to know how many buckets to create

int HashFunction(char* ,int);

struct Hash* HashInsertion(char* ,struct Hash* , struct CDR* ,int );

int SearchHash(struct Hash* ,char* ,char* );

void DeleteHash(struct Hash* );

void DeleteHashwoCDR(struct Hash* );

void PrintHash(struct Hash* );

int DeleteNode(struct Hash* ,char* ,char* );

void FindCaller(struct Hash* , char* ,char* ,char* ,char* ,char* );

void FindTopDest(struct Hash* , char* );