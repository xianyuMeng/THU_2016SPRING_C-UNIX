#ifndef _INCLUDE_WEIGHT_HEADER
#define _INCLUDE_WEIGHT_HEADER
#include <stdio.h>
#include <iostream>
#include <tuple>
#include <map>
using namespace std;

enum Status {
	light,
	heavy
};

class WeightGame {
public:
	WeightGame(int position, Status status);
	~WeightGame() {};
	static map<tuple<int, int, int>, pair<int, Status>> table;
	static bool initialized;
	static void initialize_table() {
		if (!initialized) {
			initialized = 1;
			table.insert(make_pair(make_tuple(-1, -1, 1), make_pair(1, heavy)));
			table.insert(make_pair(make_tuple(1, 1, -1), make_pair(1, light)));
			table.insert(make_pair(make_tuple(-1, 1, 1), make_pair(2, heavy)));
			table.insert(make_pair(make_tuple(1, -1, -1), make_pair(2, light)));
			table.insert(make_pair(make_tuple(-1, 1, 0), make_pair(3, heavy)));
			table.insert(make_pair(make_tuple(1, -1, 0), make_pair(3, light)));
			table.insert(make_pair(make_tuple(-1, 0, -1), make_pair(4, heavy)));
			table.insert(make_pair(make_tuple(1, 0, 1), make_pair(4, light)));
			table.insert(make_pair(make_tuple(1, -1, 1), make_pair(5, heavy)));
			table.insert(make_pair(make_tuple(-1, 1, -1), make_pair(5, light)));
			table.insert(make_pair(make_tuple(1, 1, 0), make_pair(6, heavy)));
			table.insert(make_pair(make_tuple(-1, -1, 0), make_pair(6, light)));
			table.insert(make_pair(make_tuple(1, 0, 0), make_pair(7, heavy)));
			table.insert(make_pair(make_tuple(-1, 0, 0), make_pair(7, light)));
			table.insert(make_pair(make_tuple(1, 0, -1), make_pair(8, heavy)));
			table.insert(make_pair(make_tuple(-1, 0, 1), make_pair(8, light)));
			table.insert(make_pair(make_tuple(0, -1, -1), make_pair(9, heavy)));
			table.insert(make_pair(make_tuple(0, 1, 1), make_pair(9, light)));
			table.insert(make_pair(make_tuple(0, 1, -1), make_pair(10, heavy)));
			table.insert(make_pair(make_tuple(0, -1, 1), make_pair(10, light)));
			table.insert(make_pair(make_tuple(0, -1, 0), make_pair(11, heavy)));
			table.insert(make_pair(make_tuple(0, 1, 0), make_pair(11, light)));
			table.insert(make_pair(make_tuple(0, 0, 1), make_pair(12, heavy)));
			table.insert(make_pair(make_tuple(0, 0, -1), make_pair(12, light)));
		}
	}
	int pos_of_problem;
private:
	int position;
	Status status;
	tuple<int, int, int> result;
	int first_round_result;
	int second_round_result;
	int third_round_result;
};

#endif _INCLUDE_WEIGHT_HEADER
