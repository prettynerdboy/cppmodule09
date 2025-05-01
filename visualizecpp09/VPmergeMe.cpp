#include "VPmergeMe.hpp"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

PmergeMe::PmergeMe():_recursiveCount(-1) {}
PmergeMe::~PmergeMe() {}

//コマンドライン引数をチェックして全て数字なら数値に変換→有効な範囲ならコンテナに格納
void PmergeMe::parseInput(char **argv)
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
//責任分離の観点からコンストラクタにはしない（単一責任の原則的に）
//この関数では入力の読み取りだけにしたい。

//前半にヤコブスタール配列を生成して、後半にインサートするインデックスの順番を決める配列を生成する。
//配列の生成方法、ヤコブスタール配列をJとしてJ(n) - J(n-1)がグループの大きさ
//グループの大きさで配列を分割し、先頭グループNとして、Nの最後尾→N[0]の順で挿入する、次にN＋１のグループに進む
std::vector<size_t> PmergeMe::generateInsertIndex(size_t count)
{
    std::vector<size_t> indices;
    if (count == 0)
        return indices;
    std::vector<size_t> jacob;
    jacob.push_back(1);
    jacob.push_back(3);
    while (jacob.back() < count)
        jacob.push_back(jacob.back() + 2 * jacob[jacob.size() - 2]);
    if (jacob.back() >= count)
        jacob.pop_back();
    
    //debag
    std::cout << "  Generated Jacobsthal sequence: ";
    for (size_t i = 0; i < jacob.size(); ++i)
    {
        std::cout << jacob[i];
        if (i + 1 < jacob.size())
            std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "  Create the following sequence from the above." << std::endl;
    //debag

    std::vector<size_t> groupSizes;
    groupSizes.push_back(jacob[0]);
    for (size_t i = 1; i < jacob.size(); ++i)
        groupSizes.push_back(jacob[i] - jacob[i - 1]);
    std::vector<size_t> src;
    for (size_t i = 0; i < count; ++i)
        src.push_back(count - 1 - i);
    for (size_t i = 0; i < groupSizes.size(); ++i)
    {
        std::vector<size_t> tmp;
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
        std::vector<size_t> tmp;
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

//探索上限を、挿入したいインサートチェーンのvalueと紐ついてるmainchainのindexに制限した二分探索
size_t PmergeMe::binarySearch(const std::vector<int> &vec, int value, size_t maxIndex)
{
    size_t left = 0;
    size_t right = std::min(maxIndex + 1, vec.size());
    
    while (left < right)
    {
        size_t mid = (left + right) / 2;
        if (vec[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

//ヤコブスタール数列から作られたオーダーに基づいて挿入するインサートチェーンのindexを決定、上記の関数で順に挿入する。
void PmergeMe::insertInJacobsthalOrder(std::vector<int> &mainChain, 
                                      const std::vector<int> &insertChain, 
                                      const std::vector<size_t> &indices,
                                      std::vector<SortPair> &pairs)
{
    std::cout << " STEP 4: Insest value from insertchain to main chain use befor step order." << std::endl << std::endl;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        size_t pairIndex = indices[i];
        int value = insertChain[pairIndex];
        size_t maxSearchIndex = pairs[pairIndex].largeIndex;
        
        //debug
         std::cout << "Inserting value [" << pairIndex << "] = " << value 
         << " (maxSearchIndex = " << maxSearchIndex << ")" << std::endl;
        std::cout << "MainChain before insertion: ";
        for (size_t k = 0; k < mainChain.size(); ++k)
        std::cout << "[" << k << "]" << mainChain[k] << " ";
        std::cout << std::endl;
        //debug

        size_t pos = binarySearch(mainChain, value, maxSearchIndex);

        std::cout << "insertion main chain[" << pos << "]" <<std::endl;
        mainChain.insert(mainChain.begin() + pos, value);
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (pairs[j].largeIndex >= pos)
                pairs[j].largeIndex++;
        }
    }
}

void PmergeMe::fordJohnsonSort()
{
    std::cout << "\033[31m#FordJohnsonSort Start !\033[0m" << std::endl << std::endl;
    _sorted = _input;
    recursiveFordJohnsonSort(_sorted);
    std::cout << std::endl << "\033[31m#FordJohnsonSort End(Damn it !)\033[0m" << std::endl << std::endl;
    _end = std::clock();
}

void PmergeMe::recursiveFordJohnsonSort(std::vector<int>& arr)
{
    //debug recursive
    _recursiveCount++;
    if(_recursiveCount != 0)
    {   
        std::cout<< "\033[32m # " << _recursiveCount << "th recursive call \033[0m" << std::endl;
    }
    //debug recursive

    if (arr.size() <= 1)
    {
        std::cout<< "\033[33m   →<welcome to Winding Phase>←  \033[0m" << std::endl;
        std::cout<< "\033[32m # " << _recursiveCount << "th recursive function over \033[0m" << std::endl;
        _recursiveCount--;
        return;
    }
    std::vector<SortPair> pairs;
    int pend = -1;
    
    size_t i = 0;//余りチェックで使いたいからスコープ外で宣言
    for (; i + 1 < arr.size(); i += 2)
    {
        int a = arr[i];
        int b = arr[i + 1];
        if (a < b)
            pairs.push_back(SortPair(a, b, 0));
        else
            pairs.push_back(SortPair(b, a, 0));
    }
    pend = (i < arr.size()) ? arr[i] : -1;

    //debug makepair
    std::cout << " STEP 1: Make pair from the front." << std::endl << std::endl;
    std::cout << "  Pairs (small, large): ";
    for (size_t j = 0; j < pairs.size(); ++j)
    {
        std::cout << "(" << pairs[j].small << "," << pairs[j].large << ") ";
    }
    if (pend != -1)
    {
        std::cout << "| pend = " << pend;
    }
    std::cout << std::endl << std::endl;
    //debug makepair

    std::vector<int> mainChain;
    std::vector<int> insertChain;
    
    for (size_t i = 0; i < pairs.size(); ++i)
    {
        insertChain.push_back(pairs[i].small);
        mainChain.push_back(pairs[i].large);
        pairs[i].largeIndex = i;
    }

    //debug　dividgroup
    std::cout << " STEP 2: Divide into large and small groups." << std::endl << std::endl;
    std::cout << "  Main Chain (large):  ";
    for (size_t j = 0; j < mainChain.size(); ++j)
    {
        std::cout << mainChain[j] << " ";
    }
    std::cout << std::endl;
    std::cout << "  Insert Chain (small): ";
    for (size_t j = 0; j < insertChain.size(); ++j)
    {
        std::cout << insertChain[j] << " ";
    }
    std::cout << std::endl;
    if (pend != -1)
    {
        std::cout << "  Pend (unpaired element): " << pend << std::endl;
    }
    std::cout << std::endl;
    //debug divedgroup

    recursiveFordJohnsonSort(mainChain);//折り返し地点

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

    std::cout << " STEP 3: Make Insertion Order." << std::endl << std::endl;

    std::vector<size_t> indices = generateInsertIndex(insertChain.size());

    //debug
    std::cout << "   Generated indices: ";
    for (size_t i = 0; i < indices.size(); ++i)
        std::cout << indices[i] << " ";
    std::cout << std::endl;
    //dubug

    insertInJacobsthalOrder(mainChain, insertChain, indices, pairs);

    if (pend != -1)
    {
        size_t pos = binarySearch(mainChain, pend, mainChain.size());
        mainChain.insert(mainChain.begin() + pos, pend);
    }

    if(_recursiveCount != 0)
    {   
        std::cout<< "\033[32m # " << _recursiveCount << "th recursive function over \033[0m" << std::endl;
        _recursiveCount--;
    }
    if (arr.size() <= 1) return;
    arr = mainChain;
}

void PmergeMe::printResult()
{
    std::cout << "After: ";
    for (size_t i = 0; i < _sorted.size(); ++i)
        std::cout << _sorted[i] << " ";
    std::cout << std::endl;
    double time_us = 1000000.0 * (_end - _start) / CLOCKS_PER_SEC;
    std::cout << "Time to process: " << time_us << " us" << std::endl;
}

std::vector<int> const &PmergeMe::getInput(void)const
{
    return (_input);
};

std::vector<int> const &PmergeMe::getSort(void)const
{
    return (_sorted);
};