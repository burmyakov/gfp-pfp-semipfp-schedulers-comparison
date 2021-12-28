#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <math.h>
#include "custom_types/ts.h"
#include <ilcplex/ilocplex.h>



using namespace std;
//vector<unsigned int> Pas;


ILOSTLBEGIN
IloEnv env;
IloModel model(env);
IloCplex cplex(model);
IloArray<IloBoolVarArray> Bvars;
IloArray<IloArray<IloBoolVarArray> > Ys;
int Zconst = 1000000; // const to model alternative constraints in CPLEX



void compute_Pas(vector<unsigned int>& Pas, const TS& ts, const unsigned short k) {
    
    // compute release instants for all HP tasks
    // assuming a synchronous periodic scenario
    Pas.push_back(ts.D[k]);
    for (unsigned short taskItr = 0; taskItr < k; taskItr++) {
        for (unsigned short l = 1; l*ts.P[taskItr] < ts.D[k]; l++) {
            Pas.push_back(l*ts.P[taskItr]);
        }
    }
    
    sort(Pas.begin(), Pas.end(), greater<unsigned int>());
    vector<unsigned int>::iterator ip = std::unique(Pas.begin(), Pas.end());
    Pas.resize(distance(Pas.begin(), ip));
    
    //cout << "Computed Pas: ";
    //for (int i = 0; i < Pas.size(); i++) cout << Pas[i] << "  "; cout << endl;
}





void generate_task_constraints(const TS& ts, const unsigned short m, const unsigned short k){
    
    Ys[k] = IloArray<IloBoolVarArray>(env, m);
    
    vector<unsigned int> Pas;
    compute_Pas(Pas, ts, k);
    
    // Schedulability conditions, proc. 0
    int PasSize = Pas.size();
    for (unsigned short j = 0; j < m; j++) {
        Ys[k][j] = IloBoolVarArray(env, PasSize);
        model.add(IloSum(Ys[k][j]) <= PasSize-1);
        
        // print constraint
        /*cout << "constraint: ";
        for (int i = 0; i < Pas.size(); i++) {
            cout << "Ys[" << k << "][" << j << "][" << i << "] + ";
        }
        cout << " <= " << Pas.size() - 1 << endl;*/
    }
    
    
    unsigned short t0;
    for (unsigned short t0Itr = 0; t0Itr < Pas.size(); t0Itr++) {
        t0 = Pas[t0Itr];
        //cout << "sched. instant: " << t0 << endl;
        
        for (unsigned short procItr = 0; procItr < m; procItr++) {
            //cout << "proc.: " << procItr << endl;
            
            IloExpr schedConstr_t0(env);
            //cout << "constraint: ";
            schedConstr_t0 = ts.C[k]*Bvars[k][procItr];
            //cout << ts.C[k] << "*Bvars[" << k << "][" << procItr << "] ";
            for (unsigned short taskItr = 0; taskItr < k; taskItr++) {
                schedConstr_t0 += ceil((float)t0/ts.P[taskItr])*ts.C[taskItr]*Bvars[taskItr][procItr];
                //cout << "+ " << ceil((float)t0/ts.P[taskItr]) << "*" << ts.C[taskItr] << "*Bvars[" << taskItr << "][" << procItr << "] ";
            }
            schedConstr_t0 -= (Zconst/2)*Ys[k][procItr][t0Itr];
            //cout << "- " << Zconst << "*Ys[" << k << "][" << procItr << "][" << t0Itr << "] ";
            schedConstr_t0 -= Bvars[k][procItr]*t0;
            //cout << "- " << t0 << "*Bvars[" << k << "][" << procItr << "] ";
            model.add(schedConstr_t0 <= (1 - Bvars[k][procItr])*Zconst);
            //cout << " <= (1 - Bvars[" << k << "][" << procItr << "])*10000000" << endl;
            schedConstr_t0.end();
        }
    }
}




int main(int argc, char* argv[]){
    
    bool schedBILP = true;
    
    TS ts;
    bool removeStates = false;

    unsigned short m = (unsigned short) strtoul(argv[1], NULL, 0);
    unsigned short n = (unsigned short) strtoul(argv[2], NULL, 0);
    
    ts.n = n;
    int C, D, P;
    for (int i = 0; i < n; i++) {
        C = (int) strtoul(argv[3+3*i], NULL, 0);
        D = (int) strtoul(argv[4+3*i], NULL, 0);
        P = (int) strtoul(argv[5+3*i], NULL, 0);
        ts.setTask(i, C, D, P);
    }
    
    bool verbose = (bool) strtoul(argv[4+3*n], NULL, 0);

    cplex.setOut(env.getNullStream());
    
    
    /*cout << "MILP testing task set:" << endl;
    for (int i = 0; i < ts.n; i++) {
        cout << "( " << ts.C[i] << ", " << ts.D[i] << ", " << ts.P[i] << " )" << endl;
    }*/
    
    
    try {
        
        Bvars = IloArray<IloBoolVarArray>(env, ts.n);
        for (IloInt i = 0; i < ts.n; i++) {
            Bvars[i] = IloBoolVarArray(env, m);
            model.add(IloSum(Bvars[i]) == 1);
            
            // print constraint
            /*cout << "constraint: ";
            for (int j = 0; j < m; j++) {
                cout << "Bvars[" << i << "][" << j << "] + ";
            }
            cout << " == 1" << endl;*/
        }
        
        Ys = IloArray<IloArray<IloBoolVarArray> >(env, n);
        
        
        for (unsigned short taskItr = 1; taskItr < ts.n; taskItr++) { // to replace "1" with "m-1"
            //cout << "constraints for task: " << taskItr << endl;
            generate_task_constraints(ts, m, taskItr);
        }
        
        if (cplex.solve()) {
            cout << "B:" << endl;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    cout << cplex.getValue(Bvars[i][j]) << "\t";
                }
                cout << endl;
            }
            
            /*cout << "Y:" << endl;
            for (int i = 1; i < n; i++) {
                cout << "task " << i << endl;
                for (int j = 0; j < m; j++) {
                    for (int k = 0; k < 5; k++) {
                        cout << cplex.getValue(Ys[i][j][k]) << "\t";
                    }
                    cout << endl;
                }
            }*/
        } else {
            //cout << "The problem is infeasible!" << endl;
            schedBILP = false;
        }
        
    } catch (IloException e) {
        cout << e << endl;
        abort();
    }
    
    cplex.end();
    model.end();
    env.end();
    
    return schedBILP;
}
