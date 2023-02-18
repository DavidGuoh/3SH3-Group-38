#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_LINE 80 //define the max length of input command
void print_history(char history[][MAX_LINE/2+1],int history_count){
    for(int j =0;j<history_count;j++){
        printf("%d %s\n",j+1,history[j]);
    }
    return;
}
int main(){
    char *args[MAX_LINE/2+1];
    char history[5][MAX_LINE/2+1]; //Create array to store last 5 history
    int history_count = 0; //count the number of history in list
    int should_run = 1; //flag for continue running the program
    while (should_run==1){ 
        char input[20]; //store input command
        printf("%s","osh>");
        fgets(input, 20, stdin); //read input and store
        fflush(stdout); //flushes the output buffer of a stream
        //printf("%s",input);
        int concurrent; //flag for running concurrently or not
        //fflush(stdout);
        //printf("%s",checksymbol);
        if (input[strlen(input)-2] == '&'){ //detect symbol
            concurrent = 1;
            input[strlen(input)-2] ='\0'; //add end symbol at the end of input
        }
        else{
            concurrent = 0;
            input[strlen(input)-1] = '\0';
        }

        if (strcmp(input, "exit") == 0){ //exit the program if input is exit
            should_run = 0;
            exit(0);
            break;
        }
        //fflush(stdout);
        //free(checksymbol);
        //input[strlen(input)-1] = '\0';
        if(strcmp(input,"!!")==0 && history_count==0){ //error handling !! without history
            printf("%s\n","No commands in history.");
            fflush(stdout);
        }
        else if(strcmp(input,"!!")==0 && history_count>0){ 
            if (strcmp(history[0],"!!")==0){ //error handling !! call !!
                printf("%s\n","error: !! can't call !!");
                fflush(stdout);
                continue;
            }
            else{
                strcpy(input,history[0]); //execute the last input
            }
        }

        if(strcmp(input,"history")==0 && history_count==0){ //Error handling no history in history
            printf("%s\n","No commands in history.");
            fflush(stdout);
        }
        else if(strcmp(input,"history")==0 && history_count!=0){ //print history
            print_history(history,history_count);
            /*for(int j =0;j<history_count;j++){ //function
                printf("%d %s\n",j+1,history[j]);
            }*/
        }
        for(int i = 4;i>0;i--){  //store newest command in history
                strcpy(history[i],history[i-1]);
            }
            strcpy(history[0],input);
            if (history_count<5){
                history_count++;
            }
        if(strcmp(input,"!!")==0){ //error handling no !! should be executed by execvp
            continue;
        }
        args[0] = strtok(input," "); //clean input for execvp
        int i = 1;
        while(1){
            char* tok = strtok(NULL," ");
            if (tok == NULL){
                break;
            }
            args[i++] = tok;
            }
        args[i] = NULL;
        pid_t pid; 
        //int status;
        pid = fork(); // create child process
        int status; // specify status
        //char* command = "ls"; 
        //char* argument_list[] = {"ls","-l", NULL};
        if(pid == 0){ //Childs process run
            execvp(args[0],args); //run the command   
            if(strcmp(input,"history")==0 || strcmp(input, "exit") == 0 || strcmp(input,"!!")==0){}
            else{ //error handling if command doesn't work
                printf("%s","Not A Functional Command\n");
                fflush(stdout);
            }
        }
        else if(pid < 0){ //error if child process fail to create
            printf("%s","Fork Failed");
            fflush(stdout);
            should_run =0;
        }
        else{
            if (concurrent == 0){ //If parent run after child process finish
                wait(NULL);
            }
            else{ //If parent run at the same time as child process run
                waitpid(pid,&status,WUNTRACED);//WNOHANG);WUNTRACED
            }
        }
    }
    return 0;
}
