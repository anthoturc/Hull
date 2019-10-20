CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -g
WXCOMS=`wx-config --libs` `wx-config --cxxflags`
INCLUDE=-I./

TEST_DIR=./tests/
app : visualization.cpp
	$(CC) $(CFLAGS) ${INCLUDE} visualization.cpp graham_scan.cpp $(WXCOMS) -o app

test : ${TEST_DIR}graham_scan_tests.cpp
	$(CC) $(CFLAGS) ${INCLUDE} ${TEST_DIR}graham_scan_tests.cpp graham_scan.cpp -o test

.PHONY : clean
clean:
	rm -f *.o app test
	rm -rf  *.dSYM

