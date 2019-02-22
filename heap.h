struct heap {
	float totalsum;
	char number[15]; // must be malloced
	int nodenum;
	struct heap* parent;
	struct heap* left;
	struct heap* right;
};


int HeapTraverse(struct heap* , struct CDR* );

int bitcounter(int );

struct heap* HeapInsertion(struct heap* , struct CDR* );

void swap(struct heap* ,struct heap* );

void Heapify(struct heap* , struct heap* );

void preorder(struct heap* );

void PrintHeap(struct heap* );

void DeleteHeap(struct heap* );