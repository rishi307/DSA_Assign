/*
Assignment 1 - Traffic Intersection Problem
Course: EE4371 Intro to DSA
Author: Rishhanth Maanav V EE16B036
*/
#include <iostream>
#include <cstdlib>
#include<cstring>
#include <fstream>

using namespace std;

class Graph 
{ 
    int N;    // No. of vertices 
    int** adj;    // A dynamic array of adjacency matrix 
    int* coloured; // List of all coloured nodes

    public: 
        // Constructor of class Graph with number of nodes N 
        Graph(int N)   
        {
            this->N = N; 
            adj = new int* [N];
            for (int i=0;i<N;i++)
            {
                adj[i] = new int [N];
            }
            coloured = new int [N];

            for (int i=0;i<N;i++)
            {
                for (int j=0;j<N;j++)
                {
                    adj[i][j] = 0;
                }
                coloured[i] = 0;
            } 
        } 

        // Destructor of class Graph with number of nodes N 
        ~Graph()       
        { 
            delete [] adj; 
            delete [] coloured;
        } 
  
        // func to add an edge to graph 
        void addEdge(int v, int w)
        {
            adj[v][w] = 1;
            adj[w][v] = 1;
        }

        // func to add node to coloured list
        void colour_node(int n)
        {
            coloured[n] = 1;

        }

        // func to check if node v is coloured already
        int check_colored(int v)  
        {
            return coloured[v];
        }

        // func to check if nodes v,w clash (if I[v,w]=1)
        int check_collision(int v, int w)
        {
            return adj[v][w];
        }

        // func to check if all nodes have been coloured
        int check_all_color()
        {
            int flag = 1;
            for (int i = 0;i<this->N;i++)
                if (coloured[i] == 0)
                    flag = 0;
            
            return flag;
        }


}; 

// Greedy algorithm to colour graph
int* greedy(Graph& junct,int n) 
{
    int v, w;
    int* newcolour;
    newcolour = new int [n];
    int t = 0;

    for (int i=0;i<n;i++) 
        newcolour[i] = -1;

    for (v=0;v<n;v++)  //Loop through all nodes v in junction
        if (!junct.check_colored(v))  //Check if node v is not coloured already
        {   
            int found = 0;  //int flag to denote if node has clash
                for (w=0;w<n;w++)  // loop through all nodes w in new colour to check clash
                {
                    if (newcolour[w] == -1)  // check if all nodes w in newcolour are covered
                        break;
                    int tmp = newcolour[w]; 

                    if (junct.check_collision(v,tmp))  // check if nodes v,w clash
                        found = 1;  // set found = 1 for clash
                }
            if (!found)  // add node to new colour if no clash has been found
            {  
                junct.colour_node(v);  // add node to list of already coloured nodes in junction
                newcolour[t] = v;   // add node to newcolour
                t++;
            }
      
        }

        
    return newcolour;
}

int main(int argc, char** argv)
{   
    char DEFAULT_FILE[10] = "input.dat"; 
    char* FILE_NAME = DEFAULT_FILE;
    
    if (argc>1)  //check for command line arguments
        FILE_NAME = argv[1];
            
    FILE* fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char* line = NULL;
    size_t len = 0;
    int N;
    char** traffic_nodes;

    getline(&line, &len, fp);
    N = atoi(line);
    Graph junction(N);

    traffic_nodes = (char**) malloc(N*sizeof(char*));

    // store node names
    for (int j=0;j<N;j++)
    { 
        getline(&line, &len, fp);
        int line_len = strlen(line);
        if (line[line_len-1]=='\n')
            line[line_len-1]='\0';
        traffic_nodes[j] = (char*) malloc(sizeof(line));
        strcpy(traffic_nodes[j],line);
    }

    // store incidence matrix and add edges
    for (int j=0;j<N;j++)
    { 
        getline(&line, &len, fp);
        int l = 0;
        for (int i=0;line[i]!='\0';i++)
        {
            if ((line[i]==' ') || (line[i]=='\n') )
                continue;
            else
            {   
                char c[2];
                c[0] = line[i];
                c[1] = '\0';
                if (atoi(c)==1)
                    junction.addEdge(j,l);
                l++;
            }
        }

        
    }

    fclose(fp);
    
    if (line)
        free(line);
    

    while(!junction.check_all_color())
    {
        int* newcolour = greedy(junction,N);
        for (int i=0;i<N;i++)
        {
            int tmp = newcolour[i];  
            if (tmp==-1)  //check if all newly coloured nodes have been printed
            {    
                break;
            }
            else
                cout<<traffic_nodes[tmp]<<' ';  // print the group of newly coloured nodes in same row
        }
        delete [] newcolour;
        cout<<'\n';  // move to next row
    }

    free(traffic_nodes);
    return 0;
}