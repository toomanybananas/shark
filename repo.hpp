#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
//Syncs all the repos
int Sync(std::string root = "/");
//Find the repo a package belongs to
std::string FindPkg(std::string pkg, std::string root = "/");
//Get a list of packages in a repo
std::vector<std::string> GetPkgList(std::string repo, std::string root = "/");
