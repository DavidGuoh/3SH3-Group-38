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
    return 0;



	return 0;
}

void search_TLB(void){
    return;
}

void TLB_Add(void){
    return;
}

void TLB_Update(void){
    return;
}

void PAGE_FAULT(void){
    return;
}