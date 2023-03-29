#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

#define BUFFER_SIZE 10
typedef struct TLBentry
{
	int PageNum;
	int FrameNum;
	/* data */
}TLBentry;

//Define the needed macros
#define OFFSET_MASK 255
#define PAGES 2^8
#define OFFSET_BITS 8
#define PAGE_SIZE 2^8
#define PAGE_SIZE 2^8
#define FRAME_SIZE 2^8

#define INT_SIZE 4 // Size of integer in bytes
#define INT_COUNT 16384
#define MEMORY_SIZE INT_COUNT * INT_SIZE
int intArray[MEMORY_SIZE]; 
signed char *mmapfptr;
int flag = 0;
int TLB_flag = 0;
int main(int argc, char *argv[]) {
	int page_table[PAGES];
	for (int i =0;i<PAGES-1;i++){
		page_table[i] = -1;
	}
	FILE *fptr = fopen("addresses.txt", "r");
	TLBentry TLBt[10];
	
	unsigned int vaddr, pno,page_offset,framenumber,phyAddr; 
	char buff[BUFFER_SIZE];
	//Read from labaddr.txt till you read end of file.
	while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
		vaddr = atoi(buff);
		pno = vaddr >> OFFSET_BITS;
		//framenumber = page_table[pno];
		page_offset = vaddr & OFFSET_MASK;
		//phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
		printf("%d, %d, %d\n",vaddr,pno,page_offset);//,phyAddr);
		for (int i = 0; i < 10; i++) {
			if(TLBt[i].PageNum ==pno){
				framenumber = TLBt[i].FrameNum;
				flag = 1;
			} 
		if (flag == 1){
			phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
		}
		else{
			if (page_table[pno]==-1){
				//Page_Fault and read from Backing_Store.bin
			}
			else{
				framenumber = page_table[pno];
				phyAddr = (framenumber<<OFFSET_BITS)|page_offset;

			}
		}
    }
	}
	fclose(fptr);
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);    
    //Use the mmap() system call to memory map numbers.bin file
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    
    //Use a for loop to retrieve the contents of the memory mapped file and store it in the integer array using memcpy() function.
    for(int i = 0; i < INT_COUNT; i ++){
        memcpy(intArray + i, mmapfptr + 4*i, INT_SIZE);
		printf("%d\n",intArray[i]);
    }
    munmap(mmapfptr, MEMORY_SIZE);
    return 0;
}

void search_TLB(TLBentry *TLBt,int *pno){
	int framenumber;
    for (int i = 0; i < 10; i++) {
		if(TLBt[i].PageNum ==pno){
			framenumber = TLBt[i].FrameNum;
			flag = 1;
			return framenumber;
		}
	}
	return -1;
}

void TLB_Add(TLBentry *TLBt,int *pno,int *fno){
    TLBt[TLB_flag].PageNum = pno;
 	TLBt[TLB_flag].FrameNum = fno;
	TLB_flag++;
	TLB_flag%=10;
	return;
}

void TLB_Update(TLBentry *TLBt,int *pno,int *newpage){
    for (int i = 0; i < 10; i++) {
		if(TLBt[i].PageNum ==pno){
			TLBt[i].PageNum = newpage;
		}
	}
}

void PAGE_FAULT(void){
    return;
}