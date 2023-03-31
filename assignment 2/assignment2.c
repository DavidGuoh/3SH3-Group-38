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
int TLB_Update(TLBentry *TLBt,int pno,int newpage,int newframe);
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
		framenumber = search_TLB(TLBt,pno);
		if (flag == 1){
			phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
			value = intArray[phyAddr];
            numhit++;
        }
		else{
			if (page_table[pno]==-1){
				if(page_fault>127){
					page_fault++;
                	framenumber = mem_flag;
                	memcpy(intArray+(framenumber*FRAME_SIZE), mmapfptr + (pno*PAGE_SIZE), FRAME_SIZE);
                	mem_flag = (mem_flag+1)%FRAME_COUNT;
					int oldpage = replace_Pagetable(page_table,framenumber);
                	page_table[pno]=framenumber;
					int isin = TLB_Update(TLBt,oldpage,pno,framenumber);
					if (isin==-1){
						TLB_Add(TLBt,pno,framenumber);
					}
				}
				else{
					page_fault++;
                	framenumber = mem_flag;
                	memcpy(intArray+(framenumber*FRAME_SIZE), mmapfptr + (pno*PAGE_SIZE), FRAME_SIZE);
                	mem_flag = (mem_flag+1)%FRAME_COUNT;
                	page_table[pno]=framenumber;
					TLB_Add(TLBt,pno,framenumber);
				}
			}
			else{
				framenumber = page_table[pno];
				TLB_Add(TLBt,pno,framenumber);
				//TLB_Update(TLBt,pno,pno,framenumber);
			}
		}
		phyAddr = (framenumber<<OFFSET_BITS)|page_offset;
		value = intArray[phyAddr];
        printf("%d, %d, %d\n",vaddr,phyAddr,value);
		if (vaddr == 28210){
			break;
		}
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
    for (int i = 0; i < 16; i++) {
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
	TLB_flag%=16;
	return;
}

int TLB_Update(TLBentry *TLBt,int oldpno,int newpno,int newframe){
    for (int i = 0; i < 16; i++) {
		if(TLBt[i].PageNum ==oldpno){
			TLBt[i].PageNum = newpno;
			TLBt[i].FrameNum = newframe;
			return 1;
		}
	}
	return -1;
}
int replace_Pagetable(int *page_table,int framenumber){
	for (int i=0;i<PAGES;i++){
		if (page_table[i]==framenumber){
			page_table[i] = -1;
			return i;
		}
	}
	return -1;
}
