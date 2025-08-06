#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

//loads in .env file 
void load_env_file(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t equal_pos = line.find('=');
        if (equal_pos == std::string::npos) continue;

        std::string key = line.substr(0, equal_pos);
        std::string value = line.substr(equal_pos + 1);

        key.erase(0, key.find_first_not_of(" \t\n\r"));
        key.erase(key.find_last_not_of(" \t\n\r") + 1);
        value.erase(0, value.find_first_not_of(" \t\n\r"));
        value.erase(value.find_last_not_of(" \t\n\r") + 1);

        #ifdef _WIN32
            _putenv_s(key.c_str(), value.c_str());
        #else
            setenv(key.c_str(), value.c_str(), 1); 
        #endif
    }
}