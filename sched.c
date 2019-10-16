#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>


struct process {
    int pid;
    int arr_time;
    int burst_time;
    int wait_time;
    int response_time;
    int turnaround_time;
};

struct process p[1000];

void rr(int n, int q){
    
    int k = 0;
    int cpu_wait = 0;
    
    int jl = 0;
   
    while(1)
    {
        int in = -1;
        
        // find next process which has not finished yet and has arrived now
        for (int i = jl; i < 2 * n; i++)
        {

            if (p[i%n].burst_time > 0 && k >= p[i%n].arr_time)
            {
                in = i%n;
                break;
            }
        }
        
        jl = in;
        jl++;
        
        
        // if all the processes are finished
        if (in == -1){
           
            break;
        }
        
        
        
        
        // while system time is less then arrival time then cpu has to wait
        while (k < p[in].arr_time)
        {
            printf ("<system time %d> wait\n", k);
            cpu_wait++;
            k++;
            
        }
        
        // update response time for the first time.
        if (p[in].response_time == -1)
            p[in].response_time = k - p[in].arr_time;
        
        
        int mini;
        if (q > p[in].burst_time)
            mini = p[in].burst_time;
        else
            mini = q;
        
       // smaller of time quantum or burst timne left process will run
        for (int i = 0; i < mini; i++)
        {
            printf ("<system time %d> process %d running\n", k, p[in].pid);
            
            for (int l = 0; l < n; l++)
            {
                // rest of the processes which has arrived and not completed yet has to wait
                if (l != in && p[l].burst_time != 0 && k >= p[l].arr_time)
                    p[l].wait_time++;
                
                // if process is arrived, their turnaround time is gettiong increased.
                if (p[l].burst_time > 0 && k >= p[l].arr_time)
                    p[l].turnaround_time = p[l].turnaround_time + 1;
            }
            // decrease burst time of process which is running
            p[in].burst_time--;
            k++;
        }
        
        // if process is finished
        if (p[in].burst_time == 0)
            printf ("<system time %d> process %d finished.....\n", k, p[in].pid);
    }
    
    printf ("<system time %d> all processes finished......\n", k);
    printf("=========================================================================\n");
    double res = 0, wait = 0, turnaround_time = 0;
    
    printf("Average CPU usage : %lf\n",  (k - cpu_wait) * 100.0 / k );
    
    for (int i = 0; i < n; i++)
    {
        turnaround_time += p[i].turnaround_time;
        wait += p[i].wait_time;
        res += p[i].response_time;
    }
    
    printf("Average wait time : %lf\n",  (double) wait  / n );
    printf("Average response_time : %lf\n",  (double)res / n );
    printf("Average turnaround time : %lf\n",  (double)turnaround_time / n );
    
}

void fcfs( int n){
    printf ("Total %d tasks are in input1.file\n", n);
   
    int k = 0;
    int wait_time = 0;
    for (int i = 0; i < n; i++)
    {
        int small = INT_MAX;
        int in = -1;
        
        // find the process which has arrived earliest and has not finished.
        for (int j = 0; j < n; j++) {
            if (small > p[j].arr_time && p[j].burst_time > 0)
            {
                small = p[j].arr_time;
                in = j;
            }
        }
        
        // if there is no process in system, system has to wait
        while (in != -1 && k < small)
        {
            printf("<system time %d> wait\n", k);
            wait_time++;
            k++;
        }
        
        p[in].response_time = k - p[in].arr_time;
        
        // run process p[in]
        for (int j = 0; j < p[in].burst_time; j++)
        {
            
            printf ("<system time %d> process %d running\n", k, p[in].pid);
            
            
            for (int l = 0; l < n; l++)
            {
                // rest of the processes which has arrived and not completed yet has to wait
                if (l != in && p[l].burst_time != 0 && k >= p[l].arr_time)
                    p[l].wait_time++;
            }
            k++;
        }
       
        
        p[in].turnaround_time = p[in].wait_time + p[in].burst_time ;
         p[in].burst_time = 0;
        printf ("<system time %d> process %d finished.....\n", k, p[in].pid);
    }
    
    printf ("<system time %d> all processes finished......\n", k);
    printf("=========================================================================\n");
    double res = 0, wait = 0, turnaround_time = 0;
    
    printf("Average CPU usage : %lf\n",  (k - wait_time) * 100.0 / k );
    
    for (int i = 0; i < n; i++)
    {
        turnaround_time += p[i].turnaround_time;
        wait += p[i].wait_time;
        res += p[i].response_time;
    }
    //calculate values
    printf("Average wait time : %lf\n",  (double) wait  / n );
    printf("Average response_time : %lf\n",  (double)res / n );
    printf("Average turnaround time : %lf\n",  (double)turnaround_time / n );
    
}

void srtf(int n){
    
      printf ("Total %d tasks are in input1.file\n", n);
    int k = 0;
    int cpu_wait = 0;
    
    while(1)
    {
        int in = -1;
        
        int small = INT_MAX;
        
        // find process whose burst time is lowest but not finished and has arrived.
        for (int i = 0; i < n; i++)
        {
            if (small > p[i].burst_time && p[i].burst_time != 0  && k >= p[i].arr_time)
            {
                small = p[i].burst_time;
                in = i;
            }
        }
        
        int flag = 0;
        if (in == -1)
        {
            for (int i = 0; i < n; i++)
            {
                if (p[i].burst_time > 0)
                {
                    flag = 1;
                    cpu_wait++;
                }
            }
            
            if (flag == 0)
                break;
        }
        
        // system has to wait for process arrival
        while (k < p[in].arr_time)
        {
            printf("<system time %d> wait\n", k);
            cpu_wait++;
            k++;
        }
        
        printf ("<system time %d> process %d running\n", k, p[in].pid);
        
        // uodate response time when the process starts running for the first time
        if (p[in].response_time == -1)
            p[in].response_time = k - p[in].arr_time;
        
        p[in].burst_time--;
        
        if (p[in].burst_time == 0)
             printf ("<system time %d> process %d finished.....\n", k, p[in].pid);
        
        for (int i = 0; i < n; i++)
        {
            // find process whose burst time is lowest but not finished and has arrived.
            if (i != in && p[i].burst_time > 0 && k >= p[i].arr_time)
            p[i].wait_time++;
            
            // if process is arrived, their turnaround time is gettiong increased.
            if (p[i].burst_time > 0 && k >= p[i].arr_time)
                p[i].turnaround_time = p[i].turnaround_time + 1;
        }
        
        
        if (p[in].burst_time == 0)
        p[in].turnaround_time++;
        
        k++;
        
    }
    
    printf ("<system time %d> all processes finished......\n", k);
    printf("=========================================================================\n");
    double res = 0, wait = 0, turnaround_time = 0;
    
    printf("Average CPU usage : %lf\n",  (k - cpu_wait) * 100.0 / k );
    
    for (int i = 0; i < n; i++)
    {
        turnaround_time += p[i].turnaround_time;
        wait += p[i].wait_time;
        res += p[i].response_time;
    }
    
    // calculate value
    printf("Average wait time : %lf\n",  (double) wait  / n );
    printf("Average response_time : %lf\n",  (double)res / n );
    printf("Average turnaround time : %lf\n",  (double)turnaround_time / n );
    
    
    
    
}

int main(int argc, char *argv[]) {
    FILE *file;
    file = fopen(argv[2], "r");
    
    int pid, at, bt, i = 0;
    char buff[255];
    
    // populate proces structure
    while (fgets(buff, 255, (FILE*) file))
    {
        sscanf (buff, "%d %d %d\n", &pid, &at, &bt) ;
        p[i].pid = pid;
        p[i].arr_time = at;
        p[i].burst_time = bt;
        p[i].wait_time = 0;
        p[i].response_time = -1;
        p[i].turnaround_time = 0;
        i++;
    }
    
    
    fclose(file);
  
    if(argc == 4) { // If there are 3 arguments
        if(strcmp(argv[3], "FCFS") == 0) {
            printf("Scheduling algorithm: %s\n", argv[3]);
                   fcfs( i);
                   }
        else if(strcmp(argv[3], "SRTF") == 0) {
               printf("Scheduling algorithm: %s\n", argv[3]);
                srtf(i);
        }
        else {
             fprintf(stderr, "Execute: scheduler.c input.1 FCFS|RR|SRTF [quantum]\n");
                                          
        }
                                          
    }
    else if(argc == 5 && strcmp(argv[3], "RR") == 0) { // If there are 4 arguments and using RR
              printf("Scheduling algorithm: %s\n", argv[3]);
                                     rr(i, atoi(argv[4]));
               }
    else { // If not following proper syntax,print error
            fprintf(stderr, "Execute: scheduler.c input.1 FCFS|RR|SRTF [quantum]\n");
    }
    
    return 0;
}
