#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

#define BUFFER_SIZE 10

//Define the needed macros
#define OFFSET_MASK 255
#define PAGES 2^8
#define OFFSET_BITS 8
#define PAGE_SIZE 2^8

#define INT_SIZE 4 // Size of integer in bytes
#define INT_COUNT 16384
#define MEMORY_SIZE INT_COUNT * INT_SIZE
int intArray[MEMORY_SIZE]; 
signed char *mmapfptr;

//defining TLBentry struct
typedef struct 
{
    int pg_num;
    int frame_num;
} TLBentry;


int main(int argc, char *argv[]) {

	//int page_table[PAGES] = {6,4,3,7,0,1,2,5};
	FILE *fptr = fopen("addresses.txt", "r");
	unsigned int vaddr, pno,page_offset;//, phyAddr, framenumber
	char buff[BUFFER_SIZE];
	//Read from labaddr.txt till you read end of file.
	while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
		vaddr = atoi(buff);
		pno = vaddr >> OFFSET_BITS;
		//framenumber = page_table[pno];
		page_offset = vaddr & OFFSET_MASK;
		//phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
		printf("%d, %d, %d\n",vaddr,pno,page_offset);//,phyAddr);
	}
	fclose(fptr);

    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);    
    //Use the mmap() system call to memory map numbers.bin file
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    
    //Use a for loop to retrieve the contents of the memory mapped file and store it in the integer array using memcpy() function.
    for(int i = 0; i < INT_COUNT; i ++){
        memcpy(intArray + i, mmapfptr + 4*i, INT_SIZE);
    }
    munmap(mmapfptr, MEMORY_SIZE);

    //CALLING SEARCH_TLB FUNCTION
    TLBentry tlb[MEMORY_SIZE]; //INITIALIZE TLB variable
    int TLB_first = 0; //required to count for FIFO policy, keeps track for adding

    // calling functions
    int frame = search_TLB(tlb,page);
    //error checking if frame is less than 1
    if (frame < 0 ){
        printf("ERROR FRAME NUMBER DOES NOT EXIST");
    }

    return 0;
}

int search_TLB(struct TLBentry tlb, int page){ //page is number that you're checking for
    int frame = -1 ; //initially set frame num to -1 to return in case no value is found
    
    for (int i=0; i<MEMORY_SIZE; i++){ //iterate through to find corresponding pg num
        if (tlb[i].pg_num == page){
            frame_error = tlb[i].frame_num;
            break; // if the number is found, exit the loop and return the found frame
        }
    }

    return frame;
}

void TLB_Add(struct TLBentry tlb, int index, int page, int frame){
    //reassigning
    tlb[index].frame_num = frame;
    tlb[index].pg_num = page;
    index = (index+1) % MEMORY_SIZE; //Shifts the index to account for first in first out
}

void TLB_Update(struct TLBentry tlb, int size, int page, int frame){
    //search for if the page number is found, then update frame num
    bool found = false; //variable to 

    for (int i = 0; i<MEMORY_SIZE;i++){
        if(tlb[i].pg_num == page ){
            tlb[i].frame_num=frame;
            found = true;
        }
    }

    if(!found){
        //keeping track of s
        if (size==MEMORY_SIZE){
            // replace if the table is full
            for(int i = 0; i<MEMORY_SIZE-1;i++){
                tlb[i] = tlb[i+1];
            } //FIFO take the first in and swap
            tlb[MEMORY_SIZE - 1].pg_num = page;
            tlb[MEMORY_SIZE - 1].frame_num = frame;
        } else{ //keeping 
            tlb[size].pg_num = page;
            tlb[size].frame_num = frame;
            size = size + 1;
        }
    } 
}

void PAGE_FAULT(void){
    return;
}

