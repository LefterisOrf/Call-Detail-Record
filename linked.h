struct linked
{
	char code[4];
	int counter;
	struct linked* next;
};

struct linked* InsertLinked(struct linked* , char* );

int SearchLinked(struct linked* , char* );

void DeleteLinked(struct linked* );

struct linked* FindMax(struct linked* );