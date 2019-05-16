#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <unistd.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

#define clear() printf("\033[H\033[J")
#define REPEATS 10
#define SIZE1 512
#define SIZE2 1024
#define SIZE3 2048
#define NUM_OF_OPERATIONS 4


typedef struct {
    float v1;
    float v2;
    float v3;
    float v4;
} vector;


int size;
int sizes[3] = {2048, 512, 1024};


void printMenu() {
    printf("-----------------\n");
    printf("----Main Menu----\n");
    printf("-----------------\n\n");
    printf("Calculations for:\n");
    printf("1. 2048 numbers\n");
    printf("2. 4096 numbers\n");
    printf("3. 8192 numbers\n");
    printf("4. Exit program\n\n");
    printf("Choice: ");
}

void displayVector(vector *vec, int index) {
    printf("no. %d\n", index);
    printf("%.1f\n", vec->v1);
    printf("%.1f\n", vec->v2);
    printf("%.1f\n", vec->v3);
    printf("%.1f\n\n", vec->v4);
}

void fillVectorRand(vector vec[]) {
    for(int i = 0; i<size; ++i) {
        vec[i].v1 = ((float)rand()/RAND_MAX * (float)rand()) - rand();
        vec[i].v2 = ((float)rand()/RAND_MAX * (float)rand()) - rand();
        vec[i].v3 = ((float)rand()/RAND_MAX * (float)rand()) - rand();
        vec[i].v4 = ((float)rand()/RAND_MAX * (float)rand()) - rand();
    }
}

void printTimes(double *times) {
    printf("Avg time [seconds]:\n");
    printf("'+' - %f\n", times[0]);
    printf("'-' - %f\n", times[1]);
    printf("'x' - %f\n", times[2]);
    printf("'/' - %f\n", times[3]);
}

double add_SIMD(vector *vec1, vector *vec2, vector *results) {
    clock_t time = clock();
    __asm__(
        "movups %1, %%xmm1\n"  
        "movups %2, %%xmm2\n"     
        "addps %%xmm2, %%xmm1\n"  
        "movups %%xmm1, %0"
        : "=m"(results)   
        : "m"(vec1), "m"(vec2)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC ;
}

double sub_SIMD(vector *vec1, vector *vec2, vector *results) {
    clock_t time = clock();
    __asm__(
        "movups %1, %%xmm1\n"  
        "movups %2, %%xmm2\n"     
        "subps %%xmm2, %%xmm1\n"  
        "movups %%xmm1, %0"
        : "=m"(results)   
        : "m"(vec1), "m"(vec2)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC ;
}

double mul_SIMD(vector *vec1, vector *vec2, vector *results) {
    clock_t time = clock();
    __asm__(
        "movups %1, %%xmm1\n"  
        "movups %2, %%xmm2\n"     
        "mulps %%xmm2, %%xmm1\n"  
        "movups %%xmm1, %0"
        : "=m"(results)   
        : "m"(vec1), "m"(vec2)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC ;
}

double div_SIMD(vector *vec1, vector *vec2, vector *results) {
    clock_t time = clock();
    __asm__(
        "movups %1, %%xmm1\n"  
        "movups %2, %%xmm2\n"     
        "divps %%xmm2, %%xmm1\n"  
        "movups %%xmm1, %0"
        : "=m"(results)   
        : "m"(vec1), "m"(vec2)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC ;
}


double add_SISD(vector *v1, vector *v2, vector *results){
    clock_t time = clock();
    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FADDP\n"

        : "=m" (results->v1)
        : "m" (v1->v1), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FADDP\n"

        : "=m" (results->v2)
        : "m" (v1->v2), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FADDP\n"

        : "=m" (results->v3)
        : "m" (v1->v3), "m" (v2->v3)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FADDP\n"

        : "=m" (results->v4)
        : "m" (v1->v4), "m" (v2->v4)
    );

    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC ;
}

double sub_SISD(vector *v1,vector *v2,vector *results){
    clock_t time = clock();
    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FSUBP\n"

        : "=m" (results->v1)
        : "m" (v1->v1), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FSUBP\n"

        : "=m" (results->v2)
        : "m" (v1->v2), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FSUBP\n"

        : "=m" (results->v3)
        : "m" (v1->v3), "m" (v2->v3)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FSUBP\n"

        : "=m" (results->v4)
        : "m" (v1->v4), "m" (v2->v4)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC  ;
}

double mul_SISD(vector *v1,vector *v2,vector *results){
    clock_t time = clock();
    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FMULP\n"

        : "=m" (results->v1)
        : "m" (v1->v1), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FMULP\n"

        : "=m" (results->v2)
        : "m" (v1->v2), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FMULP\n"

        : "=m" (results->v3)
        : "m" (v1->v3), "m" (v2->v3)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FMULP\n"

        : "=m" (results->v4)
        : "m" (v1->v4), "m" (v2->v4)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC ;
}

double div_SISD(vector *v1,vector *v2,vector *results){
    clock_t time = clock();
    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FDIVP\n"

        : "=m" (results->v1)
        : "m" (v1->v1), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FDIVP\n"

        : "=m" (results->v2)
        : "m" (v1->v2), "m" (v2->v2)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FDIVP\n"

        : "=m" (results->v3)
        : "m" (v1->v3), "m" (v2->v3)
    );

    __asm__(
        "FLD %1\n"
        "FLD %2\n"
        "FDIVP\n"

        : "=m" (results->v4)
        : "m" (v1->v4), "m" (v2->v4)
    );
    time = clock() - time;
    return ((double)time)/CLOCKS_PER_SEC  ;
}

void manageCalc(int selection) {
    size = sizes[selection%3];
    clock_t time;
    double time_taken;
    double timesSIMD[NUM_OF_OPERATIONS] = {0.0, 0.0, 0.0, 0.0};
    double timesSISD[NUM_OF_OPERATIONS] = {0.0, 0.0, 0.0, 0.0};
    vector vec1[size];
    vector vec2[size];
    vector results[size];
    fillVectorRand(vec1);
    fillVectorRand(vec2);

    for(int i = 0; i < REPEATS; i++) {
        for(int j = 0; j<size; j++) {
            timesSIMD[0] += add_SIMD(&vec1[j], &vec2[j], &results[j]);
            timesSISD[0] += add_SISD(&vec1[j], &vec2[j], &results[j]);
            //printf("Resultant Vector ");
            //displayVector(&results[j], j);
        }
        
        for(int j = 0; j<size; j++) {
            timesSIMD[1] += sub_SIMD(&vec1[j], &vec2[j], &results[j]);
            timesSISD[1] += sub_SISD(&vec1[j], &vec2[j], &results[j]);
            //printf("Resultant Vector ");
            //displayVector(&results[j], j);
        }
            
        for(int j = 0; j<size; j++) {
            timesSIMD[2] += mul_SIMD(&vec1[j], &vec2[j], &results[j]);
            timesSISD[2] += mul_SISD(&vec1[j], &vec2[j], &results[j]);
            //printf("Resultant Vector ");
            //displayVector(&results[j], j);
        }
        
        for(int j = 0; j<size; j++) {
            timesSIMD[3] += div_SIMD(&vec1[j], &vec2[j], &results[j]);
            timesSISD[3] += div_SISD(&vec1[j], &vec2[j], &results[j]);
            //printf("Resultant Vector ");
            //displayVector(&results[j], j);
        }   
    }

    for(int i=0; i < NUM_OF_OPERATIONS; i++) {
        timesSIMD[i] = timesSIMD[i] / (REPEATS);
        timesSISD[i] = timesSISD[i] / (REPEATS);
    }
    printf("The amount of numbers: %d\n\n", size * 4);
    printf("Calculation type: SIMD\n");
    printTimes(timesSIMD);
    printf("\nCalculation type: SISD\n");
    printTimes(timesSISD);
    printf("Press ENTER to Continue\n");  
    getchar(); getchar();
}

int main() {

    srand(time(NULL));
    bool isRunning = true;
    while(isRunning == true) {
        clear();
        printMenu();
        int selection;
        scanf("%d", &selection);

        switch(selection) {
            case 1:
                size = SIZE1;
                manageCalc(selection);
                clear();
            break;
            case 2:
                size = SIZE2;
                manageCalc(selection);
                clear();
            break;
            case 3:
                size = SIZE3;
                manageCalc(selection);
                clear();
            break;
            case 4:
                isRunning = false;
            break;
            default:
                printf("Wrong input, try again\n");
                clear();
        }
    }
}