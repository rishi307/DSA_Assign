/* 
Author: Rishhanth Maanav V
Roll No: EE16B036
Course: EE4371 DSA
Final Question 2 Fluid Particles Simulation
Date: 09/12/2019
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define n 100000
#define bin_rep 7
#define time 1e-1
#define time_step 1e-4
#define x_lim_c 1
#define y_lim_c 1
#define N_BINS 100
#define mass 1e3
#define EPS 1e-7

struct PARTICLE  // structure for particle
{
    int id;
    double pos[2];
    double vel[2];
    double acc[2];
    struct PARTICLE* next;
    struct PARTICLE* prev;
};

double randInRange(double min, double max)  // create random no in range
{
  return min + (rand() / (double) (RAND_MAX)) * (max - min);
}

struct PARTICLE** create_particles()  // function to create particles
{
    struct PARTICLE **particles = malloc(sizeof(struct PARTICLE*)*n);

    double x_lim = x_lim_c;
    double y_lim = y_lim_c;

    for(int i=0;i<n;i++)
        particles[i] = malloc(sizeof(struct PARTICLE));

    for(int i=0;i<n;i++)
    {
        particles[i]->id = i;
        particles[i]->pos[0] = randInRange(-x_lim,x_lim);
        particles[i]->pos[1] = randInRange(-y_lim,y_lim);

        if(i<(n/2))  //stationary
        {   
            particles[i]->vel[0] = 0;
            particles[i]->vel[1] = 0;
        }
        else //random unit norm velocity
        {
            double vx = randInRange(-1.0,1.0);
            double vy = randInRange(-1.0,1.0);
            double norm = sqrt(vx*vx + vy*vy + EPS);
            vx = vx/norm;
            vy = vy/norm;
            particles[i]->vel[0] = vx;
            particles[i]->vel[1] = vy;

        }
        particles[i]->acc[0] = 0;
        particles[i]->acc[1] = 0;

        particles[i]->prev = NULL;
        particles[i]->next = NULL;
    }

    return particles;
}

struct NODE  // node for tree
{
    struct NODE *c0,*c1,*c2,*c3;
    struct PARTICLE *list;
};

// helper fn to calculate binary rep.
void calc_signed_bin(struct PARTICLE* p,int binary_pos[2][bin_rep])
{
    for(int i=0;i<2;i++)
    {
        double pos_i = p->pos[i];
        double abs_pos = fabs(pos_i);
        for(int j=0;j<bin_rep;j++)
        {
            if(j==0) //sign bit
                binary_pos[i][j] = ( abs_pos==pos_i ? 0 : 1);

            else //binary rep
            {
                int bit = floor(2*abs_pos);
                binary_pos[i][j] = bit;
                abs_pos = 2*abs_pos - (double) bit;
            }
        }
    }
}

void build_tree(struct NODE* root,struct PARTICLE* p)  // add particle tree leaf
{
    struct NODE* check_node = root;
    
    int binary_pos[2][bin_rep];
    calc_signed_bin(p,binary_pos); // calculate binary representation

    for(int i=0;i<bin_rep;i++)
    {
        int check = 1*binary_pos[0][i] + 2*binary_pos[1][i];

        if(check==0)
        {    
            if(check_node->c0 == NULL)  // branching based on binary value
            {
                struct NODE* c = malloc(sizeof(struct NODE));
                c->c0 = NULL;
                c->c1 = NULL;
                c->c2 = NULL;
                c->c3 = NULL;
                c->list = NULL;
                check_node->c0 = c;
            }
            check_node = check_node->c0;
        }
        else if(check==1) // branching based on binary value
        {    
            if(check_node->c1 == NULL)
            {
                struct NODE* c = malloc(sizeof(struct NODE));
                c->c0 = NULL;
                c->c1 = NULL;
                c->c2 = NULL;
                c->c3 = NULL;
                c->list = NULL;
                check_node->c1 = c;
            }
            check_node = check_node->c1;
        }
        else if(check==2) // branching based on binary value
        {    
            if(check_node->c2 == NULL)
            {
                struct NODE* c = malloc(sizeof(struct NODE));
                c->c0 = NULL;
                c->c1 = NULL;
                c->c2 = NULL;
                c->c3 = NULL;
                c->list = NULL;
                check_node->c2 = c;
            }
            check_node = check_node->c2;
        }
        else // branching based on binary value
        {    
            if(check_node->c3 == NULL)
            {
                struct NODE* c = malloc(sizeof(struct NODE));
                c->c0 = NULL;
                c->c1 = NULL;
                c->c2 = NULL;
                c->c3 = NULL;
                c->list = NULL;
                check_node->c3 = c;
            }
            check_node = check_node->c3;
        }
    }

    // now insert the particles
    struct PARTICLE* temp = check_node->list;
    
    if(temp == NULL)  // first particle in this leaf
    {
        check_node->list = p;
    }
    
    else // doubly link with other particles already present
    {
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = p;
        p->prev = temp;
    }
}

// func to calculate acceleration of particle i after populating tree
void calc_acc(struct PARTICLE* p)
{
    p->acc[0] = 0;
    p->acc[1] = 0;
    double ax = 0;
    double ay = 0;
    // all those particles having common parent are doubly linked
    struct PARTICLE* temp = p->next; 
    while(temp!=NULL)  // traverse forward in doubly linked list
    {
        double x = p->pos[0] - temp->pos[0];
        double y = p->pos[1] - temp->pos[1];
        double denom = pow(x*x + y*y + EPS,1.5);
        ax += x/denom;
        ay += y/denom;
        temp = temp->next;
    }

    temp = p->prev;
    while(temp!=NULL) // traverse backward in doubly linked list
    {
        double x = p->pos[0] - temp->pos[0];
        double y = p->pos[1] - temp->pos[1];
        double denom = pow(x*x + y*y + EPS,1.5);
        ax += x/denom;
        ay += y/denom;
        temp = temp->prev;
    }

    p->acc[0] = ax/mass;
    p->acc[1] = ay/mass;
}

// simulate one step after populating tree
void simulate_one_step(struct PARTICLE** particles)
{
    double x_lim = x_lim_c;
    double y_lim = y_lim_c;

    for(int i = 0; i<n; i++)
    {
        calc_acc(particles[i]); //calculate acceleration for particle i
    }

    for(int i = 0; i<n; i++)
    {

        // x = 0.5 u*t + 0.5* v*t splitting into 2 halves for numerical stability

        particles[i]->pos[0] += 0.5*(particles[i]->vel[0])*time_step; // 1st half pos update
        particles[i]->pos[1] += 0.5*(particles[i]->vel[1])*time_step;

        particles[i]->vel[0] += (particles[i]->acc[0])*time_step;
        particles[i]->vel[1] += (particles[i]->acc[1])*time_step;

        particles[i]->pos[0] += 0.5*(particles[i]->vel[0])*time_step; // 2nd half pos update
        particles[i]->pos[1] += 0.5*(particles[i]->vel[1])*time_step;

        while(1)  // reflection loop stops if particle is inside confined space
        {
            if(particles[i]->pos[0]>=x_lim)  // reflect
            {
                particles[i]->pos[0] = 2*x_lim - particles[i]->pos[0];
                particles[i]->vel[0] = -particles[i]->vel[0];
            }

            else if(particles[i]->pos[0]<=-x_lim) // reflect
            {
                particles[i]->pos[0] = -2*x_lim - particles[i]->pos[0];
                particles[i]->vel[0] = -particles[i]->vel[0];
            }

            if(particles[i]->pos[1]>=y_lim)  // reflect
            {
                particles[i]->pos[1] = 2*y_lim - particles[i]->pos[1];
                particles[i]->vel[1] = -particles[i]->vel[1];
            }

            else if(particles[i]->pos[1]<=-y_lim)  // reflect
            {
                particles[i]->pos[1] = -2*y_lim - particles[i]->pos[1];
                particles[i]->vel[1] = -particles[i]->vel[1];
            }

            if((particles[i]->pos[0]<=x_lim) && (particles[i]->pos[0]>=-x_lim) && (particles[i]->pos[1]<=y_lim) && (particles[i]->pos[1]>=-y_lim))
                break;
        }
        particles[i]->prev = NULL;  // delink for next time step
        particles[i]->next = NULL;  // delink for next time step
    }
}

void free_tree(struct NODE* tree_node)  // freeing tree node space after a step
{
    if(tree_node!=NULL)
    {
        struct NODE* tree_node0 = tree_node->c0;
        struct NODE* tree_node1 = tree_node->c1;
        struct NODE* tree_node2 = tree_node->c2;
        struct NODE* tree_node3 = tree_node->c3;
        
        free(tree_node);
        free_tree(tree_node0);
        free_tree(tree_node1);
        free_tree(tree_node2);
        free_tree(tree_node3);
    }
}

// create speed vector
void create_velocity_vector(struct PARTICLE** particles, double *velocity)
{
    for(int i=0;i<n;i++)
    {
        float vx = particles[i]->vel[0];
        float vy = particles[i]->vel[1];
        velocity[i] = sqrt(vx*vx + vy*vy);
    }
}

// generate bins
float dist(double *v,int *bins)
{
    double maxVel=0.0;

    for(int i=0; i<n; i++)  // generate max velocity
    {
        if (i<N_BINS) 
            bins[i]=0;

        maxVel = fmax(maxVel, v[i]);
    }

    maxVel = maxVel + EPS;
    for(int i=0; i<n; i++) // bucket sort step 1 for binning
    {

        int idx = floor(v[i]*N_BINS/maxVel);
        bins[idx] = bins[idx] + 1;
    }
    return maxVel;
}

// main func
int main()
{
    int steps = (int) (time/time_step);
    // clock_t start, end;
    struct PARTICLE** particles = create_particles();

    double v_init[n];
    create_velocity_vector(particles,v_init);
    int bins_init[N_BINS];
    float max_vel_init = dist(v_init,bins_init);
    FILE* fpin = fopen("init.txt","w");
    for(int i=0;i<N_BINS;i++)
    {
        double bin_start = i*max_vel_init/N_BINS;
        double bin_end = (i+1)*max_vel_init/N_BINS;
        fprintf(fpin,"%f\t%f\t%d\n",i*max_vel_init/N_BINS,(i+1)*max_vel_init/N_BINS,bins_init[i]);
    }
    fclose(fpin);

    // double cpu_time_used = 0;
    // start = clock();
    for(int i=0;i<steps;i++)
    {
        struct NODE* root = malloc(sizeof(struct NODE)); // reinitialize tree
        root->c0 = NULL;
        root->c1 = NULL;
        root->c2 = NULL;
        root->c3 = NULL;
        root->list = NULL;

        for(int j=0;j<n;j++) // insert particles into tree 
        {
            build_tree(root,particles[j]);
        }

        simulate_one_step(particles);  // simulate a step
        free_tree(root); // free the tree nodes

        // cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;

    }
    // end = clock();
    
    // printf("Time used: %f ",cpu_time_used);

    double v_final[n];  // final dist
    create_velocity_vector(particles,v_final);
    int bins_final[N_BINS];
    float max_vel_final = dist(v_final,bins_final);
    
    FILE* fpout = fopen("final.txt","w");
    for(int i=0;i<N_BINS;i++)
    {
        double bin_start = i*max_vel_final/N_BINS;
        double bin_end = (i+1)*max_vel_final/N_BINS;
        fprintf(fpout,"%f\t%f\t%d\n",bin_start,bin_end,bins_final[i]);
    }

    free(particles);
    fclose(fpout);
    return 0;
}