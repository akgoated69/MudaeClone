#ifndef envlogic
#define envlogic

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

int load_env_file(const std::string& filename);

#endif