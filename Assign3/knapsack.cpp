#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<random>

using namespace std;

class knapsack_w
{
    int* w;
    int N;
    int k;

    public:

    knapsack_w(int N,int* weights)
    {
        w = new int [N];
        this->N = N;
        for (int i=0;i<N;i++)
            w[i] = weights[i];
        k = 0;
    }

    ~knapsack_w()
    {
        delete [] w;
    }

    bool knapsack(int W,int i)
    {   
        k+=1;
        if (W==0)
        {
            // cout<<endl;
            return true;
        }

        else if ((W<0) || (i>=N))
            return false;

        else if (knapsack(W-w[i],i+1))
        {
            // cout<<w[i]<<" ";
            return true;
        }

        else
        {
            return knapsack(W,i+1);
        }
    }

    int ret_calls()
    {
        return k;
    }
};

int main()
{
    time_t t;
    srand((unsigned) time(&t));

    int n_exp = 10000;
    int min_calls[20];
    int max_calls[20];
    for (int i=0;i<20;i++)
    {
        min_calls[i] = -1;
        max_calls[i] = -1;
    }

    for (int i=0;i<n_exp;i++)
    {
        int N = rand()%(20) + 1;
        int range_W = (int)(N*N/2);
        int W = rand()%(range_W+1);
        int* w = new int [N];
        for (int j=0;j<N;j++)
            w[j] = rand()%(N+1);
        knapsack_w knap(N,w);
        knap.knapsack(W,0);
        
        int calls = knap.ret_calls();
        if (min_calls[N-1]==-1)
        {   
            min_calls[N-1] = calls;
            max_calls[N-1] = calls;
        }

        else
        {
            if (calls>max_calls[N-1])
                max_calls[N-1] = calls;
            
            else if (calls<min_calls[N-1])
                min_calls[N-1] = calls;
        }
    }
    cout<<"N\tMin\tMax"<<endl;
    for (int i=0;i<20;i++)
    {
        cout<<i+1<<"\t"<<min_calls[i]<<"\t"<<max_calls[i]<<endl;  
    }
    return 0;
}