#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <math.h>
#include "custom_types/ts.h"
#include "utilities.h"



using namespace std;


const unsigned short MAX_PROC_NUM = 10;
const unsigned short MAX_TASKS_NUM = 20;

// Rset[i] - set of tasks assigned to i-th processor,
// sorted by decreasing priorities (except "splitted" tasks)
unsigned short Rset[MAX_PROC_NUM][MAX_TASKS_NUM];
unsigned short tasksNumAssigned[MAX_PROC_NUM];
bool procHasSpaceCapacity[MAX_PROC_NUM];
bool taskSplitted[MAX_TASKS_NUM];
unsigned short cFracOfTaskSplittedPerProc[MAX_PROC_NUM][MAX_TASKS_NUM];



unsigned short get_Iij(const unsigned short i, const unsigned short j, const TS& ts){
    
    unsigned short F = ts.D[i]/ts.P[j];
    
    if (ts.D[i] >= F*ts.P[j] + ts.C[j]) return (F+1)*ts.C[j];
    else return ts.D[i] - F*(ts.P[j] - ts.C[j]);
}




unsigned short get_Rik(const unsigned short i, const unsigned short k, const TS& ts) {
    
    unsigned short Rik = ts.C[i];
    
    for (unsigned short taskNum = 0; taskNum < tasksNumAssigned[k]; taskNum++) {
        unsigned short j = Rset[k][taskNum];
        if (j < i) Rik += get_Iij(i, j, ts);
    }
    
    return Rik;
}




bool assign_task_to_processor(const TS& ts, const unsigned short m, const unsigned short i) {
    
    // task tau_i is assigned to the first processor,
    // such that sched. condition holds
    /*cout << "assigning task " << i << endl;
    cout << "flags and counters: " << endl;
    cout << "Task splitted?: \t\t\t";
    for (int j = 0; j < ts.n; j++) {
        cout << taskSplitted[j] << "\t";
    }
    cout << endl;*/
    
    /*cout << "Proc. has space capacity?: \t\t";
    for (int j = 0; j < m; j++) {
        cout << procHasSpaceCapacity[j] << "\t";
    }
    cout << endl;*/
    
    /*cout << "Number of tasks assigned to procs.: \t";
    for (int j = 0; j < m; j++) {
        cout << tasksNumAssigned[j] << "\t";
    }
    cout << endl;*/
    
    /*cout << "Allocation of tasks over procs:" << endl;
    for (int j = 0; j < m; j++) {
        cout << "proc. " << j << ": \t\t";
        for (int f = 0; f < tasksNumAssigned[j]; f++) {
            cout << Rset[j][f] << "\t";
        }
        cout << endl;
    }*/
    
    for (unsigned short k = 0; k < m; k++) {
        //cout << "assigning to proc. " << k << endl;
        unsigned short Rik = get_Rik(i, k, ts);
        //cout << "Rik: " << Rik << endl;
        
        if (Rik <= ts.D[i]) {
            //cout << "sched. cond. holds; task is assigned to proc. " << k << endl;
            // task i is assigned to processor k
            Rset[k][tasksNumAssigned[k]] = i;
            tasksNumAssigned[k]++;
            return true;
        }
    }
    
    return false;
}




bool split_task_accross_processors(const TS& ts, const unsigned short m, const unsigned short s) {
    
    //cout << "splitting task" << endl;
    
    taskSplitted[s] = 1;
    
    int c_req = ts.C[s];
    int c_sk = 0;
    int x = 0;
    
    for (unsigned short k = 0; k < m; k++) {
        
        c_sk = 0; // fraction of exec. time assigned to proc. k
        
        //cout << "allocating capacity to proc. " << k << endl;
        
        if (!procHasSpaceCapacity[k]) {
            //cout << "proc. " << k << " has no spare capacity" << endl;
            continue;
        }
        
        //cout << "proc. " << k << " has spare capacity" << endl;
        //cout << "tasks num. assigned to proc. " << k << ": " << tasksNumAssigned[k] << endl;
        
        for (unsigned short taskItr = 0; taskItr < tasksNumAssigned[k]; taskItr++) {
            unsigned short i = Rset[k][taskItr];
            //cout << "computing x for task " << i << endl;
            if (taskSplitted[i]) {
                //cout << "task " << i << " is splitted" << endl;
                x = (ts.D[i] - cFracOfTaskSplittedPerProc[k][i])/(ceil((float)ts.D[i]/ts.P[s]));
            } else {
                //cout << "task " << i << " is not splitted" << endl;
                x = (ts.D[i] - get_Rik(i,k,ts))/(ceil((float)ts.D[i]/ts.P[s]));
            }
            //cout << "x: " << x << endl;
            
            if ((x < c_sk)||(c_sk == 0)) c_sk = max(0,x);
            //cout << "updated c_sk: " << c_sk << endl;
        }
        
        if (c_sk !=0) {
            //cout << "c_sk != 0" << endl;
            //cout << "assigning fraction of task " << s << " to proc. " << k << endl;
            
            Rset[k][tasksNumAssigned[k]] = s;
            tasksNumAssigned[k]++;
            c_req = c_req - c_sk;
            //cout << "updated c_req: " << c_req << endl;
            
            if (c_req == 0) {
                procHasSpaceCapacity[k] = 0;
                cFracOfTaskSplittedPerProc[k][s] = c_sk;
                //cout << "proc. " << k << "does not have a spare capacity anymore" << endl;
                //cout << "task " << s << "is successully splitted" << endl;
                return true; // task s is successfully splitted
            } else if (c_req < 0) {
                c_sk = c_sk + c_req; // here, c_req < 0, thus we negate in fact
                //cout << "s_sk updated: " << c_sk << endl;
                cFracOfTaskSplittedPerProc[k][s] = c_sk;
                //cout << "proc. " << k << "still has spare capacity" << endl;
                //cout << "task " << s << "is splitted successfully" << endl;
                return true; // task s is assigned, as well as proc. k still has spare capacity
            } else {
                procHasSpaceCapacity[k] = 0;
                cFracOfTaskSplittedPerProc[k][s] = c_sk;
                //cout << "proc. " << k << " has no spare capacity anymore" << endl;
                //cout << "task has remaining exec. requirement to bes assigned: " << c_req << endl;
            }
        }
    }
    
    //cout << "task could not be splitted" << endl;
    return false;
}





bool semipfp_test(const unsigned short m, const TS& tsNotSorted){
    
    // sort tasks by decreasing density first
    unsigned short tasksSortedByDecrDensity[tsNotSorted.n];
    sort_tasks_by_decreasing_density(tsNotSorted, tasksSortedByDecrDensity);
    
    TS ts;
    ts.n = tsNotSorted.n;
    for (unsigned i = 0; i < ts.n; i++) {
        ts.setTask(i, tsNotSorted.C[tasksSortedByDecrDensity[i]], tsNotSorted.D[tasksSortedByDecrDensity[i]], tsNotSorted.P[tasksSortedByDecrDensity[i]]);
    }
    
    
    /*cout << "partitioned sched. for task set:" << endl;
    for (int i = 0; i < ts.n; i++) {
        cout << "task " << i << ": ( " << ts.C[i] << ", " << ts.D[i] << ", " << ts.P[i] << " )" << endl;
    }*/

    for (unsigned short k = 0; k < m; k++) {
        tasksNumAssigned[k] = 0;
        procHasSpaceCapacity[k] = 1;
    }
    
    for (unsigned short i = 0; i < ts.n; i++) {
        taskSplitted[i] = 0;
    }
    
    // assign tasks
    //cout << "assigning tasks" << endl;
    for (unsigned short i = 0; i < ts.n; i++) {
        //cout << "task " << i << ": ( " << ts.C[i] << ", " << ts.D[i] << ", " << ts.P[i] << " )" << endl;
        if (!assign_task_to_processor(ts, m, i)) {
            //cout << "task " << i << " cannot be assigned to any of the processors" << endl;
            //cout << "splitting task " << i << endl;
            if (!split_task_accross_processors(ts, m, i)) {
                // task i is unschedulable
                //cout << "task " << i << " cannot be splitted, unschedulable" << endl;
                return false;
            }
        }
    }
    
    return true;
}
