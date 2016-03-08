#include "Header.h"

int main(int argc, char* argv[]) {
	cout << "the different ball is number" << argv[1] << " and " << argv[2] << "\n" << endl;
	int number;
	char input[10];
	strcpy(input, argv[2]);
	Status status = strcmp(input, "light") == 0 ? light : heavy;
	sscanf(argv[1], "%d", &number);
	WeightGame game(number, status);
	return 0;
}
