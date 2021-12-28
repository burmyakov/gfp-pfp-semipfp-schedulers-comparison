#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include "custom_types/ts.h"
#include "fp_test_joseph_pandya.h"



using namespace std;




std::string int_to_string(const int i) {
    stringstream ss;
    string s;
    ss << i;
    s = ss.str();
    
    return s;
}





// Sort tasks in the order of decreasing density
void sort_tasks_by_decreasing_density(const TS& ts, unsigned short* tasksSortedByDecrDensity) {

    for (unsigned short itrTask = 0; itrTask < ts.n; ++itrTask)
        tasksSortedByDecrDensity[itrTask] = itrTask;

    unsigned short curTask1, curTask2;
    for (unsigned short itr1 = 0; itr1 < ts.n; ++itr1)
        for (unsigned short itr2 = itr1+1; itr2 < ts.n; ++itr2) {
            curTask1 = tasksSortedByDecrDensity[itr1];
            curTask2 = tasksSortedByDecrDensity[itr2];
            if ((double)ts.C[curTask1]/ts.D[curTask1] < (double)ts.C[curTask2]/ts.D[curTask2]) {
                swap(tasksSortedByDecrDensity[itr1], tasksSortedByDecrDensity[itr2]);
            }
        }
}




// Sort tasks by DM priorities
void sort_tasks_by_DM(const TS& ts, vector<unsigned short>& procTasks) {
    
    for (unsigned short itr1 = 0; itr1 < procTasks.size(); ++itr1)
        for (unsigned short itr2 = itr1+1; itr2 < procTasks.size(); ++itr2)
            if (ts.D[procTasks[itr1]] > ts.D[procTasks[itr2]])
                swap(procTasks[itr1], procTasks[itr2]);
}




// Test schedulability (construct an input string and call an exact test)
bool test_schedulability(const int m, const TS& ts) {

    // 1. Construct input string
    string inputs = int_to_string(m) + " " + int_to_string(ts.n);
    for (int i = 0; i < ts.n; i++) {
        inputs += " " + int_to_string(ts.C[i]) + " " + int_to_string(ts.D[i]) + " " + int_to_string(ts.P[i]);
    }
    
    // 2. Call test
    string fc;
    switch (m) {
        case 1:     fc = "v21_gfp_exact_sched_test/any_processors_number/./gfp_test " + inputs + " 0 0 "; break;
        case 2:     fc = "v21_gfp_exact_sched_test/2processors/./gfp_test " + inputs + " 0 0 "; break;
        case 3:     fc = "v21_gfp_exact_sched_test/3processors/./gfp_test " + inputs + " 0 0 "; break;
        case 4:     fc = "v21_gfp_exact_sched_test/4processors/./gfp_test " + inputs + " 0 0 "; break;
        case 6:     fc = "v21_gfp_exact_sched_test/6processors/./gfp_test " + inputs + " 0 0 "; break;
        case 8:     fc = "v21_gfp_exact_sched_test/8processors/./gfp_test " + inputs + " 0 0 "; break;
        default:    cout << "No test defined for m = " << m << " processors!" << endl;
    }
    
    int ret = system(fc.c_str());
    bool sched = WEXITSTATUS(ret);
    
    /*fc = "gfp_exact_sched_test_2019/./gfp_test " + inputs + " 0 0 ";
    ret = system(fc.c_str());
    bool sched2018 = WEXITSTATUS(ret);
    if (sched2018 != sched) {
        cout << "Results differ!!!" << endl;
        cout << inputs << endl;
        cout << "2021 test: " << sched << endl;
        cout << "2019 test: " << sched2018 << endl;
        //abort();
    }*/
    
    return sched;
}




// Test schedulability of tasks assigned to the same partition
bool test_partition_schedulability(const TS& ts, vector<unsigned short>& procTasks) {
    
    // Sort tasks by DM
    sort_tasks_by_DM(ts, procTasks);
    
    // Collect tasks
    TS tsPartition;
    tsPartition.n = 0;
    int curC, curD, curP;
    
    for (unsigned short itrTask = 0; itrTask < procTasks.size(); ++itrTask) {
        
        curC = ts.C[procTasks[itrTask]];
        curD = ts.D[procTasks[itrTask]];
        curP = ts.P[procTasks[itrTask]];
        
        tsPartition.setTask(tsPartition.n, curC, curD, curP);
        tsPartition.n++;
    }
    
    bool schedJosephPandya = fp_test_joseph_pandya(tsPartition);
    
    return schedJosephPandya;
}




// Test schedulability of tasks assigned to the same partition
/*bool test_pfp_schedulability_MILP(const TS& ts, const unsigned short m) {
    
    // Sort tasks by DM
    //sort_tasks_by_DM(ts, procTasks);
    
    // 1. Construct input string
    string inputs = int_to_string(m) + " " + int_to_string(ts.n);
    for (int i = 0; i < ts.n; i++) {
        inputs += " " + int_to_string(ts.C[i]) + " " + int_to_string(ts.D[i]) + " " + int_to_string(ts.P[i]);
    }
    
    // 2. Call MILP test for partitioned schedulability
    string fc = "./pfp_test_MILP " + inputs + " 0 0 ";
    int ret = system(fc.c_str());
    bool schedMILP = WEXITSTATUS(ret);
    
    return schedMILP;
}*/





// Test schedulability of tasks assigned to the same partition
/*bool test_partition_schedulability_old(const TS& ts, vector<unsigned short>& procTasks) {
    
    // Sort tasks by DM
    sort_tasks_by_DM(ts, procTasks);
    
    // Collect tasks
    TS tsPartition;
    tsPartition.n = 0;
    int curC, curD, curP;
    
    for (unsigned short itrTask = 0; itrTask < procTasks.size(); ++itrTask) {
        
        curC = ts.C[procTasks[itrTask]];
        curD = ts.D[procTasks[itrTask]];
        curP = ts.P[procTasks[itrTask]];
        
        tsPartition.setTask(tsPartition.n, curC, curD, curP);
        tsPartition.n++;
    }
    
    // Test schedulability for uniprocessor, DM
    const unsigned short m = 1;
    bool sched = test_schedulability(m, tsPartition);
    
    cout << "testing new test" << endl;
    if (sched != test_partition_schedulability(ts, procTasks)) {
        cout << "problem" << endl;
        cout << "sched: " << sched << endl;
        abort();
    }
    
    return sched;
}*/
