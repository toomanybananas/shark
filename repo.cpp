#include "repo.hpp"
#include <cstdlib>
int Sync(std::string root)
{
	//Erase all the repos in the /etc/shark/repo dir
	std::stringstream rm;
	rm << "rm -rf " << root << "etc/shark/repo/*i; rm " << root << "etc/shark/repolist";
	system(rm.str().c_str());
	//Download the repo list
	std::stringstream rld;
	rld << "curl -o " << root <<"etc/shark/repolist" << " http://neos300.com/astro/pkg/repolist > /dev/null";
	system(rld.str().c_str());

	//Load a vector of the repos
	std::vector<std::string> repos;
	std::ifstream repolist;
	repolist.open((root + "etc/shark/repolist").c_str());
	if(!repolist.is_open())
	{
		std::cout << "Error: repolist could not be retrieved\n";
		return 1;
	}
	while(repolist.good())
	{
		std::string line;
		std::getline(repolist, line);
		repos.push_back(line);

		//Download the list of packages in the repo
		std::stringstream rcmd;
		rcmd << "curl -o " << root << "etc/shark/repo/" << line << " http://neos300.com/astro/pkg/repo/" << line << " > /dev/null";
		system(rcmd.str().c_str());
	}
	return 0;
}


std::string FindPkg(std::string pkg, std::string root)
{
	std::vector<std::string> repos;
	std::ifstream repolist;
	repolist.open((root + "etc/shark/repolist").c_str());
	while(repolist.good())
	{
		std::string line;
		std::getline(repolist, line);
		std::ifstream repo;
		repo.open((root + "etc/shark/repo/" + line).c_str());
		while(repo.good())
		{
			std::string ln;
			std::getline(repo, ln);
			if(ln == pkg)
			{
				return line;
			}
		}
	}
}


std::vector<std::string> GetPkgList(std::string repo, std::string root)
{
	std::vector<std::string> pkgs;
	std::ifstream list;
	list.open((root + "etc/shark/repo/" + repo).c_str());
	while(list.good())
	{
		std::string line;
		std::getline(list, line);
		pkgs.push_back(line);
	}
}
