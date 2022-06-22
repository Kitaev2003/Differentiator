all:
	g++ -g -Wall -Warray-bounds -Wdouble-promotion -Wold-style-cast -Wfloat-equal -Wextra -Wpedantic -std=c++11 Differentiator.cpp Differentiator_Function.cpp Equaction_Read.cpp Equaction_Reduction.cpp -o start
