#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

// Data for array sectioning array
typedef struct ArraySectionData{
    int *arr;
    int start_pos;
    int stop_pos;
    long resultSum;
}ArraySectionData;

// Read the array lenght from user and returns it
int ReadArrLen();
// Generate random dynamic array of lenght len and returns it
int *GenRandArr(int len);
// Fill ArraySectionData structure
void FillArraySectionData(ArraySectionData *data, int *arr, int start_pos, int stop_pos);
// Simulate complex algorithm
int StupidReturn(int x);
// Calculate the sum of the array section
void *SectionSum(void *data);
// Thread Test Core 
void ThreadTest(int *arr, int len);
// No Thread Test Core 
void NoThreadTest(int *arr, int len);

int main()
{   
    time_t threadTime;
    time_t noThreadTime;
    int *arr;
    int len;
    len = ReadArrLen();
    arr = GenRandArr(len);

    // Thread Test
    threadTime = time(NULL);
    ThreadTest(arr, len);   
    threadTime = time(NULL) - threadTime;
    printf("EXECUTION TIME: %lds\n", threadTime);
    // No Thread Test
    noThreadTime = time(NULL);
    NoThreadTest(arr, len);   
    noThreadTime = time(NULL) - noThreadTime;
    printf("EXECUTION TIME: %lds\n", noThreadTime);
    
    free(arr);
    exit(0);
}

int ReadArrLen(){
    int len;
    printf("Enter array lenght: ");
    scanf("%d", &len);
    return len;
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

void ThreadTest(int *arr, int len){
    
    pthread_t thread_1;
    pthread_t thread_2;

    ArraySectionData section1;
    ArraySectionData section2;

    FillArraySectionData(&section1, arr, 0, len / 2);
    FillArraySectionData(&section2, arr, len / 2 + 1, len - 1);

    pthread_create(&thread_1, NULL, SectionSum, &section1);
    pthread_create(&thread_2, NULL, SectionSum, &section2);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    
    long sum = section1.resultSum + section2.resultSum;
    printf("Sum = %ld\n", sum);
}

void NoThreadTest(int *arr, int len){
    long sum = 0;
    for(int i = 0; i < len; i++)
        sum += StupidReturn(arr[i]);
    printf("Sum = %ld\n", sum);
}
