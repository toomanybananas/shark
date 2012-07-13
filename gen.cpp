#include "gen.hpp"

void GenDirs(std::string root, std::string dir, std::string pkg, std::string prefix)
{
	std::stringstream dirname;
	dirname << dir << prefix;
	DIR* dirs = opendir(dirname.str().c_str());
	struct dirent* file;
	if(dirs == NULL)
	{
		std::cout << "Error: dir " << dirname.str() << " does not exist\n";
		return;
	}
	while((file = readdir(dirs)))
	{
		if(file->d_type == DT_DIR)
		{
			//std::cout << "DIR:" <<  file->d_name << std::endl;
			std::string name = file->d_name;
			if(name != "." && name != "..")
			{
				std::stringstream cmd;
				cmd << "echo \"" << prefix << name << "\" >> dirs";
				std::cout << cmd.str() << std::endl;
				system(cmd.str().c_str());
				std::stringstream newpre;
				newpre << prefix << name << "/";
				GenDirs(root, dir, pkg, newpre.str().c_str());	
			}
		}
	}
}


void GenLinks(std::string root, std::string dir, std::string pkg, std::string prefix)
{
	std::cout << "GEN LINKS\n";
	std::stringstream dirname;
	dirname << dir << prefix;
	DIR* dirs = opendir(dirname.str().c_str());
	struct dirent* file;
	if(dirs == NULL)
	{
		std::cout << "Error: dir " << dirname.str() << " does not exist\n";
		return;
	}
	while((file = readdir(dirs)))
	{
		std::string name = file->d_name;
		if(file->d_type != DT_DIR)
		{
			//std::cout << "DIR:" <<  file->d_name << std::endl;
			//std::string name = file->d_name;
			if(name != "." && name != "..")
			{
				std::stringstream cmd;
				cmd << "echo \"" << "usr/pkg/" << pkg << "/" << prefix << name << ":" << prefix << name << "\" >> links";
				std::cout << cmd.str() << std::endl;
				system(cmd.str().c_str());
				//std::stringstream newpre;
				//newpre << prefix << name << "/";
				//GenLinks(root, dir, pkg, newpre.str().c_str());	
			}
		}
		else
		{
			if(name != "." && name != "..")
			{
				std::stringstream newpre;
				newpre << prefix << file->d_name << "/";
				GenLinks(root, dir, pkg, newpre.str());
			}
		}
	}
}

void GenFiles(std::string root, std::string dir, std::string pkg, std::string prefix)
{
	std::cout << "GEN FILES\n";
	std::stringstream dirname;
	dirname << dir << prefix;
	DIR* dirs = opendir(dirname.str().c_str());
	struct dirent* file;
	if(dirs == NULL)
	{
		std::cout << "Error: dir " << dirname.str() << " does not exist\n";
		return;
	}
	while((file = readdir(dirs)))
	{
		std::string name = file->d_name;
		if(file->d_type != DT_DIR)
		{
			//std::cout << "DIR:" <<  file->d_name << std::endl;
			//std::string name = file->d_name;
			if(name != "." && name != "..")
			{
				std::stringstream cmd;
				cmd << "echo \"" << prefix << name << "\" >> files";
				std::cout << cmd.str() << std::endl;
				system(cmd.str().c_str());
				//std::stringstream newpre;
				//newpre << prefix << name << "/";
				//GenLinks(root, dir, pkg, newpre.str().c_str());	
			}
		}
		else
		{
			if(name != "." && name != "..")
			{
				std::stringstream newpre;
				newpre << prefix << file->d_name << "/";
				GenFiles(root, dir, pkg, newpre.str());
			}
		}
	}
}
