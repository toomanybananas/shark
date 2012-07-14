#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include "gen.hpp"
#include "info.hpp"
//Shark package manager

//Shamelessy stolen from some stack overflow thread
char* getField(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);

	if(itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool getFlag(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

std::string root;
int Install(std::string package);
int main(int argc, char* argv[])
{
	//Set the default values
	root="/";
	if(getFlag(argv, argv+argc, "--root"))
	{
		root = (std::string)getField(argv, argv+argc, "--root");
		std::cout << "Using " << root << " as /\n";
	}
	if(root == "/")
	{
		std::cout << "Error: no fake root set (safe mode)\n";
		return 1;
	}
	if(getFlag(argv, argv+argc, "-I"))
	{
		return Install(getField(argv, argv+argc, "-I"));
	}
	if(getFlag(argv, argv+argc, "--gen-dirs"))
	{
		GenDirs(root, getField(argv, argv+argc, "--gen-dirs"), "");
	}
	if(getFlag(argv, argv+argc, "--gen-links"))
	{
		GenLinks(root, getField(argv, argv+argc, "--gen-links"), getField(argv, argv+argc, "-p"));
	}
	if(getFlag(argv, argv+argc, "--gen-files"))
	{
		GenFiles(root, getField(argv, argv+argc, "--gen-files"), "");
	}
	return 0;
}

int Install(std::string package)
{
	//untar a file into /usr/pkg
	std::cout << "Extracting tarball\n";
	std::stringstream tarball;
	tarball << "tar xf " << package  << " -C "<< root << "usr/pkg";
	if(system(tarball.str().c_str()))
	{
		return 1;
	}
	std::string packagename = package;
	int slash = packagename.find_last_of("/");
	if(slash)
	{
		packagename = package.substr(slash+1);
	}
	int dot = packagename.find(".tar");
	if(dot)
	{
		packagename = packagename.substr(0, dot);
	}
	std::cout << "Verifying the package (not really)\n";
	std::cout << "Loading the package info\n";
	std::stringstream infofile;
	infofile << root << "usr/pkg/" << packagename << "/info";
	int res;
	std::map<std::string, std::string> info = LoadInfo(infofile.str(), res);
	if(res != 0)
		return res;
	std::string pkgname = info["PKG_NAME"];
	std::string ver = info["PKG_VER"];
	std::cout << "Loading the file list\n";
	std::vector<std::string> filelist;
	std::ifstream files;
	std::stringstream filesfile;
	filesfile << root << "usr/pkg/" << packagename << "/files";
	files.open(filesfile.str().c_str());
	if(!files.is_open())
	{
		std::cout << "Error: no file list found. Malformed packge\n";
		return 1;
	}
	std::string line;
	while(files.good())
	{
		std::getline(files, line);
		if(line == "EOF")
			break;
		filelist.push_back(line);
	}
	files.close();

	std::cout << "Looking for conflicts\n";
	for(int i = 0; i < filelist.size(); i++)
	{
		std::stringstream filename;
		filename << root << "usr/pkgdb/" << filelist[i];
		std::ifstream f;
		f.open(filename.str().c_str());
		if(f.is_open())
		{
			//we are probably conflicting, but make sure
			std::string pkg;
			getline(f, pkg);
			if(pkg != pkgname)
			{
				std::cout << "Error: package " << pkgname << " conflicts with " << pkg << " on file " << filename.str() << ".\nInstallation aborted\n";
				return 1;
			}
		}
	}
	std::cout << "Installing package " << packagename << std::endl;
	//TODO:load the info file, add it to the package DB
	//make the directories from dirs
	std::ifstream dirs;
	std::stringstream dirsfile;
	dirsfile << root << "usr/pkg/" << packagename << "/dirs";
	dirs.open(dirsfile.str().c_str());
	if(dirs.is_open()) //assume that nothing needs to be made if there is no file
	{
		std::string dir;
		while(dirs.good())
		{
			std::getline(dirs, dir);
			if(dir == "EOF")
				break;
			std::stringstream d;
			d << "mkdir --parents " << root << dir << " " << root << "usr/pkgdb/" << dir;
			system(d.str().c_str());
		}
	}
	//copy any files that need copying
	for(int i=0; i < filelist.size(); i++)
	{
		std::stringstream f;
		f << "cp -fd " << root << "usr/pkg/" << packagename << "/" << filelist[i] << " " << root << filelist[i];
		//std::cout << f.str() << std::endl;
		system(f.str().c_str());
		//update the package db
		std::stringstream dbcmd;
		dbcmd << "echo \"" << pkgname << "\" > " << root << "usr/pkgdb/" << filelist[i];
		//std::cout << dbcmd.str() << "\n";
		system(dbcmd.str().c_str());
	}
	//install the info file
	std::stringstream infocmd;
	infocmd << "cp -fd " << root << "usr/pkg/" << packagename << "/info " << root << "usr/pkg/" << pkgname;
	//std::cout << infocmd.str() << std::endl;
	system(infocmd.str().c_str());
	files.close();
	return 0;
}
