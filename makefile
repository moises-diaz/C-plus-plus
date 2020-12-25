all: dc dp1 dp2 clean
.PHONY : all 

dc: DC.o mystmCtr.o
	g++ -o dc DC.o mystmCtr.o
dp1: DP-1.o mystmCtr.o
	g++ -o dp1 DP-1.o mystmCtr.o
dp2: DP-2.o mystmCtr.o
	g++ -o dp2 DP-2.o mystmCtr.o
DC.o: DC.cpp
	g++ -c DC.cpp
DP-1.o: DP-1.cpp
	g++ -c DP-1.cpp
DP-2.o: DP-2.cpp
	g++ -c DP-2.cpp
mystmCtr.o: mystmCtr.cpp mystmCtr.h
	g++ -c mystmCtr.cpp
clean:
	rm DC.o DP-1.o DP-2.o mystmCtr.o
