/*
Author: Rishhanth Maanav V
Roll No: EE16B036
Course: EE4371 DSA
Final Question 1: Modified Knapsack Problem 
Date : 08/12/2019
*/

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
  
int part_and_sort(int* weight, int low, int high) // picking partition and sort
{ 
    int pivot = weight[high];    // pivot 
    int i = (low - 1);  // smallest element 
    
    int temp = 0;
    for (int j = low; j <= high- 1; j++) 
    { 
        if (weight[j] < pivot) // current element is smaller than the pivot
        { 
            i++;    // increment index of smallest element 
            temp = weight[i];
            weight[i] = weight[j];
            weight[j] = temp;
        } 
    }
    temp = weight[i+1];
    weight[i+1] = weight[high];
    weight[high] = temp;
    return (i + 1); 
} 
  
void qkSort(int* weight, int low, int high)  // quicksort function
{ 
    if (low < high) 
    { 
        int p = part_and_sort(weight, low, high); // pick pivot and sort
        qkSort(weight, low, p - 1); // sort lower sub array
        qkSort(weight, p + 1, high); // sort upper sub array
    } 
}

float knapsack(int n, int *w, int W)  //knapsack function
{
    float s=0;
    float m=0;

    qkSort(w,0,n-1);  // sort weight array

    int *chosen_ind = malloc(sizeof(int)*n);

    for(int i=0;i<n;i++)  // chosen index array
        chosen_ind[i] = 0;

    int k;
    printf("Weights used: \n");  // printing weights used
    for(k = 0;k<n;k++)
    {
        int flag = 0;
        for(int i=n-1;i>=0;i--)
        {
            if (!chosen_ind[i])
            {
                if (((float) w[i] + s)<=sqrt(k+1)*W)
                {
                    chosen_ind[i] = 1; // update chosen set
                    s = s + w[i];
                    m = m + log10(w[i])/2.0;
                    flag = 1;  // found a weight
                    printf("%d\n",w[i]);  // printing weights chosen
                    break;
                }    
            }
        }
        if (!flag)
            break;  // no weight found
    }
    free(chosen_ind);
    printf("Number of elements used: %d\n",k+1);
    return m;
}

int main(int argc, char** argv)
{
    char DEFAULT_FILE[20] = "input1.txt";  // default file name
    char* FILE_NAME = DEFAULT_FILE;

    if (argc>1)
        FILE_NAME = argv[1];
    
    FILE* fptr = fopen(FILE_NAME, "r");

    char lines[5000][20];
    int n = 0;

    while(fgets(lines[n], sizeof(lines[n]), fptr)!=NULL)  // read from file
    {
        lines[n][strlen(lines[n])-1] = '\0';
        n++;
    }

    int* weights = malloc(sizeof(int)*n);
    int w = 10000;

    for(int i=0;i<n;i++)
    {
        weights[i] = atoi(lines[i]);  // string to number
    }
    
    float val = knapsack(n,weights,w);  // call knapsack
    free(weights);
    printf("Objective value: %f\n",val);  // print returned maximised objective
}