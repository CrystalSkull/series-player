#include <cstdlib>
#include <thread>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <boost/filesystem.hpp>

//efficient thanks to move operations
std::vector<std::string> getArguments(int, char*[]);
bool isFlag(std::string, const std::vector<std::string>&);
std::string getFlag(std::string, const std::vector<std::string>&);
void printHelp();
bool playFile(boost::filesystem::path);
void getInput();
