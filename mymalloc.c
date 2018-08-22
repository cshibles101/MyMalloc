/**
	Authors: Olga Bugrina (RUID 126-00-5883) and Christopher Shibles (RUID 156-00-0042)
**/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

static char myblock[5000]; //Simulated memory
int count = 0; //variable to tell whether or not any data has been malloc'd yet


struct Node{ //MOVE TO MEMGRIND

	void * pointer; 
	struct Node * nextNode; 

};

void * mymalloc(size_t size, char * fileString, int lineInt){ 

	if(count == 0){ // checking to see if memory has previously been malloc'd
    char * countptr = &myblock[0];
    *(short *)countptr = 5000;
    countptr = &myblock[2]; 
    *(short *)countptr = 0; 
    count++;
	}
  
	if(size <= 0){ 
		if(size == 0)
    	return NULL;
    //printf("Size given for malloc is negative, %d - %s\n", lineInt, fileString);
    return NULL;
    
	}

	int i = 0; 
	char * retAddress = NULL; 
	unsigned short blockSize = NULL;
	unsigned short newSize; 

	//Check if the requested size is an even number
	if(size & 1){
		size = size + 1;
	} 
  
	size = size + 4; //Adding 4 bytes for META data

	//Traversing the block
	while(i < 5000){ 
    
		char * ptr = &myblock[i]; 
		blockSize = *(short *) ptr;

    if(blockSize == 0){
    	printf("blocksize is 0, terminating\n");
      exit(0);
    }
    
		//Checking if the block is allocated
		if(blockSize & 1){
		i = i + blockSize;
		i = i & ~1; 
		} 
		else{
			//unallocated
			if(blockSize >= size){
				//can allocate
				retAddress = &myblock[i]; //assign the return pointer an address
				//printf("retAddress: %p\n", retAddress);   //DELETE
        
				newSize = blockSize - size; //calculate the size of the newly split block after the allocated block
				//printf("%d - %d = %d\n", blockSize, size, newSize);   //DELETE
        
				if(newSize < 4){ //the split will not even allow meta data to fit in the remaining blocks, add those blocks to the allocation
          
					size = size + newSize;
					newSize = blockSize;
          
				}
				else{
          
					char * p = &myblock[size + i]; //pointer to free blocks of the partition
					//printf("p = %p\n", p);   //DELETE
      
					*(short *)p = newSize; //setting newsize of partitioned free blocks
					//printf("newSize = %d\n", newSize);   //DELETE
          
					p = &myblock[i+size+2]; //pointing to next META Data
					//printf("p = %p\n", p);   //DELETE
        
					*(short *)p = size | 1; //setting META Data of new partition for previous size
					//printf("*p = %d\n", *p);   //DELETE
          
				}
    
        
				*(short *)retAddress = (size | 1); //assign the META Data  for the size of the block with the LSB as 1 to show allocated
				//printf("*retAddress: %d\n", *retAddress);   //DELETE
        
				char * q = retAddress; //creating pointer to increment to next block after current block (both parts of partition)
				q = q+blockSize+2;
				//printf("q: %p \n", q);   //DELETE
        
				if(boundaryCheck((void *)q)){
					*(short *)q = newSize;
          
				}  
			
				return retAddress;
			}else{
				i = i + blockSize;
			}
		}
	}
  //printf("Not enough contiguous space to malloc, %d - %s\n", lineInt, fileString);
	return NULL; 
}



// returns 0 
int myfree(void *ptr, char * fileString, int lineInt){ 
	//Error Checks
	if(ptr == NULL){ //null pointer
    printf("NULL Pointer cannot be freed, %d - %s\n", lineInt, fileString);
		return 1; 
	}
	if(!boundaryCheck(ptr)){ //pointer not in the bounds of the memory block
    printf("Pointer not within bounds, therefore was not malloc'd and cannot be freed, %d - %s\n", lineInt, fileString);
		return 1; 
	}
  
	//pointer is not null and is within the boundary
  
	unsigned short size = *(short *)ptr; //size of memory block stored in the META Data
  
	if(size & 1){ //checks for a one in the least significant bit to determine if the block is allocated
    
		//memory is allocated
		unsigned short prevSize = *((short *)ptr+1); //size of block before block to be freed
    //printf("prevSize = %d \n", prevSize);
  
		unsigned short afterSize = 0; //size of the block after block to be freed (if it exists)
    
		void * next = (char *)ptr + (size & ~1); //pointer to next block
    
		void * previous = NULL; //pointer to previous block
    
  
		if(prevSize > 0 && prevSize % 2 == 0){ //prevSize is 0 if there is no previous block
    	//printf("prevSize: %d\n", prevSize); //DELETE
			previous = (char *)ptr - prevSize; //previous is pointing at the start of the previous block's META Data
		}
    
 	 	if(boundaryCheck(next)){ //checks to see if the block to free is at the end of the memory
			afterSize = *(short *)next;
      if(afterSize & 1){
        next = NULL; //sets next to null if next is allocated 
        //printf("afterSize: %d\n", afterSize);  //DELETE
      }
		}
		else{
			next = NULL; //set next to null if it's not within the boundaries
      //printf("LINE 148\n");   //DELETE
		}
    
		// previous will be null if there is no previous and next will be null if there is no next
		// prevSize will be 0 if there is no previous and afterSize will be 0 if there is no next
		// * next is pointing to the size META data of the next block (if it exists)
		// * previous is pointing to the size META data of the previous block (if it exists)
		// size is the size of the block to be freed
    
		if(previous != NULL && next != NULL){	//there is a free block before and after the block to be freed
      //combine the 3 sizes into one and put it where it belongs
     	//printf("First IF\n"); //DELETE
      unsigned short combined = (size & ~1) + prevSize + afterSize; //combined size of the new larger freed memory
      
      *(short *)previous = combined; //assigning the combined size to the META Data of the new large block
      
      *(short *)next = 0;
      *((short *)next+2) = 0;
      *(short *)ptr = 0;
   		*((short *)ptr+2) = 0;
      
      //checking for a next next
      next = next + afterSize;
      if(boundaryCheck(next)){ //is there a next next block in the memory
        next = ((char *)next+2);
        *(short *)next = combined; //assigning the new previous META Data to the next next block
      }
      
      
      
		}
		else if(previous != NULL){	//there is a free block before, but NOT after the block to be freed
      //printf("Second IF\n");   //DELETE
      unsigned short combined = (size & ~1) + prevSize; //combined size of the new larger freed memory
      
      *(short *)previous = combined; //assigning the combined size to the META Data of the new large block
      
      *(short *)ptr = 0; 
      *((short *)ptr+2) = 0; 
      
      next = (char *)ptr + (size & ~1) + 2;
      
      if(boundaryCheck(next)){
      	*(short *)next = combined;
      }
      
      
      
		}
		else if(next != NULL){	//there is a free block after, but NOT before the block to be freed
      //printf("Third IF\n");   //DELETE
      unsigned short combined = (size & ~1) + afterSize; //combined size of the new larger freed memory
      
      *(short *)ptr = combined; //assigning the combined size to the META Data of the new large block
      
      *(short *)next = 0; 
      *((short *)next+2) = 0; 
      
      //checking for a next next
      next = next + afterSize;
      if(boundaryCheck(next)){ //is there a next next block in the memory
        next = (char *)next+2;
        *(short *)next = combined; //assigning the new previous META Data to the next next block
      }
      
      
		}
		else{	//there are no free blocks after or before
     	//printf("Else\n");   //DELETE
      size = (*(short *)ptr & ~1);//making the size even 
      //printf("size: %d\n", size); //DELETE
      *(short *)ptr = size;
      //check for a next
      next = (char *)ptr + size + 2;
      if(boundaryCheck(next)){
        //printf("Next Prev Size changed \n"); //DELETE
      	*(short *)next = size;
      }
      
      
		}
	}else{
	  //Memory is not allocated
    printf("Memory at location is not currently allocated, %d - %s\n", lineInt, fileString);
	  return 1; 
	}
  return 0;
}


/**
	Simple helper function that checks if the address a pointer points to is in the range of the memory block
  @param void pointer.
  @return 1 if the pointer is within the range of the array's addresses and 0 if not.
**/
int boundaryCheck(void * ptr){

  if((char *)ptr < &myblock[0] || (char *)ptr > &myblock[4999]){
    return 0; 
  }else{
    return 1;
  }
}