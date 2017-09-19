CXX=g++
CFLAGS=-fPIC -D_REENTRANT -I/usr/java/jdk1.6.0_38/include/linux -I/usr/java/jdk1.6.0_38/include -c
LIBS=-shared


libgoodluck.so: bloom_filter.o murmur.o dablooms.o
	$(CXX) -o $@ $^ $(LIBS)

bloom_filter.o: bloom_filter.cpp
	$(CXX) -o $@ $^ $(CFLAGS)

murmur.o: murmur.cpp
	$(CXX) -o $@ $^ $(CFLAGS)

dablooms.o: dablooms.cpp
	$(CXX) -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f *.o libgoodluck.so
