//  Group 14:   Kyle Turner
//              Robert Shannahan
//              John Miller
//  Date: 2/11/18
//  Programming Assignment 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs used
typedef struct{
    char name[20];      // name of the process
    int arrival;        // arrival time
    int burst;          // burst time
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
    
    // for(int i = 0; i < size; i++) printf("%s | %d | %d\n", array[i].name, array[i].arrival, array[i].burst);
    
    
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
    
    int countdown = 0;
    
    fprintf(output, "%d processes\nusing First-Come First-Served\n", info.processCount);
    
    while(countdown <= info.runfor){
        
        
    }
}

// Implementation of shortest job first, outputs to processes.out
void shortestJobFirst(FILE*output, information info, processes*array, int size){
    
}

// Implementation of round robin, outputs to processes.out
void roundRobin(FILE*output, information info, processes*array, int size){
    
}
