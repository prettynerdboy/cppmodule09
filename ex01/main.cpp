#include "RPN.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./RPN \"expression\"" << std::endl;
		return 1;
	}
	RPN rpn;
	try {
		rpn.calculation(argv[1]);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
}

/*
./RPN "8 9 * a +"
./RPN "8 0 /"
./RPN "+ 5 6"
./RPN "50000 50000 *"
./RPN "5 6"
./RPN "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9" 
*/