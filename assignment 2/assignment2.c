#include <stdio.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

#define BUFFER_SIZE 10
//initializing struct to store TLB entries
typedef struct TLBentry
{
	int PageNum;
	int FrameNum;
	/* data */
}TLBentry;

//initializing functions
int search_TLB(TLBentry *TLBt,int pno);
void TLB_Add(TLBentry *TLBt,int pno,int fno);
int TLB_Update(TLBentry *TLBt,int oldframe,int newpno);
int replace_Pagetable(int *page_table,int framenumber);

//Define the needed macros
#define OFFSET_MASK 255
#define OFFSET_BITS 8
#define PAGES 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define FRAME_COUNT 128
#define MEMORY_SIZE 65536
#define PHYSICAL_SIZE 32768
char intArray[PHYSICAL_SIZE] = {-1}; 
signed char *mmapfptr;
int flag = 0;
int TLB_flag = 0;
int mem_flag = 0;
int page_fault = 0;
int numhit = 0;
int value = 0;
int total_address = 0;

int main(void) {
	int page_table[PAGES];
	for (int i =0;i<PAGES;i++){
		page_table[i] = -1;
	}

	//opening backing store file and mapping to memory region

	int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);    
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
	// creating a pointer to store the data in the addresses file
	FILE *fptr = fopen("addresses.txt", "r");
	TLBentry TLBt[16];
	unsigned int vaddr, pno,page_offset,framenumber,phyAddr; 
	char buff[BUFFER_SIZE];

	while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
		vaddr = atoi(buff);
		//using bitwise operators to access information in file and storing pno and offset data
		pno = vaddr >> OFFSET_BITS;
		page_offset = vaddr & OFFSET_MASK;
		framenumber = search_TLB(TLBt,pno);
		if (flag == 1){
            numhit++;
        }
		else{
			if (page_table[pno]==-1){
				//checking for page fault if size exceeds 128
				if(page_fault>127){
					printf("%d\n",1);
					page_fault++;
                	framenumber = mem_flag; //set frame number to be flagged
                	memcpy(intArray+(framenumber*FRAME_SIZE), mmapfptr + (pno*PAGE_SIZE), FRAME_SIZE);
                	mem_flag = (mem_flag+1)%FRAME_COUNT;
					int oldpage = replace_Pagetable(page_table,framenumber); //if size exceeds, page swap
                	page_table[pno]=framenumber; //set to set value
					int isin = TLB_Update(TLBt,framenumber,pno); //update page value
					if (isin==-1){
						TLB_Add(TLBt,pno,framenumber);
					}
				}
				else{ //accounting for pg fault and adding page
					page_fault++;
                	framenumber = mem_flag;
					//verifying memory map
                	memcpy(intArray+(framenumber*FRAME_SIZE), mmapfptr + (pno*PAGE_SIZE), FRAME_SIZE);
                	mem_flag = (mem_flag+1)%FRAME_COUNT;
                	page_table[pno]=framenumber;
					TLB_Add(TLBt,pno,framenumber);
				}
			}
			else{ //adding necessary page
				framenumber = page_table[pno];
				TLB_Add(TLBt,pno,framenumber);
			}
		}		
		//updating physical address and value to be printed
		phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
		value = intArray[phyAddr];
        
		printf("%d, %d, %d\n",vaddr,phyAddr,value);
		total_address++;
    }
	printf("Total Address: %d\n",total_address);
	printf("Page Fault %d\n",page_fault);
	printf("TLB HIT: %d\n",numhit);
    fclose(fptr);
    munmap(mmapfptr, PHYSICAL_SIZE);
    return 0;
}

int search_TLB(TLBentry *TLBt,int pno){
	int framenumber = 0;
	flag = 0;
	//iterate through the TLB until a page number is found
    for (int i = 0; i < 16; i++) {
		if(TLBt[i].PageNum ==pno){
			framenumber = TLBt[i].FrameNum;
			flag = 1;
			return framenumber; //framenumber is returned
		}
	}
	return -1; //if no page is found, frame number is -1
}

void TLB_Add(TLBentry *TLBt,int pno,int fno){
	//adding new page 
    TLBt[TLB_flag].PageNum = pno;
 	TLBt[TLB_flag].FrameNum = fno;
	TLB_flag++;
	TLB_flag%=16;
	return;
}

int TLB_Update(TLBentry *TLBt,int oldframe,int newpno){
	//updating oldframe based on frame number based with new pg number
    for (int i = 0; i < 16; i++) {
		if(TLBt[i].FrameNum ==oldframe){
			TLBt[i].PageNum = newpno;
			return 1;
		}
	}
	return -1;
}

//page swapping function
int replace_Pagetable(int *page_table,int framenumber){
	for (int i=0;i<PAGES;i++){
		if (page_table[i]==framenumber){
			page_table[i] = -1; //returns the index if frame number matches page_table at index to determine correct swap value
			return i;
		}
	}
	return -1;
}