#include<stdio.h> 
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t readCountAccess;
sem_t databaseAccess;
long readCount = 0;

void * Reader(void * arg);
void * Writer(void * arg);

int main() {
   long i = 0, number_reader = 0, numberofwriterthread;
   sem_init( &readCountAccess, 0, 1);
   sem_init( &databaseAccess, 0, 1);

   pthread_t Readers_thr[100], Writer_thr[100];
   printf("\nEnter number of Readers thread(MAX 10)");
   scanf("%ld", &number_reader);
   printf("\nEnter number of Writers thread(MAX 10)");
   scanf("%ld", &numberofwriterthread);

   for (i = 0; i < number_reader; i++) {
     pthread_create( &Readers_thr[i], NULL, Reader, (void * ) i);
   }
   for (i = 0; i < numberofwriterthread; i++) {
     pthread_create( &Writer_thr[i], NULL, Writer, (void * ) i);
   }
   for (i = 0; i < numberofwriterthread; i++) {
     pthread_join(Writer_thr[i], NULL);
   }

   for (i = 0; i < number_reader; i++) {
     pthread_join(Readers_thr[i], NULL);
   }
   sem_destroy( &databaseAccess);
   sem_destroy( &readCountAccess);
   return 0;
}

void * Writer(void * arg) {
   sleep(1);
   long temp = (long) arg;
   printf("\nWriter %ld is trying to enter into database for modifying the data", temp);
   sem_wait( &databaseAccess);
   printf("\nWriter %ld is writing into the database", temp);
   printf("\nWriter %ld is leaving the database", temp);
   sem_post( &databaseAccess);
}

void * Reader(void * arg) {
   sleep(1);
   long temp = (long) arg;
   printf("\nReader %ld is trying to enter into the Database for reading the data", temp);
   sem_wait( &readCountAccess);
   readCount++;
   if (readCount == 1) {
     sem_wait( &databaseAccess);
     printf("\nReader %ld is reading the database", temp);
   }
   sem_post( &readCountAccess);
   sem_wait( &readCountAccess);
   readCount--;
   if(readCount == 0){
     printf("\nReader %ld is leaving the database", temp);
     sem_post( &databaseAccess);
   }
   sem_post( &readCountAccess);
}
