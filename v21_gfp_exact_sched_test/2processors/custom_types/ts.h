#include <algorithm>
#include <cassert>

struct TS {
	// Task system
    unsigned short n; // n. of tasks -- must be less than MAXN
	static const unsigned short MAXN = 16;
    unsigned short C[MAXN]; // WCETs
    unsigned short D[MAXN]; // deadlines
    unsigned short P[MAXN]; // minimum interarrival times
    unsigned short WRTub[MAXN]; // the upper bound for the worst-case response times
	
    unsigned short pmax; // max(P_i) (buffered value)
	TS() { }
    
	TS(unsigned short n_) : n(n_), pmax(0) { assert(n <= MAXN); }
    
	void setTask(unsigned short i, unsigned short Ci, unsigned short Di, unsigned short Pi) {
		C[i] = Ci;
		D[i] = Di;
		P[i] = Pi;
        
		pmax = std::max(pmax, Pi);
        
		assert(C[i] <= D[i]); 
		assert(D[i] <= P[i]); // * constrained deadlines *
	}
    
    unsigned short Pmax() const { return pmax; }
	void read();
};

