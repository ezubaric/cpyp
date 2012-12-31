GPP = g++-4.7

ifeq (`which g++-4.7`,)
	GPP = g++
endif

clean:
	rm *.o hpyplm dhpyplm dhpyplm_train dhpyplm_query incremental_query

all: hpyplm dhpyplm

hpyplm: hpyplm.cc
	g++ -std=c++11 -O3 -Wall -I.. hpyplm.cc -o hpyplm

dhpyplm: dhpyplm.cc
	g++ -std=c++11 -O3 -g -Wall -I.. dhpyplm.cc -o dhpyplm

## stuff below here is optional

BOOST_ROOT=/opt/local/include/boost
BOOST_INCLUDE=$(BOOST_ROOT)/include
BOOST_SERIALIZATION=-lboost_serialization-mt

INCLUDEDIRS =-I../../../ -I/opt/local/include/ \
	-I/fs/clip-software/boost_1_40_0/include -I..

LIBDIRS =-L/usr/local/lib/ \
	 -L/opt/local/lib -L/fs/clip-software/boost_1_40_0/$(ARCH)/lib/

hpyplm_train: hpyplm_train.cc
	$(GPP) -std=c++11 -O3 -Wall $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)

hpyplm_query: hpyplm_query.cc
	$(GPP) -std=c++11 -O3 -Wall $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)

hpyplm_query_observe: hpyplm_query_observe.cc
	$(GPP) -std=c++11 -O3 -Wall $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)

incremental_query: incremental_query.cc
	$(GPP) -std=c++11 -Wall -DEBUG --save-temps -ggdb $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)
	#$(GPP) -std=c++11 -O3 -Wall $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)

dhpyplm_train: dhpyplm_train.cc
	$(GPP) -std=c++11 -O3 -Wall $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)

dhpyplm_query: dhpyplm_query.cc
	$(GPP) -std=c++11 -O3 -Wall $(INCLUDEDIRS) $(LIBDIRS) $< -o $@ $(BOOST_SERIALIZATION)

CDEC = ../../cdec
cdec_ff_hpyplm.o: cdec_ff_hpyplm.cc
	$(GPP) -shared -fPIC -std=c++11 -O3 -g -Wall $(INCLUDEDIRS) $(LIBDIRS) -I$(CDEC)/utils -I$(CDEC)/mteval -I$(CDEC)/decoder $< -c

libcdec_ff_hpyplm.so: cdec_ff_hpyplm.o
	$(GPP) -shared -Wl,-soname,$@ -o $@ $<

