
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define NUMTHREADS 200
#define MAXCNT 10000

/* Global variables - shared between threads */
double counter = 0;
sem_t sem;

/* Declaring functions*/
void* counting(void *);

int main(void) {
  pthread_t tid[NUMTHREADS];
  int i=0;

  /* Semaphore init*/
  sem_init(&sem,0,1);

  for( i=0; i<NUMTHREADS; i++){
     pthread_create(&tid[i], NULL, &counting, NULL);
  }

  for( i=0; i< NUMTHREADS; i++){
     pthread_join(tid[i], NULL);
  }
  /* Semaphore destroy*/
  sem_destroy(&sem);
  printf("\nCounter must be in: %d\n", MAXCNT*NUMTHREADS);
  printf("\nCounter value is: %.0f\n\n", counter);


  return 0;
}

/* Function Thread*/
void* counting(void * unused) {
  int i=0;

  
  for(i=0; i<MAXCNT; i++){
     sem_wait(&sem);
     counter++;
     sem_post(&sem);
  }

  return NULL;
}
