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

int search_TLB(TLBentry *TLBt,int pno);
void TLB_Add(TLBentry *TLBt,int pno,int fno);
void TLB_Update(TLBentry *TLBt,int pno,int newpage,int newframe);

int t = 0;

//Define the needed macros
#define OFFSET_MASK 255
#define PAGES 256
#define OFFSET_BITS 8
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define FRAME_COUNT 128
//#define INT_SIZE 4 // Size of integer in bytes
//#define INT_COUNT 16384
//#define MEMORY_SIZE INT_COUNT * INT_SIZE
#define MEMORY_SIZE 65536
#define PHYSICAL_SIZE 32768

char intArray[PHYSICAL_SIZE]; 
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
	for (int i =0;i<PAGES-1;i++){
		page_table[i] = -1;
	}
	int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);    
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
	FILE *fptr = fopen("addresses.txt", "r");
	TLBentry TLBt[16];
	unsigned int vaddr, pno,page_offset,framenumber,phyAddr; 
	char buff[BUFFER_SIZE];
	while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
		vaddr = atoi(buff);
		
		pno = vaddr >> OFFSET_BITS;
		page_offset = vaddr & OFFSET_MASK;
        flag = 0;
		//,phyAddr);
		framenumber = search_TLB(TLBt,pno);
		if (flag == 1){
			phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
			value = intArray[phyAddr];
            numhit++;
        }
		else{
			if (page_table[pno]==-1){
				//Page_Fault and read from Backing_Store.bin
                page_fault++;
                framenumber = mem_flag;
                memcpy(intArray+(framenumber*FRAME_SIZE), mmapfptr + (pno*PAGE_SIZE), FRAME_SIZE);
                mem_flag = (mem_flag+1)%FRAME_COUNT;
                page_table[pno]=framenumber;
				if (search_TLB(TLBt,pno)!=-1){
					TLB_Update(TLBt,pno,pno,framenumber);
				}
			}
			else{
				framenumber = page_table[pno];
				TLB_Add(TLBt,pno,framenumber);
			}
		}
		phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
		value = intArray[phyAddr];
		
		/*if (t >= 30){
			break;
		}
		t++;
		*/
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

void TLB_Add(TLBentry *TLBt,int pno,int fno){
    TLBt[TLB_flag].PageNum = pno;
 	TLBt[TLB_flag].FrameNum = fno;
	TLB_flag++;
	TLB_flag%=10;
	return;
}

void TLB_Update(TLBentry *TLBt,int pno,int newpage,int newframe){
    for (int i = 0; i < 10; i++) {
		if(TLBt[i].PageNum ==pno){
			TLBt[i].PageNum = newpage;
			TLBt[i].FrameNum = newframe;
		}
	}
	return;
}
