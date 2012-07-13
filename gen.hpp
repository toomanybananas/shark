#pragma once
#include <string>
#include <sstream>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <algorithm>

void GenDirs(std::string root, std::string dir, std::string pkg, std::string prefix = "");

void GenLinks(std::string root, std::string dir, std::string pkg, std::string prefix = "");



void GenFiles(std::string root, std::string dir, std::string pkg, std::string prefix = "");
