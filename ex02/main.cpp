#include "PmergeMe.hpp"
#include "PmergeMeDeque.hpp"
#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./PmergeMe [positive integers...]" << std::endl;
        return 1;
    }
    try {
        PmergeMe vectorSorter;
        PmergeMeDeque dequeSorter;

        std::cout << "Before: ";
        for (int i = 1; argv[i]; ++i)
            std::cout << argv[i] << " ";
        std::cout << std::endl;

        // Vector sort
        vectorSorter.parseInput(argv);
        vectorSorter.fordJohnsonSort();
        vectorSorter.printResult();

        // Deque sort
        dequeSorter.parseInput(argv);
        dequeSorter.fordJohnsonSort();
        
        std::cout << "Time to process a range of " << argc - 1 
                  << " elements with std::vector : " 
                  << std::fixed << std::setprecision(5) 
                  << vectorSorter.getProcessingTime() << " us" << std::endl;
        std::cout << "Time to process a range of " << argc - 1 
                  << " elements with std::deque : " 
                  << std::fixed << std::setprecision(5) 
                  << dequeSorter.getProcessingTime() << " us" << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
