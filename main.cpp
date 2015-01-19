#include "main.h"
#include "series.h" 

using namespace std;
namespace fs = boost::filesystem;

int main(int argc, char *argv[]) {
	Series se;
	//either we want to show the help or view the latest series...?
	if(argc == 1) {
	}
	else {	
		vector<string> args = getArguments(argc, argv);
		if(isFlag("--help", args))
			printHelp();
		else if(isFlag("--new", args)) {
			string name = getFlag("--new", args);
			string path = getFlag("--path", args);
			int season = 1, episode = 1;
			//TODO
			//probably shouldnt assume that the user gives a valid int, but let's not care
			if(isFlag("--season", args))
				stringstream(getFlag("--season", args)) >> season;
			if(isFlag("--episode", args))
				stringstream(getFlag("--episode", args)) >> episode;
			se = Series(name, path, season, episode);
		}
	}
	return 0;
}

vector<string> getArguments(int argc, char *argv[]) {
	//the first argument is the program itself
	vector<string> v(argc-1);
	for(int i = 0; i < argc-1; i++)
		v[i] = argv[i+1];
	return v;
}

bool isFlag(string flag, const vector<string>& v) {
	return find(v.begin(), v.end(), flag) != v.end() ? true : false;
}

string getFlag(string flag, const vector<string>& v) {
	//find the correct object, then increment the pointer by one and return the object pointed to
	return *(++(find(v.begin(), v.end(), flag)));
}

void printHelp() {
	//TODO
	cout << "You requested help, should probably show you how you're supposed to use the program here" << std::endl;
}
