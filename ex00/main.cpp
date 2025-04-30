#include "BitcoinExchange.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
    {
		std::cerr << "Invalid arguments" << std::endl;
        std::cerr << "Usage: ./btc input.txt" << std::endl;
		return (1);
	}
	try {
        (void) argv;
		BitcoinExchange bit("data.csv");
		bit.calculation(argv[1]);
	} catch (std::exception &e) {
        std::cerr << "Error: " ;
		std::cerr << e.what() << std::endl;
		return (1);
	}

}