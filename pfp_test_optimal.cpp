#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <math.h>
#include "custom_types/ts.h"
#include "utilities.h"
#include "fp_test_joseph_pandya.h"



using namespace std;

unsigned short** partitions;



unsigned long generate_feasible_partitions(const unsigned short m, const TS& ts) {
    
    const unsigned short n = ts.n;
    unsigned long partNum = pow(m,n);
    
    // init array of feasible partitions
    //unsigned short** partitions;
    partitions = new unsigned short* [n];
    for (unsigned short taskItr = 0; taskItr < n; taskItr++)
        partitions[taskItr] = new unsigned short [partNum];
    
    
    // generate feasible partitions
    for (unsigned short taskItr = 0; taskItr < n; taskItr++) {
        unsigned long itr1max = pow(m, taskItr);
        unsigned long itr3max = pow(m, n - (taskItr + 1));
        for (unsigned short itr1 = 0; itr1 < itr1max; itr1++) {
            for (unsigned short itr2 = 0; itr2 < m; itr2++) {
                for (unsigned short itr3 = 0; itr3 < itr3max; itr3++) {
                    partitions[taskItr][itr1*((unsigned long)pow(m, n-taskItr)) + itr2*itr3max + itr3] = itr2;
                }
            }
        }
    }
    
    return partNum;
}




float compute_U(const TS& ts) {
    
    const unsigned short n = ts.n;
    float U = 0;
    
    for (unsigned short taskItr = 0; taskItr < n - 1; taskItr++) {
        U += ((float)ts.C[taskItr])/ts.P[taskItr];
    }
    U += ((float)ts.C[n-1])/ts.D[n-1];
    
    return U;
}




bool check_cur_partition_schedulability(const TS& tsPerProc) {
    
    bool sched = true;
    float U = compute_U(tsPerProc);
    
    // Check necessary schedulability and sufficient unschedulability conditions
    if (U > 1) return sched = false;
    else if (U <= tsPerProc.n*(pow(2, (float)(1)/tsPerProc.n) - 1)) return sched = true;
    
    // Call an exact test instead of Joseph and Pandya
    return sched = fp_test_joseph_pandya(tsPerProc);
}




bool check_partitioning_schedulability(const unsigned short m, const TS& ts, const unsigned long partItr) {
    
    vector<TS> tsPerProc(m);
    vector<unsigned short> tasksPerProcNum(m);
    
    for (unsigned short procItr = 0; procItr < m; procItr++)
        tasksPerProcNum[procItr] = 0;
    
    for (unsigned short taskItr = 0; taskItr < ts.n; taskItr++) {
        tsPerProc[partitions[taskItr][partItr]].setTask(tasksPerProcNum[partitions[taskItr][partItr]], ts.C[taskItr], ts.D[taskItr], ts.P[taskItr]);
        tasksPerProcNum[partitions[taskItr][partItr]] = tasksPerProcNum[partitions[taskItr][partItr]] + 1;
    }
    
    for (unsigned short procItr = 0; procItr < m; procItr++) {
        tsPerProc[procItr].n = tasksPerProcNum[procItr];
    }
    
    
    // check if the partition is schedulable
    bool partitionsSched = false;
    for (unsigned short procItr = 0; procItr < m; procItr++) {
        partitionsSched = check_cur_partition_schedulability(tsPerProc[procItr]);
        if (!partitionsSched) return false;
    }
    
    return true;
}






bool find_optimal_pfp_partitioning_JosephPandya(const unsigned short m, const TS& ts){
    
    // Generate feasible partitions
    const unsigned long partitionsNum = generate_feasible_partitions(m, ts);
    
    // test schedulability of each partition
    bool schedPartitionFound = false;
    for (unsigned long partItr = 0; partItr < partitionsNum; partItr++) {
        schedPartitionFound = check_partitioning_schedulability(m, ts, partItr);
        if (schedPartitionFound) {
            /*cout << "sched. partition found: " << endl;
            for (int i = 0; i < ts.n; i++) {
                cout << "task " << i << ": proc. " << partitions[i][partItr] << endl;
            }*/
            break;
        }
    }
    
    return schedPartitionFound;
}






bool pfp_test_optimal(const unsigned short m, const TS& ts){
    
    //bool schedJosephPandya = find_optimal_pfp_partitioning_JosephPandya(m, ts);
    //cout << "Joseph Pandya: " << schedJosephPandya << endl;
    
    // MILP Approach
    
    // Sort tasks by DM
    //sort_tasks_by_DM(ts, procTasks);
    
    // 1. Construct input string
    string inputs = int_to_string(m) + " " + int_to_string(ts.n);
    for (int i = 0; i < ts.n; i++) {
        inputs += " " + int_to_string(ts.C[i]) + " " + int_to_string(ts.D[i]) + " " + int_to_string(ts.P[i]);
    }
    
    // 2. Call MILP test for partitioned schedulability
    string fc = "./pfp_test_BILP " + inputs + " 0 0 ";
    int ret = system(fc.c_str());
    bool schedMILP = WEXITSTATUS(ret);
    
    /*if (schedMILP != schedJosephPandya) {
        cout << "MILP differs from JosephPandya!===============================================================================================" << endl;
        cout << "MILP: " << schedMILP << endl;
        cout << "JosephPandya: " << schedJosephPandya << endl;
        abort();
    }*/
    
    return schedMILP;
}
