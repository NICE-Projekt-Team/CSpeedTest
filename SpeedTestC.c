#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

unsigned char byteArray1[1024];
unsigned char byteArray2[1024];
unsigned char byteArray3[1024];


void writeArray(unsigned char byteArray[]) {
	for (int i = 0; i <= 1024; i++) {
		byteArray[i] = rand() % 2;
	}
}

unsigned char transfer(unsigned char input[], unsigned char transferInput[]) {
	//*transferInput = *input;
	for (int i = 0; i < 1024; i++) {
		transferInput[i] = input[i];
	}
	return *transferInput;
}

void *bA1(void *arg) {
	writeArray(byteArray1);
	return NULL;
}

void *bA2(void *arg) {
	writeArray(byteArray2);
	return NULL;
}

void *bA3(void *arg) {
	writeArray(byteArray3);
	return NULL;
}

void *tB1(void *arg) {
	transfer(byteArray1, byteArray2);
	return NULL;
}

void *tB2(void *arg) {
	transfer(byteArray2, byteArray3);
	return NULL;
}

void *tB3(void *arg) {
	transfer(byteArray3, byteArray1);
	return NULL;
}

int main()
{
	int size = sizeof(byteArray1);

	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	clock_t start;
	double cpu_time_used;

	start = clock();

	pthread_create(&t1, NULL, bA1, NULL);
	pthread_create(&t2, NULL, bA2, NULL);
	pthread_create(&t3, NULL, bA3, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	start = clock() - start;
	cpu_time_used = ((double)start)/ CLOCKS_PER_SEC;

	printf("Done writing arrays. Duration in seconds: %f", cpu_time_used);

	pthread_t t4;
	pthread_t t5;
	pthread_t t6;

	clock_t start2;
	double cpu_time_used2;

	start2 = clock();

	pthread_create(&t4, NULL, tB1, NULL);
	pthread_create(&t5, NULL, tB2, NULL);
	pthread_create(&t6, NULL, tB3, NULL);
		
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);
	pthread_join(t6, NULL);

	start2 = clock() - start2;
	cpu_time_used2 = ((double)start2) / CLOCKS_PER_SEC;
	double total_time = cpu_time_used + cpu_time_used2;

	printf("Done transferring arrays. Duration in seconds: %f", cpu_time_used2);
	printf("Total time needed in seconds: %f", total_time);
	printf("Speed in mBit/s: %d", ((size * 4 * 8)/ 60));

	return 0;
}
