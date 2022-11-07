#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Data for array sectioning array
typedef struct ArraySectionData{
    int *arr;
    int start_pos;
    int stop_pos;
    long resultSum;
}ArraySectionData;

// Read the array lenght from user and returns it
void ReadData(int *len, int *threadsNr);
// Generate random dynamic array of lenght len and returns it
int *GenRandArr(int len);
// Fill ArraySectionData structure
void FillArraySectionData(ArraySectionData *data, int *arr, int start_pos, int stop_pos);
// Simulate complex algorithm
int StupidReturn(int x);
// Calculate the sum of the array section
void *SectionSum(void *data);
// Thread Test Core 
void ThreadTest(int *arr, int len, int threadsNr);
// No Thread Test Core 
void NoThreadTest(int *arr, int len);

int main()
{   
    time_t threadTime;
    time_t noThreadTime;
    int *arr;
    int len;
    int threadsNr;
    ReadData(&len, &threadsNr);
    arr = GenRandArr(len);

    // Thread Test
    printf("==================\n");
    threadTime = time(NULL);
    ThreadTest(arr, len, threadsNr);   
    threadTime = time(NULL) - threadTime;
    printf("-> %d THREADS EXECUTION TIME: %lds\n", threadsNr, threadTime);
    printf("==================\n");
    // No Thread Test
    noThreadTime = time(NULL);
    NoThreadTest(arr, len);   
    noThreadTime = time(NULL) - noThreadTime;
    printf("-> 1 THREAD EXECUTION TIME: %lds\n", noThreadTime);
    printf("==================\n");

    free(arr);
    exit(0);
}

void ReadData(int *len, int *threadsNr){
    printf("Enter array lenght: ");
    scanf("%d", len);
    printf("Enter number of threads: ");
    scanf("%d", threadsNr);

}

int *GenRandArr(int len){
    int *arr = (int*)malloc(sizeof(int) * len);
    srand(time(NULL));
    for(int i = 0; i < len; i++)
        arr[i] = rand() % 100;

    return arr;
}

void FillArraySectionData(ArraySectionData *data, int *arr, int start_pos, int stop_pos){
    (*data).arr = arr;
    (*data).start_pos = start_pos;
    (*data).stop_pos = stop_pos;
    (*data).resultSum = 0;
}

int StupidReturn(int x){
    int decoySum = 0;
    for(int i = 0; i < 100000; i++)
        decoySum++;
    return x;
}

void *SectionSum(void *data)
{   
    for(int i = ((ArraySectionData*)data)->start_pos; 
        i <= ((ArraySectionData*)data)->stop_pos; i++)
        ((ArraySectionData*)data)->resultSum += StupidReturn(((ArraySectionData*)data)->arr[i]);
    
    return NULL;
}

void ThreadTest(int *arr, int len, int threadsNr){
    
    pthread_t *threads;
    ArraySectionData *section;
    long sum = 0;
    int part = len / threadsNr;

    threads = (pthread_t*)(malloc(sizeof(pthread_t) * threadsNr));
    section = (ArraySectionData*)(malloc(sizeof(ArraySectionData) * threadsNr));
    
    for(int i = 0; i < threadsNr - 1; i++)
        FillArraySectionData(&(section[i]), arr, i * part, (i + 1) * part - 1);
    FillArraySectionData(&(section[threadsNr - 1]), arr, (threadsNr - 1) * part, len - 1);

    for(int i = 0; i < threadsNr; i++)
        pthread_create(&(threads[i]), NULL, SectionSum, &(section[i]));
    for(int i = 0; i < threadsNr; i++)
        pthread_join(threads[i], NULL);
    for(int i = 0; i < threadsNr; i++)
        sum += section[i].resultSum;            

    printf("Sum = %ld\n", sum);
    free(section);
    free(threads);
}

void NoThreadTest(int *arr, int len){
    ArraySectionData section;
    FillArraySectionData(&section, arr, 0, len - 1);
    SectionSum(&section);
    printf("Sum = %ld\n", section.resultSum);
}
