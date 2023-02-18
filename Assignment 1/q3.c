#include <stdio.h>
#include  <sys/types.h> /* This header file has the definition for pid_t type*/
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

int chairs=3;//three chairs available at beginning

pthread_mutex_t office; //setting up the in office state
/* making threads */
pthread_t TA;
pthread_t student;

sem_t student_arrive;
sem_t TA_avail;
sem_t chair_update;

int rand_num(){ //random time generator function
    //max is 90 seconds, min is 1 sec of process time waiting
    int r = rand() % 90000000 ; //max amount of time is 9 seconds
    while (r < 10000000){ // one second minimum (in microsec)
        r = rand() % 90000000; //regenerate time delay until an appropiate time delay is made
    }
    return r;
}

void student_help(){
    sem_wait(&chair_update); //lock the semaphore
    if (chairs >= 1){ //checking if at least one chair
        chairs --;
        printf("Student[pid= %lu] is waiting.\n",pthread_self());
        sem_post(&student_arrive); //unlock semaphore
        printf("Chairs available: %d\n",chairs);
        sem_post(&chair_update); //unlock seat semaphore
        sem_wait(&TA_avail); //wait for avail TA
        printf("Student[pid= %lu] is in the office.\n",pthread_self());        
    } else{ //when no waiting chairs are available
        sem_post(&chair_update);
        printf("No chairs avail, student left");
    }
    pthread_exit(NULL);
};

void TA_help(){
    while (1){
        sem_wait(&student_arrive); //check on if theres a student, sleep if not
        sem_wait(&chair_update); //access chair modifier and update value to take 
        chairs = chairs +1;
        sem_post(&chair_update);//unlock
        sem_post(&TA_avail); //set TA to ready
        pthread_mutex_lock(&office); //lock office mutex for when help student
        usleep(rand_num()); //helping a student for a random amount of time
        pthread_mutex_unlock(&office); //unlock office mutex when done helping
        printf("Student leaving \n");
    }
    pthread_exit(NULL);
};

void spawn_student(){
    int temp; //temporary variable

    while (1){
        pthread_t student_active; // starting thread for when student needs help
        temp = pthread_create(&student_active,NULL,(void*)student_help, NULL); 
        // connecting active student thread with its function
        usleep(rand_num());//suspend calling next thread for amount of time
    }

};

int main() {	
    /* Initialization, should only be called once */
    srand(time(NULL));  

    int temp; //random temp variable for storing stuff

    /* Initialize mutex and semaphores */
    pthread_mutex_init(&office, NULL);
    sem_init(&TA_avail, 0, 0);
    sem_init(&student_arrive, 0, 0);
    sem_init(&chair_update, 0, 1); //1 indicates terminal is avail for writing
    
    temp = pthread_create(&TA, NULL, (void *)TA_help, NULL);  
	
    if (temp)
        printf("Failed to create thread."); 
    
    /* Create customer_maker thread */
    temp = pthread_create(&student, NULL, (void *)spawn_student, NULL);  
	
    if (temp)
        printf("Failed to create thread."); 
     
    /* Waiting for threads*/
    pthread_join(TA, NULL);
    pthread_join(student, NULL);
        	
};

