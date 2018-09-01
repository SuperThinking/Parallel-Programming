#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>

#define NO_THREADS 16

int inside_circ(int batch)
{
    int cir_pts=0;
    double rx, ry, dist;
    
    for(int i=0; i<batch; i++)
    {
        /*
        PRO TIP:
        Do not use rand() => Eats a lot of time in threading!
        */
        unsigned int myseed = i;
        rx = (double)(rand_r(&myseed))/RAND_MAX;
        ry = (double)(rand_r(&myseed))/RAND_MAX;
        dist = rx*rx + ry*ry;

        if(dist<=1)
        {
            cir_pts++;
        }
    }
    return cir_pts;
}

int main()
{
    int interval;
    double rx, ry, dist, pi;
    int cir_pts=0, sqr_pts=0;

    double t1 = omp_get_wtime();
    omp_set_num_threads(NO_THREADS);
    int batch = 100000;
    int EPOCHS = 1000;
    #pragma omp parallel for reduction(+:cir_pts)
    for(int i=0; i<EPOCHS; i++)
    {
        int t = inside_circ(batch);
        #pragma omp critical
        {
            cir_pts+=t;
        }
        //printf("%d by %d\n", cir_pts, omp_get_thread_num());
    }
    pi = (double)(4*cir_pts)/(EPOCHS*batch);
    double t2 = omp_get_wtime();
    printf("Time taken => %f\n%f\n", t2-t1, pi);
}