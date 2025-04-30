#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

class BitcoinExchange
{
    private:
        //canonical 
        BitcoinExchange();
	    BitcoinExchange( const BitcoinExchange &other);
	    BitcoinExchange &operator=( const BitcoinExchange &other);
        //util
        bool    is_num( std::string &str );
        bool	dateCheck( std::string &date );
        bool	checkYear( std::string& year );
        bool	checkMonthDate( std::string& year, std::string& month, std::string& date );
        bool	isFloat(const std::string& str );
        std::string trim(const std::string& str);
        bool	checkValue( std::string& str );
        double	getRate( std::string const &date );
        bool    checkHeader(std::string &header);
        //member
        std::map<std::string, double> _data;
    public:
        //main constructor that initialaized datafile
        BitcoinExchange(const std::string& fileName );
        //destructor
	    ~BitcoinExchange();
        //main func
        void	calculation(const std::string& inputFileName );

        //exeption class
        class DataFileOpenErrorException : public std::exception
        {
            public: virtual const char* what() const throw (){ return "cannot open data.csv file"; }
        };
        class InvalidDataException : public std::exception
        {
            public: virtual const char* what() const throw (){ return "invalid data.csv"; }
        };
        class InputFileOpenErrorException : public std::exception
        {
            public: virtual const char* what() const throw (){ return "cannot open input file"; }
        } ;
        class invalidHeaderErrorExeption: public std::exception
        {
            public: virtual const char* what() const throw (){ return "this file header is invalid"; }
        } ;
        //debug
        void printdata(void)const;

};

#endif
