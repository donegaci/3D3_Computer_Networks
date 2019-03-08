CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread  $(CXXOPTIMIZE)
USERID=CiaranDonegan_16322253
CLASSES=

all: web-server web-client

web-server: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

web-client: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM web-server web-client *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
