#include "randomlogic.h"

// checks whether a string contains numbers (relevant for /checkimg id error handling )
bool containsNumber(const std::string& s) {
    return s.find_first_of("0123456789") != std::string::npos;
}

std::string vectorStringToMessage(std::vector<std::string> values)
{
    std::string message = std::string("");

    for(uint16_t i = 0; i < values.size(); i++)
    {
        if(i == values.size() - 1)
        {
            message = message + std::string("and ") + values[i];
        }
        else
        {
            message = message + values[i] + std::string(", ");
        }
    }
    return message;
}

std::vector<std::string> intVectorToString(std::vector<int> integers)
{
    std::vector<std::string> strings {};

    for(uint16_t i = 0; i < integers.size(); i++)
    {
        strings.push_back(std::to_string(integers[i]));
    }
    return strings;
}

void printIntVector(std::vector<int> v)
{
    for(uint16_t i = 0; i < v.size(); i++)
    {
        std::cout<<std::to_string(v[i]) +" ";
    }
}

void printStringVector(std::vector<std::string> v)
{
    for(uint16_t i = 0; i < v.size(); i++)
    {
        std::cout<<v[i] +" ";
    }
}

std::string userString(std::string user_id)
{
    return user_id + std::string(" owns cards with id: ");
}