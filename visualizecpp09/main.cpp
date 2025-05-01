#include "VPmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./PmergeMe [positive integers...]" << std::endl;
        return 1;
    }

    try {
        PmergeMe sorter;

        std::cout << "Before: ";
        for (int i = 1; argv[i]; ++i)
            std::cout << argv[i] << " ";
        std::cout << std::endl << std::endl;

        sorter.parseInput(argv);
        sorter.fordJohnsonSort();
        if (!sorter.isSorted())
        {
            std::cout <<std::endl << "Error: Vector sort failed!\nUnsorted result: ";
            const std::vector<int>& vecResult = sorter.getSort();
            for (size_t i = 0; i < vecResult.size(); ++i)
                std::cout << vecResult[i] << " ";
            std::cout << std::endl;
            return 1;
        }
        if(sorter.getInput().size() !=sorter.getSort().size())
        {
            std::cerr << "Fatal error ocuured " << std::endl;
            std::cout << "input size is " << sorter.getInput().size() << std::endl;
            std::cout << "sorted size is " << sorter.getSort().size() << std::endl;
            return (1);
        }
        sorter.printResult();

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
