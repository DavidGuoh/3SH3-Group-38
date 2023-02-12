//#include <linux/init.h>
//#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/proc_fs.h>
//#include <asm/uaccess.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_LINE 80

int main(){
    char *args[MAX_LINE/2+1];
    char history[5][MAX_LINE/2+1];
    int history_count = 0;
    int should_run = 1;
    while (should_run){
        char cmd[20];
        printf("%s","osh>");
        fgets(cmd, 20, stdin);
        fflush(stdout);
        //printf("%s",cmd);
        int concurrent;
        //fflush(stdout);
        //printf("%s",checksymbol);
        if (cmd[strlen(cmd)-2] == '&'){
            concurrent = 1;
            cmd[strlen(cmd)-2] ='\0';
        }
        else{
            concurrent = 0;
            cmd[strlen(cmd)-1] = '\0';
        }
        
        //fflush(stdout);
        //free(checksymbol);
        //cmd[strlen(cmd)-1] = '\0';
        if(strcmp(cmd,"!!")==0 && history_count==0){
            printf("%s\n","No commands in history.");
        }
        else if(strcmp(cmd,"!!")==0 && history_count>0){
            strcpy(cmd,history[0]);
        }
        if (strcmp(cmd, "exit") == 0){
            should_run = 0;
        }
        if(strcmp(cmd,"history")==0 && history_count==0){
            printf("%s\n","No commands in history.");
        }
        else if(strcmp(cmd,"history")==0 && history_count!=0){
            for(int j =0;j<history_count;j++){
                printf("%d %s\n",history_count-j,history[j]);
            }
        }
        else{}
        
        for(int i = 4;i>0;i--){
                strcpy(history[i],history[i-1]);
            }
            strcpy(history[0],cmd);
            if (history_count<5){
                history_count++;
            }
            else{}
        args[0] = strtok(cmd," ");
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
        pid = fork();
        //char* command = "ls"; 
        //char* argument_list[] = {"ls","-l", NULL};
        if(pid == 0){
            execvp(args[0],args);
        }
        else if(pid < 0){
            printf("%s","Fork Failed");
        }
        else{
            if (concurrent == 0){
                wait(NULL);
            }
        }
    }
    return 0;
}
