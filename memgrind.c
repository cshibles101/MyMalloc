#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/time.h> 
#include "mymalloc.h"

/**
	Authors: Olga Bugrina (RUID 126-00-5883) and Christopher Shibles (RUID 156-00-0042)
**/

static void * blocks;

 void runA(); 
 void runB(); 
 void runC(); 
 void runD(); 
 void runE(); 
 void runF(); 

struct Node{

	void * pointer; 
	struct Node * nextNode; 

};

int main(int argc, char *argv[]){
	
  blocks = malloc(1);
  free(blocks);
  //Gives us a pointer to the beginning of the memory block so we can easily point to and get the short from the first indices of the array.
  
  struct timeval t1, t2;
  double elapsedTimeA, elapsedTimeB, elapsedTimeC, elapsedTimeD, elapsedTimeE, elapsedTimeF;

  /* Running and Timing A */
  gettimeofday(&t1, NULL);
  int j;
  for(j=0; j< 100; j++){
		runA();
  }
  gettimeofday(&t2, NULL);

  elapsedTimeA = (t2.tv_sec - t1.tv_sec) * 1000.0;
  
  /* Running and Timing B */
  gettimeofday(&t1, NULL);
  for(j=0; j< 100; j++){
		runB();
  }
  gettimeofday(&t2, NULL);

  elapsedTimeB = (t2.tv_sec - t1.tv_sec) * 1000.0;
  
  /* Running and Timing C */
  gettimeofday(&t1, NULL);
  for(j=0; j< 100; j++){
		runC();
  }
  gettimeofday(&t2, NULL);

  elapsedTimeC = (t2.tv_sec - t1.tv_sec) * 1000.0;
  
  /* Running and Timing D */
  gettimeofday(&t1, NULL);
  for(j=0; j< 100; j++){
		runD();
  }
  gettimeofday(&t2, NULL);

  elapsedTimeD = (t2.tv_sec - t1.tv_sec) * 1000.0;
  
  /* Running and Timing E */
  gettimeofday(&t1, NULL);
  for(j=0; j< 100; j++){
		runE();
  }
  gettimeofday(&t2, NULL);

  elapsedTimeE = (t2.tv_sec - t1.tv_sec) * 1000.0;
  
  /* Running and Timing F */
  gettimeofday(&t1, NULL);
  for(j=0; j< 100; j++){
		runF();
  }
  gettimeofday(&t2, NULL);

  elapsedTimeF = (t2.tv_sec - t1.tv_sec) * 1000.0;
  
  printf("Time to perform A: %.16f\n", elapsedTimeA);
  printf("Time to perform B: %.16f\n", elapsedTimeB);
  printf("Time to perform C: %.16f\n", elapsedTimeC);
  printf("Time to perform D: %.16f\n", elapsedTimeD);
  printf("Time to perform E: %.16f\n", elapsedTimeE);
  printf("Time to perform F: %.16f\n", elapsedTimeF);
  
  
  /**
  runA(); 
  runB(); 
  runC(); 
  runD(); 
  runE(); 
  runF(); 
	**/
  
return 0; 
}

/*
  Memgrind scenario A
	*/
void runA(){

  int j;
  void * memA[3000];
  int aMCount = 0; //counts successful mallocs
  int aCount = 0; //counts successful frees

  for(j = 0; j < 3000; j++){
   	memA[j] = malloc(1); 
    if(memA[j] != NULL){
    	aMCount++;
    }
  }
  for(j = 0; j < 3000; j++){
    int g = free(memA[j]);
    if(g == 0)
      aCount++;
  }
  
  /*
  aCount is the count of successful frees
  aMCount is the count of Mallocs resulting in non-NULL pointers (successful mallocs) 
  prints the address and short value at that address of the beginning of myblock array that simulates memory. The value is 5000 if the memory all freed properly
  */
  printf("aCount = %d\n", aCount);
  printf("aMCount = %d\n", aMCount);
  printf("%p with size %d\n", blocks, *((short*)blocks));
}

/*
  Memgrind scenario B
*/

void runB(){
  
  int j;
  int g;
  void * BPtr;
  int bCount = 0; //counts successful frees
  int bMCount = 0; //counts successful mallocs
  
  for(j = 0; j < 3000; j++){
    BPtr = malloc(1);
    if(BPtr != NULL)
      bMCount++;
    g = free(BPtr);
    if(g == 0)
       bCount++;
  }
  
  /*
  bCount is the count of successful frees
  bMCount is the count of successful mallocs not resulting in NULL pointers
  prints the address and short value at that address of the beginning of myblock array that simulates memory. The value is 5000 if the memory all freed properly
  */
  printf("bCount = %d\n", bCount);
  printf("bMCount = %d\n", bMCount);
  printf("%p with size %d\n", blocks, *((short*)blocks));
}

  /*
  	Memgrind scenario C
	*/

void runC(){

  int j;
  int g; 
  srand (time(NULL));
  int random; //assigned 0 for malloc, 1 for free
  int cmalloc = 0; //counts mallocs
  int currIndex = 0; 
  void * memC[3000]; 
  int cCount = 0; //counts the frees
  int mCount = 0; //counts NULL frees
  
  for(j = 0; j < 6000; j++){
    random = rand() % 2; 
    if(cmalloc == 3000 || (random == 1 && cCount < cmalloc)){ 
      //Freeing
      //Need to check the value of the currIndex
      if(currIndex > 0){
        g = free(memC[currIndex-1]); 
        if(g)
      		mCount++;
        currIndex--;
    	}
      cCount++;
    }else {
      //Allocating
      memC[currIndex] = malloc(1);
      currIndex++;
      cmalloc++; 
    }
  }
  /*
  cCount is the count of total frees (successful and not successful)
  cmalloc is the count of total mallocs (successful and NULL returning)
  mCount is the count of frees attempted on null pointers
  prints the address and short value at that address of the beginning of myblock array that simulates memory. The value is 5000 if the memory all freed properly
  */
  printf("cCount = %d\n", cCount);
  printf("cmalloc = %d\n", cmalloc);
  printf("mCount = %d\n", mCount);
  printf("%p with size %d\n", blocks, *((short*)blocks));

}

	/*
		Memgrind scenario D 
	*/
void runD(){
  
  srand (time(NULL));
  int random;
  int randSize; //assigned 0 for malloc, 1 for free
  int currIndex = 0; 
  int cmalloc = 0;
  void * memD[3000];
  int dCount = 0; //counts frees
  int mCount = 0; //counts non-null mallocs
  int j;
  int g;

  for(j = 0; j < 6000; j++){
    random = rand() % 2;
    //printf("j = %d\n", j); //DELETE
    if(cmalloc == 3000 || (random == 1 && cmalloc > dCount)){ 
      g = free(memD[currIndex - 1]);
      if(g)
      	mCount++;
      currIndex--;
      dCount++;
    }else{
      randSize = rand() % 4997;
      memD[currIndex] = malloc(randSize);
      currIndex++;
      cmalloc++; 
    }
  }
  
  /*
  dCount is the count of total frees (successful and not successful)
  cmalloc is the count of total mallocs (successful and NULL returning)
  mCount is the count of frees attempted on null pointers
  prints the address and short value at that address of the beginning of myblock array that simulates memory. The value is 5000 if the memory all freed properly
  */ 
  
  printf("dCount = %d\n", dCount);
  printf("cmalloc = %d\n", cmalloc);
  printf("mCount = %d\n", mCount);
  printf("%p with size %d\n", blocks, *((short*)blocks));

}

  /*
		Memgrind scenario E 
  	Malloc the whole block into multiple allocated blocks. Then, randomly free each of the pointers. As each
    pointer is created (malloc'd), they're added to a linked list and then the nodes are randomly accessed and 
    the pointers are freed and then removed.
  */

void runE(){

  int mCount = 0;
  int j;
  srand (time(NULL));
  int random;
  
  
  struct Node nodes[100]; //housing a linked list without needing the actual malloc
  //For Loop that mallocs the entire block
  for(j = 0; j < 10; j++){ 
    //need 500 bytes allocated per loop, 460 bytes data, 10 4 byte META Data blocks...500 Bytes
    int k = j * 10;
    nodes[k].pointer = malloc(20);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(10);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(78);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(92);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(34);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(64);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(14);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(100);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(2);
    if(nodes[k].pointer != NULL)
      mCount++;
    k++;
    nodes[k].pointer = malloc(46);
    if(nodes[k].pointer != NULL)
      mCount++;
    
  }
  
  for(j = 0; j < 99; j++){
    nodes[j].nextNode = &nodes[j+1];
  }
  
  nodes[99].nextNode = NULL;
  
  struct Node * head = &nodes[0]; 
  struct Node * temp;
  struct Node * follower;
  
  int eCount; //count used to countdown the number of frees to be done
  
  for(eCount = 100; eCount > 0; eCount--){
    
    temp = head;
    random = rand() % eCount;
    if(random == 0){
      free((*head).pointer); //free the pointer
      head = (*head).nextNode; //move the head
    }else{
      for(j = 0; j < random; j++){
        follower = temp;
        temp = (*temp).nextNode;
      }
      free((*temp).pointer);
      (*follower).nextNode = (*temp).nextNode;
    }
    
  }
  /*
  mCount is the count of successful mallocs meant to track the initial 100 mallocs to ensure they're all successful
  prints the address and short value at that address of the beginning of myblock array that simulates memory. The value is 5000 if the memory all freed properly
  */
  printf("mCount = %d\n", mCount);
  printf("%p with size %d\n", blocks, *((short*)blocks));
  
}

  /*
		Memgrind scenario F
    Alternate allocating a bigger block (size 40 + 4 for META) and a minimum sized block until the memory is all taken. Then free all of the small
    blocks. A total of 600 Bytes are now free, try allocating 596 Bytes (600 including the META) and countdown, attempting to malloc 595, 594, 593,...
    until 1 byte is attempted. This shows how our Malloc and Free work with fragmented data. There is 600 Bytes total to work with, but it's in all 
    6 Byte blocks, so attempting to malloc more than 2 Bytes will not work.
	*/

void runF(){
  void * smallBlocks[100]; // 600 Bytes total
  void * largeBlocks[100]; //400 Bytes Meta...4000 Bytes to work with 100 blocks of 40 Bytes so 44 Bytes each after META
  int nullPtr = 0;
  int notNullPtr = 0; 
  int j;
  void * ptr;

  for(j = 0; j < 100; j++){
    //Large ones
    largeBlocks[j] = malloc(40); 
    //Small ones
    smallBlocks[j] = malloc(1);
  }

  //Freeing the small blocks
  for(j = 0; j < 100; j++){
    free(smallBlocks[j]); 
  }

  //Trying to allocate more data

  for(j = 596; j > 0; j--){
    ptr = malloc(j); 
    if(ptr != NULL){
      notNullPtr++; 
    }else{
      nullPtr++; 
    }
  }

  free(smallBlocks[0]);
  free(smallBlocks[1]);

  for(j = 0; j < 100; j++){
    free(largeBlocks[j]);
  }

  /*
  Successfully allocated (notNullPtr) is the count of successful mallocs during the latter allocation after freeing the small blocks. Only allocations small enough work at this point
  Failed to allocate (nullPtr) is the count of NULL pointers returned from mallocs during the latter allocation after freeing the small blocks. Although there is 600 Bytes technically available
  	only allocations of size 1 or 2 will work because of the extreme fragmentation.
  prints the address and short value at that address of the beginning of myblock array that simulates memory. The value is 5000 if the memory all freed properly
  */

  printf("Successfully allocated: %d\n", notNullPtr); 
  printf("Failed to allocate: %d\n", nullPtr);
  printf("%p with size %d\n", blocks, *((short*)blocks));

}