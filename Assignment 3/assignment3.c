#include <stdio.h>
#include <sys/mman.h> /*For mmap() function*/
#include <string.h> /*For memcpy function*/
#include <fcntl.h> /*For file descriptors*/
#include <stdlib.h> /*For file descriptors*/

#define INT_SIZE 4 // Size of integer in bytes
#define INT_COUNT 20 // ?
#define MEMORY_SIZE INT_COUNT * INT_SIZE
int intArray[MEMORY_SIZE]; 

signed char *mmapfptr;
void bubble_sort(int arr[], int len);
int stringsearch(int arr[], int len,int value);
int repeat = 0;
int main(int argc, char *argv[])
{   
    int sum = 0;
    int result[MEMORY_SIZE+1] ={0};
    int mmapfile_fd = open("request.bin", O_RDONLY); 
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    printf("Total requests = 20\n");
    if(atoi(argv[1])>0 && atoi(argv[1])<=299 || strcmp(argv[1],"0")==0){
        printf("Initial Head Position: %d\n",atoi(argv[1]));
    }
    else{
        printf("INPUT ERROR\n");
        return -1;
    }
    if(strcmp(argv[2],"RIGHT")==0 || strcmp(argv[2],"LEFT")==0){
        printf("Direction of Head: %s\n",argv[2]); 
    }
    else{
        printf("INPUT ERROR\n");
        return -1;
    }
    result[0] = atoi(argv[1]);
    for(int i = 0; i < INT_COUNT; i ++){
        memcpy(intArray + i, mmapfptr + 4*i, INT_SIZE);
    }
    printf("FCFS DISK SCHEDULING ALGORITHM:\n");
    for(int i = 0; i < INT_COUNT; i ++){
        result[i+1] = intArray[i];
        printf("%d, ",result[i+1]);
    }
    printf("\n");
    for(int i = 0; i < INT_COUNT; i ++){
        sum = sum + abs(result[i]-result[i+1]);
    }
    printf("FCFS - Total head movements = %d\n",sum);






    printf("SSTF DISK SCHEDULING ALGORITHM:\n");
    sum = 0;
    int array1[INT_COUNT+1] ={0};
    int res1[INT_COUNT] ={0};
    array1[0] = atoi(argv[1]);
    for(int i = 0; i < INT_COUNT; i ++){
        array1[i+1] = intArray[i];
    }    
    bubble_sort(array1,INT_COUNT+1);
    int index = stringsearch(array1, INT_COUNT+1,atoi(argv[1]));
    int a,b;
    if(atoi(argv[1]) == array1[0]){
        a = 0;
    }
    else{
        a = index-1;
    }
    if(atoi(argv[1]) == array1[INT_COUNT]){
        b = INT_COUNT;
    }
    else{
        b = index+1;
    }
    
    int count = 0;
    while(a!=0 || b!=INT_COUNT){
        if (a==0){
            res1[count] = array1[b];
            index = b;
            count++;
            b++;
            if (b==INT_COUNT){
                    res1[count] = array1[b];
                    index = b;
                    count++;
                    break;
                }
        }
        else if (b==INT_COUNT){
            res1[count] = array1[a];
            index = a;
            count++;
            a--;
            if (a==0){
                    res1[count] = array1[a];
                    index = a;
                    count++;
                    break;
                }
        }
        else{
            if(abs(array1[index]-array1[a])>abs(array1[index]-array1[b])){
                res1[count] = array1[b];
                index = b;
                count++;
                b++;
                if (b==INT_COUNT && abs(array1[index]-array1[a])>abs(array1[index]-array1[b])){
                    res1[count] = array1[b];
                    index = b;
                    count++;
                }
            }
            else{
                res1[count] = array1[a];
                index = a;
                count++;
                a--;
                if (a==0 && abs(array1[index]-array1[a])<abs(array1[index]-array1[b])){
                    res1[count] = array1[a];
                    index = a;
                    count++;
                }
            }
        }
    }
    for(int i = 0; i < INT_COUNT; i ++){
        printf("%d, ",res1[i]);
    }  
    printf("\n");
    
    for(int i = 0; i < INT_COUNT-1; i ++){
        sum = sum + abs(res1[i]-res1[i+1]);
    }
    printf("SSTF - Total head movements = %d\n",sum);


    printf("SCAN DISK SCHEDULING ALGORITHM\n");
    sum = 0;
    int array2[INT_COUNT+1] ={0};
    int res2[INT_COUNT+1] ={0};
    array2[0] = atoi(argv[1]);
    for(int i = 0; i < INT_COUNT; i ++){
        array2[i+1] = intArray[i];
    }    
    bubble_sort(array2,INT_COUNT+1);
    repeat=0;
    index = stringsearch(array2, INT_COUNT+1,atoi(argv[1]));
    if (strcmp(argv[2],"LEFT")==0){
        if (repeat==1){
            for (int i =index;i>=0;i--){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            for (int i =index+2;i<INT_COUNT+1;i++){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            printf("\n");
            printf("SCAN DISK - Total head movements = %d\n",abs(atoi(argv[1])-0)+abs(0-res2[INT_COUNT]));
        }
        else{
            for (int i =index-1;i>=0;i--){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            for (int i =index+1;i<INT_COUNT+1;i++){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            printf("\n");
            printf("SCAN DISK - Total head movements = %d\n",abs(atoi(argv[1])-0)+abs(0-res2[INT_COUNT]));
        }
        
    }
    else if (strcmp(argv[2],"RIGHT")==0){
        if (repeat==1){
            for (int i =index+1;i<INT_COUNT+1;i++){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            for (int i =index-1;i>=0;i--){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            printf("\n");
            printf("SCAN DISK - Total head movements = %d\n",abs(299-res2[0])+abs(299-atoi(argv[1])));
        }
        else{
            for (int i =index+1;i<INT_COUNT+1;i++){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            for (int i =index-1;i>=0;i--){
                res2[i] = array2[i];
                printf("%d, ",res2[i]);
            }
            printf("\n");
            printf("SCAN DISK - Total head movements = %d\n",abs(299-res2[0])+abs(299-atoi(argv[1])));
        }
        
    }
    else{
        printf("Error, Direction should be LEFT or RIGHT");
    }


    printf("C-SCAN DISK SCHEDULING ALGORITHM\n");
    sum = 0;
    int array4[INT_COUNT+1] ={0};
    int res4[INT_COUNT+1] ={0};
    array4[0] = atoi(argv[1]);
    for(int i = 0; i < INT_COUNT; i ++){
        array4[i+1] = intArray[i];
    }    
    bubble_sort(array4,INT_COUNT+1); //sort items in bin in order
    repeat=0;
    index = stringsearch(array4, INT_COUNT+1,atoi(argv[1]));
    if (strcmp(argv[2],"LEFT")==0){ //left scanning
        if (repeat==1){
            for (int i =index;i>=0;i--){ //start at initial head position and decrement until zero
                res4[i] = array4[i];
                printf("%d, ",res4[i]);
            }
            for (int i =INT_COUNT;i>index+1;i--){ //since wraps around, go to max and decrement until one above initial
                res4[i] = array4[i];
                printf("%d, ",res4[i]);
            }
            printf("\n");
            printf("C-SCAN DISK - Total head movements = %d\n",abs(atoi(argv[1])-299)+abs(0-array4[index+1]));
        }
        
    }
    else if (strcmp(argv[2],"RIGHT")==0){ //right scanning
        if (repeat==1){
            for (int i =index+1;i<INT_COUNT+1;i++){ // start at initial head position and increment until max
                res4[i] = array4[i];
                printf("%d, ",res4[i]);
            }
            for (int i = 0;i<index;i++){ //wrap around to zero and increment to meet in middle
                res4[i] = array4[i];
                printf("%d, ",res4[i]);
            }
            printf("\n");
            printf("C-SCAN DISK - Total head movements = %d\n",abs(atoi(argv[1])-299)+abs(0-res4[0]));
        }
        
    }
    else{
        printf("Error, Direction should be LEFT or RIGHT");
    }

    printf("LOOK DISK SCHEDULING ALGORITHM\n");
    sum = 0;
    int array3[INT_COUNT+1] ={0};
    int res3[INT_COUNT+1] ={0};
    array3[0] = atoi(argv[1]);
    for(int i = 0; i < INT_COUNT; i ++){
        array3[i+1] = intArray[i];
    }    
    bubble_sort(array3,INT_COUNT+1);
    repeat=0;
    index = stringsearch(array3, INT_COUNT+1,atoi(argv[1]));
    if (strcmp(argv[2],"LEFT")==0){ //left scanning
        if (repeat==1){
            for (int i =index;i>=0;i--){ //start at initial head position and decrement to 0
                res3[i] = array3[i];
                printf("%d, ",res3[i]);
            }
            int end1 = res3[0];
            for (int i =index+2;i<INT_COUNT+1;i++){ //go back to initial and increment to end
                res3[i] = array3[i];
                printf("%d, ",res3[i]);
            }
            printf("\n");
            printf("LOOK DISK - Total head movements = %d\n",abs(atoi(argv[1])-end1)+abs(res3[0]-res3[INT_COUNT-1]));
        }
        
    }
    else if (strcmp(argv[2],"RIGHT")==0){ //right scanning
        if (repeat==1){
            for (int i =index+1;i<INT_COUNT+1;i++){ //start at initial position and increment to end
                res3[i] = array3[i];
                printf("%d, ",res3[i]);
            }
            for (int i =index -1;i>=0;i--){ //go back to initial and decrement to zero
                res3[i] = array3[i];
                printf("%d, ",res3[i]);
            }
            printf("\n");
            printf("LOOK DISK - Total head movements = %d\n",abs(atoi(argv[1])-299)+abs(atoi(argv[1])-0));
        }
        
    }
    else{
        printf("Error, Direction should be LEFT or RIGHT");
    }
    
    printf("C-LOOK DISK SCHEDULING ALGORITHM\n");
    sum = 0;
    int array5[INT_COUNT+1] ={0};
    int res5[INT_COUNT+1] ={0};
    array5[0] = atoi(argv[1]);
    for(int i = 0; i < INT_COUNT; i ++){
        array5[i+1] = intArray[i];
    }    
    bubble_sort(array5,INT_COUNT+1); //sort items in bin in order
    repeat=0;
    index = stringsearch(array5, INT_COUNT+1,atoi(argv[1]));
    if (strcmp(argv[2],"LEFT")==0){ //left scanning
        if (repeat==1){
            for (int i =index;i>=0;i--){ //start at initial head position and decrement until zero
                res5[i] = array5[i];
                printf("%d, ",res5[i]);
            }
            for (int i =INT_COUNT;i>index+1;i--){ //since wraps around, go to max and decrement until one above initial
                res5[i] = array5[i];
                printf("%d, ",res5[i]);
            }
            printf("\n");
            printf("C-LOOK DISK - Total head movements = %d\n",abs(atoi(argv[1])-299)+abs(0-array5[index+1]));
        }
        
    }
    else if (strcmp(argv[2],"RIGHT")==0){ //right scanning
        if (repeat==1){
            for (int i =index+1;i<INT_COUNT+1;i++){ // start at initial head position and increment until max
                res5[i] = array5[i];
                printf("%d, ",res5[i]);
            }
            int end1 = res5[INT_COUNT];
            for (int i = 0;i<index;i++){ //wrap around to zero and increment to meet in middle
                res5[i] = array5[i];
                printf("%d, ",res5[i]);
            }
            int end2 = res5[index-1];
            printf("\n");
            printf("C-LOOK DISK - Total head movements = %d\n",abs(atoi(argv[1])-end1)+abs(end1-299)+ abs(0-res5[0])+abs(res5[0]-end2));
        }
        
    }
    else{
        printf("Error, Direction should be LEFT or RIGHT");
    }

    munmap(mmapfptr, MEMORY_SIZE);
    return 0;
}

void bubble_sort(int arr[], int len) {
        int i, j, temp;
        for (i = 0; i < len - 1; i++){
                for (j = 0; j < len - 1-i; j++){
                        if (arr[j]>arr[j+1]){
                            temp = arr[j];
                            arr[j] = arr[j + 1];
                            arr[j + 1] = temp;
                        }
                }
        }
}
//SEARCHING FOR HEAD VALUES
int stringsearch(int arr[], int len,int value){
    int find = 0;
    for (int i = 0; i < len - 1; i++){
        if(arr[i]==value){
            find = i;
            if(arr[i+1]==value){
                repeat++;
            }
            return find;
        }
    }
    return -1;
}
