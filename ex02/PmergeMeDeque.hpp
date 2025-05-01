#ifndef PMERGEMEDEQUE_HPP
#define PMERGEMEDEQUE_HPP

#include <deque>
#include <ctime>
#include <iostream>
#include <limits>

struct SortPairDeque {
    int small;
    int large;
    size_t largeIndex;
    
    SortPairDeque(int s, int l, size_t idx) : small(s), large(l), largeIndex(idx) {}
};

class PmergeMeDeque {
private:
    std::deque<int> _input;
    std::deque<int> _sorted;
    clock_t _start;
    clock_t _end;

    void recursiveFordJohnsonSort(std::deque<int>& arr);
    std::deque<size_t> generateInsertIndex(size_t count);
    size_t binarySearch(const std::deque<int>& deq, int value, size_t maxIndex);
    void insertInJacobsthalOrder(std::deque<int>& mainChain, 
                                const std::deque<int>& insertChain,
                                const std::deque<size_t>& indices,
                                std::deque<SortPairDeque>& pairs);

public:
    PmergeMeDeque();
    ~PmergeMeDeque();
    
    void parseInput(char **argv);
    void fordJohnsonSort();
    void printResult();
    double getProcessingTime() const;
};

#endif
