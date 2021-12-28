#include <iostream>
#include <string>
#include <fstream>
#include "custom_types/ts.h"
#include "tasks/test_3d_task.h"
#include "tasks/test_4th_task.h"
#include "tasks/test_5th_task.h"
#include "tasks/test_6th_task.h"
#include "tasks/test_7th_task.h"
#include "tasks/test_8th_task.h"
/*#include "tasks/test_9th_task.h"
#include "tasks/test_10th_task.h"
#include "tasks/test_11th_task.h"
#include "tasks/test_12th_task.h"
#include "tasks/test_13th_task.h"
#include "tasks/test_14th_task.h"*/


using namespace std;

ofstream fileResults;



int main(int argc, char* argv[]) {
    
    TS tsOriginal, ts;

    unsigned short m = (unsigned short) strtoul(argv[1], NULL, 0);
    unsigned short n = (unsigned short) strtoul(argv[2], NULL, 0);
    
    tsOriginal.n = n;
    int C, D, P;
    for (int i = 0; i < n; i++) {
        C = (int) strtoul(argv[3+3*i], NULL, 0);
        D = (int) strtoul(argv[4+3*i], NULL, 0);
        P = (int) strtoul(argv[5+3*i], NULL, 0);
        //cout << "C[" << i << "]: " << C << "\t D[" << i << "]: " << D << "\t P[" << i << "]: " << P << endl;
        tsOriginal.setTask(i, C, D, P);
    }
    
    
    //bool removeDominatedStatesFromMap = (bool) strtoul(argv[3+3*n], NULL, 0);
    bool verbose = (bool) strtoul(argv[4+3*n], NULL, 0);
    verbose = 0;

    
    for (int i = 0; i < m; i++) ts.setTask(i, tsOriginal.C[i], tsOriginal.D[i], tsOriginal.P[i]);
    
    unsigned long int savedStatesNum = 0;
    unsigned long int visitedStatesNum = 0;
    unsigned long int savedStatesNumIncr = 0;
    unsigned long int visitedStatesNumIncr = 0;
    
    bool sched = true;
    
    unsigned long int t0;
    unsigned long int tExecution_p1;
    unsigned long int tExecutionTotal_p1 = 0;
    
    
    for (int N = m + 1; N <= tsOriginal.n; N++) {
        ts.n = N;
        ts.setTask(N-1, tsOriginal.C[N-1], tsOriginal.D[N-1], tsOriginal.P[N-1]);

        if (verbose) {
            cout << endl << "===================" << endl;
            cout << "Checking task " << (int)ts.n << endl;
        }

        switch (N) {
            case 3:
                t0 = clock();
                sched = test_3d_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;
                
            case 4:
                t0 = clock();
                sched = test_4th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;
                
            case 5:
                t0 = clock();
                sched = test_5th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;
                
            case 6:
                t0 = clock();
                sched = test_6th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 7:
                t0 = clock();
                sched = test_7th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 8:
                t0 = clock();
                sched = test_8th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            /*case 9:
                t0 = clock();
                sched = test_9th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 10:
                t0 = clock();
                sched = test_10th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 11:
                t0 = clock();
                sched = test_11th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 12:
                t0 = clock();
                sched = test_12th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 13:
                t0 = clock();
                sched = test_13th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;

            case 14:
                t0 = clock();
                sched = test_14th_task(verbose, m, ts, savedStatesNumIncr, visitedStatesNumIncr);
                tExecution_p1 = clock() - t0; tExecutionTotal_p1 += tExecution_p1;

                savedStatesNum += savedStatesNumIncr; visitedStatesNum += visitedStatesNumIncr;
                break;*/

                
            default: cout << "No function defined to test " << N << " tasks!" << endl;
        }

        if (!sched) break;
    }

    
    /*if (!removeDominatedStatesFromMap) cout << "Sporadic burm2018 p1 (no r.):\t" << (float)(tExecutionTotal_p1*100/CLOCKS_PER_SEC)/100 << " sec,  \t / " << savedStatesNum << " saved states" << "  \t / " << visitedStatesNum << " visited states";
    else cout << "Sporadic burm2018 p1:\t\t" << (float)(tExecutionTotal_p1*100/CLOCKS_PER_SEC)/100 << " sec,  \t / " << savedStatesNum << " saved states" << "  \t / " << visitedStatesNum << " visited states";
    
    if (sched) cout << "  \t / SCHED" << endl;
    else cout << "  \t / UNSCHED" << endl;*/

    return sched;
}
