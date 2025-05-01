#include "PmergeMeDeque.hpp"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

PmergeMeDeque::PmergeMeDeque() {}
PmergeMeDeque::~PmergeMeDeque() {}

void PmergeMeDeque::parseInput(char **argv)
{
    _start = std::clock();
    _input.clear();
    for (int i = 1; argv[i]; ++i)
    {
        std::string s(argv[i]);
        for (size_t j = 0; j < s.length(); ++j)
        {
            if (!isdigit(s[j]))
                throw std::invalid_argument("Error: invalid character in input");
        }
        long long val = std::stoll(argv[i]);
        if(val <= 0 || val > std::numeric_limits<int>::max())
            throw std::invalid_argument("Error: argument out of range");
        _input.push_back(static_cast<int>(val));
    }
}

void PmergeMeDeque::fordJohnsonSort()
{
    if (_input.empty())
        return;
    _sorted = _input;
    recursiveFordJohnsonSort(_sorted);
    _end = std::clock();
}

void PmergeMeDeque::recursiveFordJohnsonSort(std::deque<int>& arr)
{
    if (arr.size() <= 1) return;

    std::deque<SortPairDeque> pairs;
    int pend = -1;
    
    size_t i = 0;
    for (; i + 1 < arr.size(); i += 2)
    {
        int a = arr[i];
        int b = arr[i + 1];
        if (a < b)
            pairs.push_back(SortPairDeque(a, b, 0));
        else
            pairs.push_back(SortPairDeque(b, a, 0));
    }
    pend = (i < arr.size()) ? arr[i] : -1;

    std::deque<int> mainChain;
    std::deque<int> insertChain;
    
    for (size_t i = 0; i < pairs.size(); ++i)
    {
        insertChain.push_back(pairs[i].small);
        mainChain.push_back(pairs[i].large);
        pairs[i].largeIndex = i;
    }

    recursiveFordJohnsonSort(mainChain);

    for (size_t i = 0; i < mainChain.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (pairs[j].large == mainChain[i])
            {
                pairs[j].largeIndex = i;
                break;
            }
        }
    }

    std::deque<size_t> indices = generateInsertIndex(insertChain.size());

    //debug
    // std::cout << "   Generated indices: ";
    // for (size_t i = 0; i < indices.size(); ++i)
    //     std::cout << indices[i] << " ";
    // std::cout << std::endl;
    //dubug

    insertInJacobsthalOrder(mainChain, insertChain, indices, pairs);

    if (pend != -1)
    {
        size_t pos = binarySearch(mainChain, pend, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, pend);
    }

    arr = mainChain;
}

std::deque<size_t> PmergeMeDeque::generateInsertIndex(size_t count)
{
    std::deque<size_t> indices;
    if (count == 0)
        return indices;
    std::deque<size_t> jacob;
    jacob.push_back(1);
    jacob.push_back(3);
    while (jacob.back() < count)
        jacob.push_back(jacob.back() + 2 * jacob[jacob.size() - 2]);
    if (jacob.back() >= count)
        jacob.pop_back();
    std::deque<size_t> groupSizes;
    groupSizes.push_back(jacob[0]);
    for (size_t i = 1; i < jacob.size(); ++i)
        groupSizes.push_back(jacob[i] - jacob[i - 1]);
    std::deque<size_t> src;
    for (size_t i = 0; i < count; ++i)
        src.push_back(count - 1 - i);
    for (size_t i = 0; i < groupSizes.size(); ++i)
    {
        std::deque<size_t> tmp;
        size_t groupSize = groupSizes[i];
        size_t x = (groupSize < src.size()) ? groupSize : src.size();
        for (size_t j = 0; j < x; ++j)
        {
            tmp.push_back(src.back());
            src.pop_back();
        }
        while (!tmp.empty())
        {
            indices.push_back(tmp.back());
            tmp.pop_back();
        }
    }
    while (!src.empty())
    {
        std::deque<size_t> tmp;
        while (!src.empty())
        {
            tmp.push_back(src.back());
            src.pop_back();
        }
        while (!tmp.empty())
        {
            indices.push_back(tmp.back());
            tmp.pop_back();
        }
    }
    return indices;
}

size_t PmergeMeDeque::binarySearch(const std::deque<int>& deq, int value, size_t maxIndex)
{
    size_t left = 0;
    size_t right = maxIndex;

    while (left < right)
    {
        size_t mid = left + (right - left) / 2;
        if (deq[mid] > value)
            right = mid;
        else
            left = mid + 1;
    }
    return left;
}

void PmergeMeDeque::insertInJacobsthalOrder(std::deque<int>& mainChain,
                                           const std::deque<int>& insertChain,
                                           const std::deque<size_t>& indices,
                                           std::deque<SortPairDeque>& pairs)
{
    for (size_t i = 0; i < indices.size(); ++i)
    {
        size_t pairIndex = indices[i];
        if (pairIndex >= insertChain.size()) continue; //いらないかも
        
        int value = insertChain[pairIndex];
        size_t maxSearchIndex = pairs[pairIndex].largeIndex;
        
        size_t pos = binarySearch(mainChain, value, maxSearchIndex);
        mainChain.insert(mainChain.begin() + pos, value);
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (pairs[j].largeIndex >= pos)
                pairs[j].largeIndex++;
        }
    }
}

void PmergeMeDeque::printResult()
{
    std::cout << "After:  ";
    for (size_t i = 0; i < _sorted.size(); ++i)
        std::cout << _sorted[i] << " ";
    std::cout << std::endl;
}

const std::deque<int>& PmergeMeDeque::getSorted() const 
{ 
    return _sorted; 
}



double PmergeMeDeque::getProcessingTime() const
{
    return 1000000.0 * (_end - _start) / CLOCKS_PER_SEC;
}
