GPP = g++-4.7

ifeq (`which g++-4.7`,)
	GPP = g++
endif

all: crp_test

crp_test: crp_test.cc
	$(GPP) -std=c++11 -O3 -Wall crp_test.cc -o crp_test
