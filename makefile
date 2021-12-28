
CCLNFLAGS = -m64 -lm -lpthread -framework CoreFoundation -framework IOKit  -std=c++17

OPTIONS = -ansi -Wall -Wno-sign-compare

AUX = custom_types/ts.cpp utilities.cpp gfp_test_dm.cpp gfp_test_dcmpo.cpp gfp_test_dkc.cpp gfp_test_dc.cpp gfp_test_dd.cpp gfp_test_opa.cpp pfp_test_ffd.cpp pfp_test_wfd.cpp fp_test_joseph_pandya.cpp pfp_test_optimal.cpp semipfp_test.cpp

all: evaluation

evaluation: evaluation.cpp $(AUX)
	g++ $(OPTIONS) -Ofast -o evaluation evaluation.cpp $(AUX) $(CCLNFLAGS)

clean: 
	rm evaluation





