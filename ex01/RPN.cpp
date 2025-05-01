#include "RPN.hpp"

bool  RPN::isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' ;
};

int  RPN::safeCalculate(int a, int b, char op)
{
    switch(op)
    {
        case '+':
            if((b > 0 && a > std::numeric_limits<int>::max() - b) ||
                (b < 0 && a < std::numeric_limits<int>::min() - b))
                throw OverflowException();
            return a + b;
        case '-':
            if((b < 0 && a > std::numeric_limits<int>::max() + b) ||
                (b > 0 && a < std::numeric_limits<int>::min() + b))
                throw OverflowException();
            return a - b;
        case '*':
            if (a != 0 && (b > std::numeric_limits<int>::max() / a ||
                b < std::numeric_limits<int>::min() / a))
                throw OverflowException();
            return a * b;
        case '/':
            if (b == 0)
                throw DivisionByZeroException();
            return a / b;
        default:
            throw InvalidTokenException();
    }
}

void	RPN::calculation( std::string input)
{
    if(input.empty())
        throw SyntaxErrorException();
    std::istringstream stream(input);
    std::string token;

    while(stream >> token)
    {
        if(token.length()==1 && std::isdigit(token[0]))
        {
            int value = token[0] - '0';
            if(value < 0 || value > 9)
                throw InvalidTokenException();
            _storage.push_back(value);
        }
        else if(token.length()==1 && isOperator(token[0]))
        {
            if(_storage.size() < 2)
                throw SyntaxErrorException();
            int right = _storage.back();
            _storage.pop_back();
            int left = _storage.back();
            _storage.pop_back();
            _storage.push_back(safeCalculate(left,right,token[0]));
        }
        else
            throw InvalidTokenException();
    }
    if(_storage.size()!=1)
        throw SyntaxErrorException();
    std::cout << _storage.back() << std::endl;
};










RPN::RPN() {};
RPN::~RPN() {};

RPN::RPN(RPN const &other)
{
    *this = other;
}

RPN &RPN::operator=(RPN const &other)
{
    if (this != &other) {
        this->_storage = other._storage;
    }
    return *this;
}