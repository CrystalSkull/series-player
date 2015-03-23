#include <iostream>
#include <cstdlib>
#include <regex>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "series.h"

using namespace std;
namespace fs = boost::filesystem;

//public
void series::setName(string n) {
	name = n;
}	
void series::setPath(fs::path p) {
	path = p;
}
void series::setSeason(int s) {
	season = s;
	//if we've updated the season, we might as well update the path to the new season setSeasonPath();
}
void series::setEpisode(int e) {
	episode = e;
}
string series::getName() {
	return name;
}
boost::filesystem::path series::getPath() {
	return path;
}
int series::getSeason() {
	return season;
}
int series::getEpisode() {
	return episode;
}
void series::incrementSeason() {
	++season;
	episode = 1;
	updateSeriesFile();
}
void series::incrementEpisode() {
	++episode;
	updateSeriesFile();
}
void series::addSeriesToFile() {
	string start;
	if(fs::exists(SERIES_FILE))
		start = "\n";
	fs::ofstream fs(SERIES_FILE, fs::fstream::out | fs::fstream::app);
	//we want a starting new line if the file exists
	fs << start;
	fs << "name=" << name << "\n";
	//string() to get rid of the quotation marks
	fs << "path=" << path.string() << "\n";
	fs << "season=" << season << "\n";
	fs << "episode=" << episode << flush;
}
void series::updateSeriesFile() {
	fs::ifstream ifstream(SERIES_FILE);
	string line;
	vector<string> lines;
	while(ifstream && !ifstream.eof()) {
		getline(ifstream, line);
		//make sure it's the correct series we're editing
		if(line == ("name=" + name)) {
			string s("name=");
			s += name;
			lines.push_back(s);
			s = "path=";
			s += path.string();
			lines.push_back(s);
			s = "season=";
			s += to_string(season);
			lines.push_back(s);
			s = "episode=";
			s += to_string(episode);
			lines.push_back(s);
			//let's just read the rest of the information about this
			for(int i = 0; i < 4; i++)				
				getline(ifstream, line);
		}
		else
			lines.push_back(line);
	}

	fs::ofstream ofstream(SERIES_FILE, fs::fstream::out | fs::fstream::trunc);
	for(string a : lines) {
		if(!ofstream)
			cout << "something wrong with stream\n";
		ofstream << a << "\n";
	}
	ofstream.close();
}
string series::fieldName(series::field f) {
	switch(f) {
	case series::field::name:
		return "name";
	case series::field::path:
		return "path";
	case series::field::season:
		return "season";
	case series::field::episode:
		return "episode";
	default:
		return "invalid field";
	}
}
void series::populateFields() {
	fs::ifstream ifs(SERIES_FILE);
	string line;
	while(ifs && !ifs.eof()) {
		if(line == ("name=" + name)) {
			string temp;
			//it seems the first getline will just "clear" the previous line
			//the reason we use this is because of the possibility of white spaces in path
			getline(ifs, temp);
			getline(ifs, temp);
			path = temp.substr(temp.find("=") + 1);
			ifs >> temp;
			season = stoi(temp.substr(temp.find("=") + 1));
			ifs >> temp;
			episode = stoi(temp.substr(temp.find("=") + 1));
			ifs.close();
			return;
		}
		ifs >> line;
	}
}
fs::path series::getNextEpisode() {
	if(seasonPath == "")
		setSeasonPath();

	string rgxString(R"(.*(episode|e|_|-|\.|\s)0?)" + to_string(episode) + R"(\D+.*\.)");
	string rgxStringWithType(rgxString);
	rgxStringWithType += "(";
	unsigned int count = 0;
	for(string type : validTypes) {
		++count;
		rgxStringWithType += type;
		if(count != validTypes.size())
			rgxStringWithType += "|";
	}
	rgxStringWithType += ")";
//	rgxString = string("/") + rgxString + "$";
	rgxStringWithType = string("/") + rgxStringWithType + "$";
	regex rgx(rgxString, regex_constants::icase);
	regex rgxWithType(rgxStringWithType, regex_constants::icase);

	fs::directory_iterator dirIt(seasonPath);
	for(auto file : dirIt) {
		if(regex_search(file.path().string(), rgxWithType) && file.status().type() == fs::file_type::regular_file)
			return file.path();
		else if(regex_search(file.path().string(), rgx)) {
			fs::directory_iterator dirItEp(file.path());
			for(auto epFile : dirItEp) {
				if(regex_search(epFile.path().string(), rgxWithType) && epFile.status().type() == fs::file_type::regular_file) {
					return epFile.path();
				}
			}	
		}
	}
	return fs::path();
}	
void series::setSeasonPath() {
	fs::path p = getPathFromRegex(path, R"(.*(season|s).*)" + to_string(season) + R"((\D+.*)?)", fs::file_type::directory_file);
	//if we found no result, lets assume that we're watching an anime or something that has no seasons
	//so let's use the current path as the wanted season directory
	seasonPath = p == "" ? path : p;
}
//also give the wanted filetype (directory or regular file)
fs::path series::getPathFromRegex(fs::path p, string regexString, fs::file_type type) {
	regexString = string("/") + regexString + "$";
	//ignore case
	regex rgx(regexString, regex_constants::icase);
	fs::directory_iterator dirIt(p);
	for(fs::directory_entry file : dirIt)
		if(regex_search(file.path().string(), rgx) && file.status().type() == type)
				return file.path();
	return fs::path();
}
