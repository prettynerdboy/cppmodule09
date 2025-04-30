#include "BitcoinExchange.hpp"

//utils
std::string BitcoinExchange::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \f\v\t\n\r");
    size_t last = str.find_last_not_of(" \f\v\t\n\r");
    if (first == std::string::npos || last == std::string::npos)
        return "";
    return str.substr(first, (last - first + 1));
};

bool    BitcoinExchange::is_num( std::string &str )
{
	for(size_t i = 0; i < str.length(); i++ )
    {
		if (!std::isdigit(str.at(i)))
			return false;
	}
	return true;
}

bool    BitcoinExchange::checkYear( std::string& year )
{
	if (year.size() != 4 || !is_num(year))
		return false;
	size_t	num;
	std::istringstream(year) >> num;
	return 2000 <= num && num <= 2099;
}

bool    BitcoinExchange::checkMonthDate(std::string& year, std::string& month, std::string& date)
{
    if (month.empty() || month.size() > 2 || !is_num(month))
        return false;
    if (date.empty() || date.size() > 2 || !is_num(date))
        return false;
    size_t m, d;
    std::istringstream(month) >> m;
    std::istringstream(date) >> d;
    if (m < 1 || m > 12)
        return false;

    static const int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int max_days = days_in_month[m - 1];
    if (m == 2)
    {
        int y;
        std::istringstream(year) >> y;
        if (y % 4 == 0)
            max_days = 29;
    }
    return 1 <= d && d <= static_cast<size_t>(max_days);
}

bool BitcoinExchange::dateCheck(std::string &string)
{
    std::istringstream str(string);
    std::string	year, month, date;
	if (!std::getline(str, year, '-'))
		return false;
    if( !std::getline(str, month, '-'))
        return false;
    if(!std::getline(str, date))
        return false;
	if (!checkYear(year))
		return false;
	if (!checkMonthDate(year, month, date))
		return false;
	return true;
}

bool BitcoinExchange::isFloat(const std::string& str)
{
    if (str.empty())
        return false;
    bool dotFound = false;
    bool digitFound = false;
    size_t i = 0;
    if (str[0] == '+')
        i++;
    for (; i < str.length(); ++i)
    {
        if (std::isdigit(str[i]))
        {
            digitFound = true;
            continue;
        }
        if (str[i] == '.' && !dotFound)
        {
            dotFound = true;
            continue;
        }
        return false;
    }
    if (!digitFound)
        return false;
    if (str.back() == '.')
        return false;
    return true;
}

double BitcoinExchange::getRate(std::string const &date)
{
    std::map<std::string,double>::const_iterator it = _data.upper_bound(date);
    if(it != _data.begin())
    {
        it--;
        return it->second;
    }
    else
        return -1;
};

bool	BitcoinExchange::checkValue( std::string& str )
{
    if (!isFloat(str))
        return false;
    float num;
    std::istringstream(str) >> num;
    return (num >= 0 && num <= 1000);
};

bool    BitcoinExchange::checkHeader(std::string &header)
{
    std::istringstream stream(header);
    std::string date,value;
    char split;
    if(!(stream >> date >> split >> value))
        return false;
    else if (date !="date" || split != '|' || value != "value" )
        return false;
    stream >> std::ws; 
    if (!stream.eof())
        return false;
    return true;
};

//main constructor 

BitcoinExchange::BitcoinExchange(const std::string &fileName)
{
    std::fstream dataFile(fileName.c_str());
    if(!dataFile)
        throw (BitcoinExchange::DataFileOpenErrorException());
    std::string line;
    std::getline(dataFile,line);
    size_t linePos = 1;
    while(std::getline(dataFile,line))
    {
        ++linePos;
        std::string::size_type split_pos = line.find(',');
        if(split_pos == std::string::npos)
        {
            std::cerr << "Error: Invalid argument on line " << linePos << "in data.csv" << std::endl;
            continue;
        }
        std::string date = trim(line.substr(0, split_pos));
        std::string rateStr = trim(line.substr(split_pos + 1));
        if (!dateCheck(date))
        {
            std::cerr << "Error: Invalid date on line " << linePos << std::endl;
            continue;
        }
        if (!isFloat(rateStr))
        {
            std::cerr << "Error: Invalid rate format on line " << linePos << std::endl;
            continue;
        }
        double num;
		std::istringstream(rateStr) >> num;
		_data[date] = num;
    }
    dataFile.close();
	if (_data.size() == 0)
		throw InvalidDataException();
}

void	BitcoinExchange::calculation(const std::string& inputFileName )
{
    std::fstream inputFile(inputFileName.c_str());
    if(!inputFile)
        throw BitcoinExchange::InputFileOpenErrorException();
    std::string line;
    std::getline(inputFile,line);
    if(!checkHeader(line))
        throw BitcoinExchange::invalidHeaderErrorExeption();

    while(std::getline(inputFile,line))
    {
        std::istringstream stream(line);
        std::string date,value;
        char    split;
        if(!(stream >> date >> split >> value))
        {
            std::cerr << "Error: Format error: Syntax error" << std::endl;
			continue ;
        }
        else if (split != '|')
        {
			std::cerr << "Error: Format error: delimiter has to be '|'" << std::endl;
			continue ;
		}
        stream >> std::ws; 
        if (!stream.eof())
        {
            std::cerr << "Error: Format error: Unexpected extra input" << std::endl;
            continue;
        }
        if (!dateCheck(date))
        {
			std::cerr << "Error: Format error: Invalid date" << std::endl;
			continue ;
		}
        if (!checkValue(value))
        {
			std::cerr << "Error: Format error: Invalid value" << std::endl;
			continue ;
		}
        double rate = getRate(date);
        if(rate < 0)
            std::cerr << "Error: no exchange rate beofore this date" << std::endl;
        else
        {
            double val;
            std::istringstream(value) >> val;
            std::cout << date << " => " << value << " = " <<  val * rate << std::endl;
        }
    }
    inputFile.close();
    return ;
};

//debag
void BitcoinExchange::printdata(void)const
{
    for (std::map<std::string, double>::const_iterator it = _data.begin(); it != _data.end(); ++it)
    {
        std::cout << it->first << " => " << it->second << std::endl;
    }
};


//canonical
BitcoinExchange::BitcoinExchange( void ) {};
BitcoinExchange::~BitcoinExchange( void ) {};
BitcoinExchange::BitcoinExchange( const BitcoinExchange &other ) { this->_data = other._data; };
BitcoinExchange &BitcoinExchange::operator=( const BitcoinExchange& other )
{
	if (this != &other)
		this->_data = other._data;
	return *this;
};