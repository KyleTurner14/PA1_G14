//  Group 14:   Kyle Turner
//              Robert Shannahan
//              John Miller
//  Date: 2/11/18
//  Programming Assignment 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// structs used
typedef struct{
    char name[20];      // name of the process
    int arrival;        // arrival time
    int burst;          // burst time
    int run;            // tracks if it's running or not
    int waitTime;       // how long it waits
    int turnaround;     // how long it took to finish
} processes;

typedef struct {
    int processCount;   // # of processes
    int runfor;         // run for duration
    int use;            // 1 = fcfs, 2 = pe-sjf, 3 = rr
    int quantnum;       // only for rr
} information;


// function prototypes
information readInput(FILE*input, processes*array, int*size);
void quickSort(processes*array, int size);
void firstComeFirstServe(FILE*output, information info, processes*array, int size);
void shortestJobFirst(FILE*output, information info, processes*array, int size);
void roundRobin(FILE*output, information info, processes*array, int size);
void printWaitTimes(FILE*output, processes*array, int size);

int main(int argc, const char * argv[]) {
    
    int size = 0;
    
    // open input file & create output file
    FILE*input = fopen("processes.in", "r");
    FILE*output = fopen("processes.out", "w");
    
    // read the input
    processes array[1000];
    information info = readInput(input, array, &size);
    
    // sort the array of processes
    quickSort(array, size);
    
    //for(int i = 0; i < size; i++) printf("%s | %d | %d\n", array[i].name, array[i].arrival, array[i].burst);
    
    switch(info.use){
        case 1:
            firstComeFirstServe(output, info, array, size);
            break;
        case 2:
            shortestJobFirst(output, info, array, size);
            break;
        case 3:
            roundRobin(output, info, array, size);
            break;
        default:
            fclose(input);
            fclose(output);
            exit(-1);
    }
    
    // close files
    fclose(input);
    fclose(output);
    return 0;
}


// reads from the input and stores the data collected in to the information struct.
information readInput(FILE*input, processes*array, int*size){
    int i = 0;
    char strBuild[100], use[4];
    
    information info;
    
    // while string != end, we will read through the file
    while(strcmp(strBuild, "end") != 0){
        fscanf(input, "%s\n", strBuild);
        
        // skip until next line if #
        if(strcmp(strBuild, "#") == 0){
            fscanf(input, "%[^\n]", strBuild);
            continue;
            
        // store the process count
        } else if(strcmp(strBuild, "processcount") == 0){
            fscanf(input, "%d", &info.processCount);

        // store the runfor duration
        } else if(strcmp(strBuild, "runfor") == 0){
            fscanf(input, "%d", &info.runfor);
            
        // get the use (fcfs = 1/sjf = 2/rr = 3)
        } else if(strcmp(strBuild, "use") == 0){
            fscanf(input, "%s", use);
            
            if(strcmp(use, "fcfs") == 0){
                info.use = 1;
                
            } else if(strcmp(use, "sjf") == 0){
                info.use = 2;
                
            } else if(strcmp(use, "rr") == 0){
                info.use = 3;
                
                fscanf(input, "%[^\n]", strBuild);
                fscanf(input, "%s", strBuild);
                
                // get the quantum size
                fscanf(input, "%d", &info.quantnum);
            }
        
        // get info for each process
        } else if(strcmp(strBuild, "process") == 0){
            
            // get the name
            fscanf(input, "%s", strBuild);
            fscanf(input, "%s", array[i].name);

            // get arrival time
            fscanf(input, "%s", strBuild);
            fscanf(input, "%d", &array[i].arrival);
            
            // get burst time
            fscanf(input, "%s", strBuild);
            fscanf(input, "%d", &array[i].burst);
            
            array[i].run = 0;
            //increment indext
            i++;
        }
    }
    
    //save array size
    *size = i;
    
    return info;
}


// Put the array in chronological order based off arrival time.
void quickSort(processes*array, int size){
    
    // for an array of size 1, return because there's no point of sorting
    if(size < 2) return;
    
    int pivot = array[size/2].arrival;
    
    int i = 0, j = 0;
    for(i = 0, j = size - 1; ; i++, j--){
        while(array[i].arrival < pivot) i++;
        while(array[j].arrival > pivot) j--;
        
        if(i >= j) break;
        
        // swap around
        processes temp;
        
        temp.arrival = array[i].arrival;
        temp.burst   = array[i].burst;
        strcpy(temp.name, array[i].name);
        
        array[i].arrival = array[j].arrival;
        array[i].burst   = array[j].burst;
        strcpy(array[i].name, array[j].name);
        
        array[j].arrival = temp.arrival;
        array[j].burst   = temp.burst;
        strcpy(array[j].name, temp.name);
        
    }
    
    // recursive calls
    quickSort(array, i);
    quickSort(array + i, size - i);
}


// Implementation of first first come first serve, outputs to processes.out
void firstComeFirstServe(FILE*output, information info, processes*array, int size){
    
    int i = 0, countdown = 0, runningSomething = 0, current = 0;
    
    // Print Header
    fprintf(output, "%d processes\nUsing First-Come First-Served\n\n", info.processCount);
    
    // For the duration, try to run the FIFO
    while(countdown <= info.runfor){
        
        if(array[i].arrival == countdown){
            fprintf(output, "Time %d: %s arrived\n", countdown, array[i].name);
            i++;
            
        }
        
        // Say this process is running now
        if(array[current].arrival <= countdown && array[current].run == 0 && runningSomething == 0){
            fprintf(output, "Time %d: %s selected (burst %d)\n", countdown, array[current].name, array[current].burst);
            runningSomething   = 1;
            array[current].run = 1;
            
            // calculate wait time
            array[current].waitTime = countdown - array[current].arrival;
            
            // calculate the turnaround time
            array[current].turnaround = array[current].waitTime + array[current].burst;
            
        }
        
        // decrement the burst length of the running process
        if(array[current].arrival <= countdown) array[current].burst = array[current].burst - 1;
    
        countdown++;
        
        // check if we finish early
        if(array[size-1].burst == 0 && countdown <= info.runfor){
            fprintf(output, "Finished at time %d\n\n", countdown);
            
            // print the wait times
            printWaitTimes(output, array, size);
            
            // go back to main
            return;
            
        // check for idle
        } else if(runningSomething == 0){
            fprintf(output, "Time %d: IDLE\n", countdown);
            
        // reset the current process running because this on is finished
        } else if(array[current].burst == 0){
            fprintf(output, "Time %d: %s Finished\n", countdown, array[current].name);
            
            // reset the running flag and increment current
            runningSomething = 0;
            current = current + 1;
            
        }
        
    }// end while();
    
    fprintf(output, "Failed to Finish\n");
}


// Implementation of shortest job first, outputs to processes.out
// HANDLES HEADER & FOOTER
void shortestJobFirst(FILE*output, information info, processes*array, int size)
{ 
	// Created by John Millner
	
	// Print Header
    fprintf(output, "%d processes\nUsing Shortest Job First (Pre) \n\n", info.processCount);
	
	int timer 	= 0;					// descibes current time
	int time 	[ info.runfor ];		// descibes timeline 
	
	// keep track of time and run until time completed
	for( timer = 0; timer < info.runfor; timer++ )
		time[ timer ] = helper_reportWinner( output, info, array, timer, time[ timer - 1 ] );
		
	//footer
	fprintf(output, "Finished at time %d\n\n", timer );
	int i;
	for( i = 0; i < info.processCount; i ++ )
	{
		fprintf( output, "%s wait %d turnaround %d\n", array[ i ].name, array[ i ].waitTime, array[ i ].turnaround); 
	}  
}

// returns the process that "wins" the time slot for P-SJF
// HANDLES PRINTING OF SELECTION, IDLE, ARRIVED AND FINISHED
int helper_reportWinner( FILE*output, information info, processes* array, int timer, int oldWinner )
{
	//Created by John Millner
	
	// who can run (arrival time)
	// who can run completly before time runs out
	// who has the shortest runtime
	
	int processIndex = -1; 		// -1 represents IDLE, if not winner, default IDLE
	int burstWinner = INT_MAX; 	// holder for shortest burst time
	
	int i;
	for( i = 0; i < info.processCount; i ++ )
	{
		// check if process is available to run
		if( array[ i ].arrival <= timer )
		{
			//if the process is fresh off the boat - make note!
			if( array[ i ].arrival == timer)
				fprintf(output, "Time %d: %s arrived\n", timer, array[ i ].name);
				
				
			// check if this process still has work to do
			if( array[ i ].burst > 0 )
			{
				// check if this process has the smallest burst time
				if( array[ i ].burst < burstWinner )
				{
					processIndex = i;
					burstWinner = array[ i ].burst;
				}
				else // this processs has lost to a shorter job, so incrememt wait timer
				{
					array[ i ].waitTime++;
				}
			}
			else // process has just completed!
			{
				array[ i ].turnaround = timer - array[ i ].arrival; //set turnaround time
				array[ i ].arrival = INT_MAX; // lets not crunch numbers if we dont have to
      			fprintf(output, "Time %d: %s Finished\n", timer, array[ i ].name);
			}
		}				
	}
	
	//make note if this process IDLE or was freshly selected
	if( processIndex == -1 )
		fprintf(output, "Time %d: IDLE\n", timer);	
	else if( processIndex != oldWinner )
	{
		array[i].burst--; // decrement burst since work has been done
		fprintf(output, "Time %d: %s selected (burst %d)\n", timer, array[ processIndex ].name, array[ processIndex ].burst);
	}
	
	return processIndex;
}

// Implementation of round robin, outputs to processes.out
void roundRobin(FILE*output, information info, processes*array, int size){
    
}


// function to print wait times so we don't have to all write it in our functions
void printWaitTimes(FILE*output, processes*array, int size){
    int i = 0;
    
    for(i = 0; i < size; i++){
        fprintf(output, "%s wait %d turnaround %d\n", array[i].name, array[i].waitTime, array[i].turnaround);
    }
}
