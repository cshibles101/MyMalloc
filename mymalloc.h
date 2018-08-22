/**
	Authors: Olga Bugrina (RUID 126-00-5883) and Christopher Shibles (RUID 156-00-0042)
**/

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x,__FILE__, __LINE__)

void * mymalloc(size_t size, char * fileString, int lineInt);
int myfree(void *ptr, char * fileString, int lineInt);
int boundaryCheck(void *ptr);