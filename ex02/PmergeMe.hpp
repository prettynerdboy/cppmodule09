#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <utility>
#include <iostream>

struct SortPair {
    int small;
    int large;
    size_t largeIndex;
    SortPair(int s, int l, size_t i) : small(s), large(l), largeIndex(i) {}
};

class PmergeMe {
private:
    std::vector<int> _input;
    std::vector<int> _sorted;
    std::clock_t _start;
    std::clock_t _end;

    std::vector<size_t> generateJacobsthalIndices(size_t count);

    size_t binarySearch(const std::vector<int> &vec, int value, size_t maxIndex);

    void insertInJacobsthalOrder(std::vector<int> &mainChain, 
                                const std::vector<int> &insertChain, 
                                const std::vector<size_t> &indices,
                                std::vector<SortPair> &pairs);

    void recursiveFordJohnsonSort(std::vector<int>& arr);

public:
    PmergeMe();
    ~PmergeMe();
    void parseInput(char **argv);
    void fordJohnsonSort();
    void printResult();
};

#endif