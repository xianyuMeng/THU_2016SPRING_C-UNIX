#include "Header.h"

//map<tuple<int, int, int>, pair<int, Status>> WeightGame::table;

WeightGame::WeightGame(int position, Status status) :position(position), status(status) {
	const int number_of_balls = 12;
	const int weigh = 0;
	const int diff = 1;
	int ball[number_of_balls];
	for (size_t i = 0; i <= 11; ++i) {
		ball[i] = weigh;
	}

	ball[position - 1] = (status == light) ? -diff : diff;
	
	/*first round 
	1,2,3,4 vs 5,6,7,8
	*/
	int left = ball[0] + ball[1] + ball[2] + ball[3];
	int right = ball[4] + ball[5] + ball[6] + ball[7];
	this->first_round_result = (left == right) ? 0 : ((left > right) ? -1 : 1);
	/*second round 
	1, 5, 9, 11 vs 2, 3, 6, 10
	*/
	left = ball[0] + ball[4] + ball[8] + ball[10];
	right = ball[1] + ball[2] + ball[5] + ball[9];
	this->second_round_result = (left == right) ? 0 : ((left > right) ? -1 : 1);
	/*third round
	4, 8, 9, 10 vs 1, 2, 5, 12
	*/
	left = ball[3] + ball[7] + ball[8] + ball[9];
	right = ball[0] + ball[1] + ball[4] + ball[11];
	this->third_round_result = (left == right) ? 0 : ((left > right) ? -1 : 1);
	this->result = make_tuple(this->first_round_result, this->second_round_result, this->third_round_result);
	//WeightGame::initialized = false;
	WeightGame::initialize_table();
	cout << "the diffeent ball is number " << (this->table[this->result]).first << "\n";
	cout << "the different ball is " << (this->table[this->result]).second << "\n" << endl;
}
