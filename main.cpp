#include "main.h"
#include "series.h" 

using namespace std;
namespace fs = boost::filesystem;

const fs::path DEFAULT_PATH(string(getenv("HOME")) + "/.seriesTest");

int main(int argc, char *argv[]) {
	//either we want to show the help or view the latest series...?
	if(argc == 1) {
	}
	else if(argc == 2) {	
		vector<string> args = getArguments(argc, argv);
		if(isFlag("--help", args))
			printHelp();
		else {
			string name(args[1]);
		}
	}
	else {	
		vector<string> args = getArguments(argc, argv);
		if(isFlag("--new", args)) {
			string name = getFlag("--new", args);
			string path = getFlag("--path", args);
			int season = 1, episode = 1;
			//TODO
			//probably shouldnt assume that the user gives a valid int, but let's not care
			if(isFlag("--season", args))
				stringstream(getFlag("--season", args)) >> season;
			if(isFlag("--episode", args))
				stringstream(getFlag("--episode", args)) >> episode;
			Series s(name, path, season, episode);
			s.addSeriesToFile(DEFAULT_PATH);
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
	cout << "You requested help, should probably show you how you're supposed to use the program here" << endl;
}
