#ifndef RANDOMLOGIC
#define RANDOMLOGIC

#include <vector>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <iostream>

bool containsNumber(const std::string& s);


std::string vectorStringToMessage(std::vector<std::string> values);

std::vector<std::string> intVectorToString(std::vector<int> integers);

void printIntVector(std::vector<int> v);

void printStringVector(std::vector<std::string> v);

std::string userString(std::string user_id);

#endif
