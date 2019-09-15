#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

struct PACKET
{
    int id; // packet id
    int t0; // arrival time of packet
    int priority; // higher means more important 
    char contents[80]; // contents of packet
};

class QUEUE
{
    PACKET* p;
    int N;
    int rear,front;

    public:

    QUEUE(int N)
    {
        this->N = N;
        p = new PACKET [N];
        rear = -1;
        front = -1;
    }

    ~QUEUE()
    {
        delete [] p;
    }

    bool isFull()
    {
        return ((rear+1)%N == front);
    }

    bool isEmpty()
    {
        return (rear==front || rear==-1);
    }

    void push(PACKET* p_in)
    {
        if (!isFull())    
        {
            if (rear == -1)
            {
                front = 0;
                rear = 0;
            }

            rear = (rear+1)%N;
            p[rear].id = p_in->id;
            p[rear].t0 = p_in->t0;
        }
    }

    void pop()
    {
        if(!isEmpty())
            front = (front+1)%N;
    }

    PACKET* front_val()
    {
        if(!isEmpty())    
            return &p[front];
        
        else
        {
            cout<<"Stack is Full";
            return (PACKET*)NULL;
        }
    }
};

int nextTime(float rateParameter)
{
    return (int)(-logf(1.0f - (float) random() / (RAND_MAX)) / rateParameter);
}

int main(int argc, char** argv)
{
    srand(time(0));

    if (argc!=4)
    {
        cout<<"Invalid arguments"<<endl;
        exit(0);
    }

    int N = atoi(argv[1]);
    float lambda = atof(argv[2]);
    float mu = atof(argv[3]);
    QUEUE node(N);

    int t = 0;
    int iter = 10000;
    int id = 0;
    int drop = 0;
    int flag = nextTime(lambda);

    while (t<iter)
    {
        
        while(flag == 0) // to push pkts occuring simultaneously
        {
            PACKET* p;
            p = new PACKET;
            p->t0 = t;
            p->id = id++;
            if (node.isFull())
                drop++;
            else
                node.push(p);
            flag = nextTime(lambda);
            delete p;
        }
        flag--;
        
        int check = (t% (int)(1/mu));

        if (check == 0)
        {
            node.pop();
        }
        
        t++;
    }
    cout<<"Total Packets: "<<id<<endl;
    cout<<"Total Time: "<<t<<endl;
    cout<<"Total Drops: "<<drop<<endl;
    cout<<"%Drop: "<<(100.0*drop/id)<<endl;
    cout<<"Time Per Packet: "<<(float)t/id<<endl;
    return 0;
}