#include<iostream>
#include<cstdlib>

using namespace std;

#define data_type 0
#define video_type 1

struct PACKET
{
    int id;
    int type;
    float t_in;
    float priority;
    PACKET *next;

};

class FIFO_QUEUE
{
    PACKET *front;
    PACKET *rear;

    public:

    FIFO_QUEUE()
    {
        front = NULL;
        rear = NULL;
    }
    
    ~FIFO_QUEUE()
    {
        delete front;
        delete rear;
    }
    
    bool EMPTY()
    {
        return (rear==NULL);
    }

    void ENQUEUE(PACKET *p)
    {
        PACKET *temp = new PACKET;
        *temp = *p;

        if(EMPTY())
            front = rear = temp;

        else
        {
            rear->next = temp;
            rear = rear->next;
        }
    }

    void DEQUEUE()
    {
        if(!EMPTY())
        {
            PACKET *temp;
            temp = front;
            
            if(front==rear)
            {
                front = rear = NULL;
            }
            else
            {
                front = front->next;
            }

            delete temp;
        }
    }

    PACKET *FRONT()
    {
        PACKET *p = front;
        return p;
    }
};

int gcd(int a, int b)
{ 
    if (a == 0) 
        return b;  
    return gcd(b % a, a);  
 } 
  
int lcm(int a, int b)  
{  
   return (a*b)/gcd(a, b);  
}  

int main(int argc, char **argv)
{
    float video_inrate = 48;
    float data_insize = 6;
    float transm_rate = 64;
    float data_burstsize = 512;

    if (argc==5)
    {
        transm_rate = atof(argv[1]);
        video_inrate = atof(argv[2]);
        data_burstsize = atof(argv[3]);
        data_insize = atof(argv[4]);
    }

    float video_size = 0.5;
    float data_size = 1;
    float data_sec_arrtime = 0.5;  // between each second

    float max_video_delay = 1;
    float max_data_delay = 30;
    float drop_tol_v = 0.05;

    int video_indiv = (int) video_inrate/video_size;
    int video_outdiv = (int) transm_rate/video_size;
    int data_outdiv = (int) transm_rate/data_size;
    int time_div = lcm(lcm(video_indiv,video_outdiv),data_outdiv);
    video_indiv = time_div/video_indiv;
    video_outdiv = time_div/video_outdiv;
    data_outdiv = time_div/data_outdiv;

    int simulate_time = 120;
    
    int transmit_packet = 0;
    int data_drop = 0;
    int tot_data_drop = 0;
    int video_drop = 0;
    int tot_video_drop = 0;
    int data_transm = 0;
    int tot_data_transm = 0;
    int video_transm = 0;
    int tot_video_transm = 0;
    int data_retrans_next_sec = 0;
    int data_retrans_curr = 0;
    float data_delay = 0;
    float video_delay = 0;
    float tot_data_delay = 0;
    float tot_video_delay = 0;
    bool expired = false;
    int queue_len = 0;
    float video_transm_time = video_size/transm_rate;
    float data_transm_time = data_size/transm_rate;

    
    FIFO_QUEUE router;

    for(int i=0;i< (int) data_burstsize/data_size;i++)
    {
        PACKET *p = new PACKET;
        p->t_in = 0;
        p->type = data_type;
        router.ENQUEUE(p);
        queue_len++;
        delete p;
    }

    cout<<"Time\tQLength\tCondt\tHTTP\tVideo\tHTTP Drops\tVideo Drops"<<endl;

    for(int i=0; i<=time_div*simulate_time;i++)
    {
        float curr_time = i/time_div + (float)(i%time_div)/time_div;

        if(!(i%time_div))
        {
            data_retrans_curr = data_retrans_next_sec;
            tot_video_drop += video_drop;
            tot_data_drop += data_drop;
            tot_video_transm += video_transm;
            tot_data_transm += data_transm;
            tot_data_delay += data_delay;
            tot_video_delay += video_delay;

            float curr_video_delay = video_delay/(video_transm+1e-5);  // useful for plots
            float curr_data_delay = data_delay/(data_transm+1e-5);  // useful for plots
            float curr_video_wait = curr_video_wait - video_transm_time;
            float curr_data_wait = curr_data_wait - data_transm_time;
            
            if(expired)
                cout<<curr_time<<'\t'<<queue_len<<'\t'<<"EXPIRED"<<'\t'<<data_transm<<'\t'<<video_transm<<'\t'<<data_drop<<'\t'<<video_drop<<endl;        
            
            else
                cout<<curr_time<<'\t'<<queue_len<<'\t'<<"NO_DROP"<<'\t'<<data_transm<<'\t'<<video_transm<<'\t'<<data_drop<<'\t'<<video_drop<<endl;        

            data_drop = 0;
            video_drop = 0;
            data_transm = 0;
            video_transm = 0;
            data_delay = 0;
            video_delay = 0;
            data_retrans_next_sec = 0;
            expired = false;

            if((i/time_div) == simulate_time)
            {
                float avg_data_delay = tot_data_delay/(tot_data_transm+1e-5);
                float avg_video_delay = tot_video_delay/(tot_video_transm+1e-5);
                float avg_video_wait = avg_video_delay - video_transm_time;
                float avg_data_wait = avg_data_delay - data_transm_time;

                
                cout<<"Steady State Data Delay: "<<curr_data_delay<<"\tSteady State Video Delay: "<<curr_video_delay<<endl;
                cout<<"Data Transm. Time: "<<data_transm_time<<"\tVideo Transm. Time: "<<video_transm_time<<endl;
                cout<<"Avg. Data Wait Time: "<<avg_data_wait<<"\tAvg. Video Wait Time: "<<avg_video_wait<<endl;
                cout<<"Avg. Data Delay: "<<avg_data_delay<<"\tAvg. Video Delay: "<<avg_video_delay<<endl;
            }
        }

        if(!transmit_packet)
        {
            bool drop_packet = true;
            while(drop_packet)
            {
                if(router.EMPTY())
                    break;

                PACKET *p = router.FRONT();
                int type = p->type;
                float delay_time = curr_time - p->t_in;
                router.DEQUEUE();
                queue_len--;
                
                if(type == data_type)
                {
                    delay_time += data_transm_time;
                    drop_packet = (delay_time >= max_data_delay);
                    
                    if(drop_packet)
                    {
                        data_drop++;
                        data_retrans_next_sec++;
                        expired = true;
                    }
                    else
                    {   
                        data_transm++;
                        data_delay += delay_time;
                        transmit_packet = data_outdiv;
                    }
                }
                
                if(type == video_type)
                {
                    delay_time += video_transm_time;
                    drop_packet = (delay_time >= max_video_delay);
                    
                    if(drop_packet)
                    {
                        video_drop++;
                        expired = true;
                    }    

                    else
                    {   
                        video_transm++;
                        video_delay += delay_time;
                        transmit_packet = video_outdiv;
                    }
                }
            }
        }

        if(curr_time - (i/time_div) == data_sec_arrtime)
        {
            for(int k=0;k<(int)(data_insize/data_size)+data_retrans_curr;k++)
            {
                PACKET *p = new PACKET;
                p->t_in = curr_time;
                p->type = data_type;
                router.ENQUEUE(p);
                queue_len++;
                delete p;
            }
        }

        if(!(i%video_indiv))
        {
            PACKET *p = new PACKET;
            p->t_in = curr_time;
            p->type = video_type;
            router.ENQUEUE(p);
            queue_len++;
            delete p;
        }
        if(transmit_packet>0)
            transmit_packet--;
    }
    return 0;
}