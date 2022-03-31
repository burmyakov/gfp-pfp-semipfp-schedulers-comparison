//
//  main.cpp
//  GFPSchedulabilityTest_v0.3_cpp
//
//  Created by Artem Burmyakov on 22/11/14.
//  Copyright (c) 2014 Artem Burmyakov. All rights reserved.
//

#include <iostream>
#include "custom_types/ts.h"



using namespace std;

const unsigned short NMAX = 16;
unsigned short Rs[NMAX];
unsigned int Is_NC[NMAX];
unsigned int Is_CI[NMAX];
unsigned int Is_DIFF[NMAX];
unsigned int Is_DIFF_SORTED[NMAX];





/*unsigned short get_Wi_NC(const TS& ts, const unsigned short i, const unsigned short Rk) {
    
    int Wi_NC = 0;
    int Ni_NC = Rk/ts.P[i];
    
    Wi_NC = Ni_NC*ts.C[i] + min(ts.C[i], Rk - Ni_NC*ts.P[i]);
    Wi_NC = max(0, Wi_NC);

    return Wi_NC;
}


void get_Is_NC(const TS& ts, const unsigned short k, const unsigned short Rk) {
    
    for (int i = 0; i < k; i++) {
        int Wi_NC = get_Wi_NC(ts, i, Rk);
        Is_NC[i] = max(0, min(Wi_NC, Rk - ts.C[k] + 1));
    }
}




unsigned short get_Wi_CI(const TS& ts, const unsigned short i, const unsigned short Rk, const unsigned short* Rs) {
    
    int Wi_CI = 0;
    int Ni_CI = (Rk + Rs[i] - ts.C[i])/ts.P[i];
    
    Wi_CI = Ni_CI*ts.C[i] + min(ts.C[i], Rk + Rs[i] - ts.C[i] - Ni_CI*ts.P[i]);
    Wi_CI = max(0, Wi_CI);

    return Wi_CI;
}




void get_Is_CI(const TS& ts, const unsigned short k, const unsigned short Rk, const unsigned short* Rs) {
    
    for (int i = 0; i < k; i++) {
        int Wi_CI = get_Wi_CI(ts, k, Rk, Rs);
        Is_CI[i] = max(0, min(Wi_CI, Rk - ts.C[k] + 1));
    }
}





void get_Is_DIFF(int k) {

    for (int i = 0; i < k; i++) {
        Is_DIFF[i] = Is_CI[i] - Is_NC[i];
    }
    
    // sort by decreasing
    int x;
    for (int j = k-1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            if (i < j) {
                if (Is_DIFF[i] < Is_DIFF[i+1]) {
                    x = Is_DIFF[i];
                    Is_DIFF[i] = Is_DIFF[i+1];
                    Is_DIFF[i+1] = x;
                }
            }
        }
    }
}





unsigned short get_omega(const TS& ts, const unsigned short m, const unsigned short k, const unsigned short Rk, const unsigned short* Rs) {
    
    unsigned short Omega = 0;
    unsigned short terms = 0;
    unsigned short term1 = 0;
    unsigned short term2 = 0;
    
    get_Is_NC(ts, k, Rk);
    get_Is_CI(ts, k, Rk, Rs);
    get_Is_DIFF(k);
    
    for (int i = 0; i < k; i++) {
        term1 += Is_NC[i];
        term2 += Is_DIFF[i];
    }
    
    terms = term1 + term2;
    Omega = ts.C[k] + terms/m;

    return Omega;
}




void get_Rk(const TS& ts, const unsigned short m, const unsigned short k, unsigned short* Rs) {
    
    if (k < m) {
        Rs[k] = ts.C[k];
        return;
    } else {
    
        int Rk = 0;
        int Omega = ts.C[k];
    
        while (Rk != Omega) {
            Rk = Omega;

            if (Rk > ts.P[k]) {
                Rs[k] = Rk;
                return;
            }
        
            Omega = get_omega(ts, m, k, Rk, Rs);
        }

        Rs[k] = Rk;
        return;
    }
}*/



unsigned int get_Wi_NC(unsigned short i, unsigned short Rk, unsigned short Cs[NMAX], unsigned short Ps[NMAX]) {
    unsigned int Wi_NC = 0;
    unsigned int Ni_NC = Rk/Ps[i];
    
    Wi_NC = Ni_NC*Cs[i] + min((int)Cs[i], (int)Rk - (int)Ni_NC*Ps[i]);
    Wi_NC = max(0, (int)Wi_NC);
    
    //cout << "Wi_NC[" << i << "]: " << Wi_NC << endl;
    
    return Wi_NC;
}


void get_Is_NC(unsigned short k, unsigned short Rk, unsigned short Cs[NMAX], unsigned short Ps[NMAX]) {
    
    for (unsigned short i = 0; i < k; i++) {
        unsigned int Wi_NC = get_Wi_NC(i, Rk, Cs, Ps);
        Is_NC[i] = max(0, min((int)Wi_NC, (int)Rk - Cs[k] + 1));
        //cout << "Wi_NC[" << i << "]:" << Wi_NC << ", Is_NC[" << i << "]:" << Is_NC[i] << endl;
    }
}




unsigned int get_Wi_CI(unsigned short i, unsigned short Rk, unsigned short Cs[NMAX], unsigned short Ps[NMAX], unsigned short* rtGuan2009) {
    
    unsigned int Wi_CI = 0;
    unsigned int Ni_CI = (Rk + rtGuan2009[i] - Cs[i])/Ps[i];
    
    Wi_CI = Ni_CI*Cs[i] + min((int)Cs[i], (int)Rk + (int)rtGuan2009[i] - (int)Cs[i] - (int)Ni_CI*Ps[i]);
    Wi_CI = max(0, (int)Wi_CI);
    
    //cout << "Wi_CI[" << i << "]: " << Wi_CI << endl;
    
    return Wi_CI;
}




void get_Is_CI(unsigned short k, unsigned short Rk, unsigned short Cs[NMAX], unsigned short Ps[NMAX], unsigned short* rtGuan2009) {
    
    for (int i = 0; i < k; i++) {
        int Wi_CI = get_Wi_CI(i, Rk, Cs, Ps, rtGuan2009);
        Is_CI[i] = max(0, min((int)Wi_CI, (int)Rk - Cs[k] + 1));
        //cout << "Wi_CI[" << i << "]:" << Wi_CI << ", Is_CI[" << i << "]:" << Is_CI[i] << endl;
    }
}





void get_Is_DIFF(int k) {
    //cout << "Is_DIFF before sorting:" << endl;
    for (unsigned short i = 0; i < k; i++) {
        Is_DIFF[i] = Is_CI[i] - Is_NC[i];
        //cout << "Is_DIFF[" << i << "]:" << Is_DIFF[i] << endl;
    }
    
    // sort by decreasing
    int x;
    for (int j = k-1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            if (i < j) {
                if (Is_DIFF[i] < Is_DIFF[i+1]) {
                    x = Is_DIFF[i];
                    Is_DIFF[i] = Is_DIFF[i+1];
                    Is_DIFF[i+1] = x;
                }
            }
        }
    }
    
    /*cout << "Is_DIFF after sorting:" << endl;
    for (int i = 0; i < k; i++) {
        cout << "Is_DIFF[" << i << "]:" << Is_DIFF[i] << endl;
    }*/
}





unsigned short get_omega(unsigned short m, unsigned short k, unsigned short Rk, unsigned short Cs[NMAX], unsigned short Ps[NMAX], unsigned short* rtGuan2009) {
    
    unsigned short Omega = 0;
    unsigned int terms = 0;
    unsigned int term1 = 0;
    unsigned int term2 = 0;
    
    get_Is_NC(k, Rk, Cs, Ps);
    get_Is_CI(k, Rk, Cs, Ps, rtGuan2009);
    get_Is_DIFF(k);
    
    for (int i = 0; i < k; i++) {
        term1 += Is_NC[i];
        term2 += Is_DIFF[i];
    }
    
    terms = term1 + term2;
    
    Omega = Cs[k] + terms/m;
    //cout << "Omega: " << Omega << endl;
    
    return Omega;
}





void get_Rk(unsigned short m, unsigned short k, unsigned short Cs[NMAX], unsigned short Ps[NMAX], unsigned short* rtGuan2009) {
    
    if (k < m) {
        rtGuan2009[k] = Cs[k];
        return;
    }
    
    unsigned short Rk = 0;
    unsigned short Omega = Cs[k];
    
    while (Rk != Omega) {
        Rk = Omega;
        //cout << "k: " << k << ", Rk: " << Rk <<endl;
        if (Rk > Ps[k]) {
            rtGuan2009[k] = Rk;
            return;
        }
        
        Omega = get_omega(m, k, Rk, Cs, Ps, rtGuan2009);
    }
    //cout << "Rk = Omega; Rk: " << Rk << ", Omega: " << Omega << endl;
    
    rtGuan2009[k] = Rk;
}





unsigned short* response_time_Guan2009(const TS& ts, const unsigned short m) {

    unsigned short* rtGuan2009 = new unsigned short[ts.n];
    unsigned short* Cs = new unsigned short[ts.n];
    unsigned short* Ps = new unsigned short[ts.n];
    
    for (unsigned short k = 0; k < ts.n; k++) {
        Cs[k] = ts.C[k];
        Ps[k] = ts.P[k];
    }
    
    for (unsigned short k = 0; k < ts.n; k++) {
        //get_Rk(ts, m, k, rtGuan2009);
        get_Rk(m, k, Cs, Ps, rtGuan2009);
    }
    
    return rtGuan2009;
}
