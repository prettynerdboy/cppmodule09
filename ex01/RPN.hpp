#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <sstream>
#include <limits>
#include <cctype>
#include <list>

class RPN {
private:
	RPN( RPN const &other );
	RPN	&operator=( RPN const &other );
    //util
    bool isOperator(char c);
    int safeCalculate(int a, int b, char op);
    //member
	std::list<int>	_storage;
public:
	RPN( void );
	~RPN( void );
    //main function
	void	calculation( std::string input);

    //exception class
    class InvalidTokenException: public std::exception
    {
        public:virtual const char *what()const throw(){ return "Error: invalid token";}
    };
    class DivisionByZeroException: public std::exception
    {
        public:virtual const char *what()const throw(){ return "Error: division by zero";}
    };
    class OverflowException: public std::exception
    {
        public:virtual const char *what()const throw(){ return "Error: overflow detected";}
    };
	class	SyntaxErrorException : public std::exception
    {
		public:virtual const char *what() const throw (){ return "Error: syntax error"; }
	};
};

#endif