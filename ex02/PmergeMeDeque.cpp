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
    if (arr.size() <= 1)
        return;

    std::deque<SortPairDeque> pairs;
    std::deque<int> mainChain;
    std::deque<int> insertChain;
    
    // ペアを作成
    size_t i;
    for (i = 0; i + 1 < arr.size(); i += 2)
    {
        int small = arr[i];
        int large = arr[i + 1];
        if (small > large)
            std::swap(small, large);
        pairs.push_back(SortPairDeque(small, large, i + 1));
        mainChain.push_back(small);
    }

    // 余った要素を処理
    if (i < arr.size())
        insertChain.push_back(arr[i]);

    // メインチェーンを再帰的にソート
    recursiveFordJohnsonSort(mainChain);

    // ラージ要素をインサートチェーンに追加
    for (size_t j = 0; j < pairs.size(); ++j)
        insertChain.push_back(pairs[j].large);

    // ヤコブスタール数列に基づいてインサート
    std::deque<size_t> insertIndices = generateInsertIndex(insertChain.size());
    insertInJacobsthalOrder(mainChain, insertChain, insertIndices, pairs);

    arr = mainChain;
}

std::deque<size_t> PmergeMeDeque::generateInsertIndex(size_t count)
{
    if (count == 0)
        return std::deque<size_t>();

    std::deque<size_t> result;
    size_t prev2 = 0;
    size_t prev1 = 1;
    
    while (prev1 <= count)
    {
        result.push_back(prev1);
        size_t next = prev2 + 2 * prev1;
        prev2 = prev1;
        prev1 = next;
    }

    for (size_t i = count - 1; i > 0; --i)
    {
        if (std::find(result.begin(), result.end(), i) == result.end())
            result.push_back(i);
    }

    return result;
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
    for (size_t i = 0; i < indices.size() && i < insertChain.size(); ++i)
    {
        size_t insertIndex = indices[i];
        if (insertIndex >= insertChain.size())
            continue;

        int valueToInsert = insertChain[insertIndex];
        size_t pos;
        
        if (insertIndex == insertChain.size() - 1 && insertIndex >= pairs.size())
            pos = binarySearch(mainChain, valueToInsert, mainChain.size());
        else
            pos = binarySearch(mainChain, valueToInsert, pairs[insertIndex].largeIndex);

        mainChain.insert(mainChain.begin() + pos, valueToInsert);
        
        for (size_t j = insertIndex; j < pairs.size(); ++j)
            pairs[j].largeIndex++;
    }
}

void PmergeMeDeque::printResult()
{
    std::cout << "After:  ";
    for (size_t i = 0; i < _sorted.size(); ++i)
        std::cout << _sorted[i] << " ";
    std::cout << std::endl;
}

double PmergeMeDeque::getProcessingTime() const
{
    return 1000000.0 * (_end - _start) / CLOCKS_PER_SEC;
}
