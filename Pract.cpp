#include <string>
#include <iostream>
#include <stdio.h>
#define MAX 3600
using namespace std;
float randomN(int,int,int);
class systems
{
private:
    int name; //идентификатор кассы 1 или 2
    int queue;
    float free;
public:
    systems(int n, int q, float t);
    float* takeplace(float *fec, int count); //метод занятия кассы  
    float randomNu(int min, int max);
    void set_free(float time){free=time;};
    float get_free(){return free;};
    int getQ() {return(queue);};
    void setQ(int q){queue=q;};
};
systems::systems(int n, int q, float t)
{
    name = n;
    queue = q;
    free = t;
}
float * systems::takeplace(float *fec, int count)
{
    int i;
    float *t_work = new float[i];
    for (i=0; i< count; i++)
    {
        if (name == 1)
            t_work[i] = randomNu(8,27);
        else 
            t_work[i] = randomNu(10,27);
    }
    return (t_work);
}
float systems::randomNu(int min, int max)
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    
    return static_cast<float>(rand() * fraction * (max - min + 1) + min);
}
float randomN(int min, int max, int seed)
{
    srand(seed);
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}
int main()
{
    FILE *f = fopen("log_file.txt", "w+");
    srand(time(NULL));
    int seed;
    seed = rand();
    float t_model = 0; //модельное время
    float t_generate = 0;
    int i, j, count, name1, name2, queue1, queue2, k, n;
    float *fec = new float[MAX]; //цепочка будущих событий
    for (i=0; t_model<=MAX; i++)
    {
        t_generate = randomN(0,27,seed++);
        t_model += t_generate;
        fec[i]=t_model;
    }
    count = i;
    name1=1;
    name2=2;
    queue1 = 0, queue2 = 0;
    
    systems A(name1,queue1,0);
    systems B(name2,queue2,0);
    
    float *t_work1 = A.takeplace(fec,count);
    float *t_work2 = B.takeplace(fec,count);
    float *t_leave = new float[MAX];
    for (k=0;k<count;k++)
    {
        t_model=fec[k];
        fprintf(f, "В момент времени %f заявка %d вошла в систему.\n",t_model,k+1);
        fprintf(f, "В момент времени %f заявка %d заняла очередь ",t_model,k+1);
        if (t_model>=A.get_free())
        {
            fprintf(f, "1.\n");
            if (A.getQ()>0)
            {
                queue1--;
                A.setQ(queue1);
            }
            fprintf(f, "В момент времени %f заявка %d заняла устройство 1.\n",t_model,k+1);
            t_leave[k] = t_model + t_work1[k];
            fprintf(f, "В момент времени %f заявка %d освободила устройство 1.\n",t_leave[k],k+1);
            A.set_free(t_leave[k]);
            
        }
            else if (t_model>=B.get_free())
                {
                    fprintf(f, "2.\n");
                    if (B.getQ()>0)
                    {
                        queue2--;
                        B.setQ(queue2);
                    }
                    fprintf(f, "В момент времени %f заявка %d заняла устройство 2.\n",t_model,k+1);
                    t_leave[k] = t_model + t_work2[k];
                      fprintf(f, "В момент времени %f заявка %d освободила устройство 2.\n",t_leave[k],k+1);
                    B.set_free(t_leave[k]);
                }
                else if (A.getQ()<=B.getQ())
                    {
                        fprintf(f, "1.\n");
                        t_model=A.get_free();
                        t_leave[k] = t_model + t_work1[k];
                        queue1++;
                        A.setQ(queue1);
                        fprintf(f, "В момент времени %f заявка %d заняла устройство 1.\n",t_model,k+1);
                        fprintf(f, "В момент времени %f заявка %d освободила устройство 1.\n",t_leave[k],k+1);
                        A.set_free(t_leave[k]);
                    }
                    else 
                    {
                        fprintf(f, "2.\n");
                        t_model=B.get_free();
                        queue2++;
                        B.setQ(queue2);
                        t_leave[k] = t_model + t_work2[k];
                        fprintf(f, "В момент времени %f заявка %d заняла устройство 2.\n",t_model,k+1);
                        fprintf(f, "В момент времени %f заявка %d освободила устройство 2.\n",t_leave[k],k+1);
                        B.set_free(t_leave[k]);
                    }
                    
    }
    fclose(f);
    return (1);
}
