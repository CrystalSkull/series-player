#include "series.h"

using namespace std;
namespace fs = boost::filesystem;

//public
void Series::setName(string n) {
	name = n;
}	
void Series::setPath(string p) {
	path = p;
}
void Series::setPath(fs::path p) {
	path = p;
}
void Series::setSeason(int s) {
	season = s;
}
void Series::setEpisode(int e) {
	episode = e;
}
string Series::getName() {
	return name;
}
boost::filesystem::path Series::getPath() {
	return path;
}
int Series::getSeason() {
	return season;
}
int Series::getEpisode() {
	return episode;
}
void Series::incrementSeason() {
	++season;
}
void Series::incrementEpisode() {
	++episode;
}
void Series::addSeriesToFile(fs::path p) {
	string start;
	if(fs::exists(p))
		start = "\n\n";
	fs::ofstream fs(p, fs::fstream::out | fs::fstream::app);
	//we want a starting new line if the file exists
	fs << start;
	fs << "name=" << name << "\n";
	fs << "path=" << path << "\n";
	fs << "season=" << season << "\n";
	fs << "episode=" << episode << flush;
	fs.close();
}
void Series::updateSeriesFile(fs::path p, string s, Series::field f) {
	fs::fstream fs(s, fs::fstream::out | fs::fstream::in);
	string line;
	vector<string> lines;
	while(fs >> line) {
		//make sure it's the correct series we're editing
		if(line == (fieldName(f) + "=" + name)) {
			//TODO edit the correct field here
			lines.push_back(fieldName(f) + name);
		}
		else
			lines.push_back(line);
	}
	while(fs << line);
	fs.close();
}
string Series::fieldName(Series::field f) {
	switch(f) {
	case Series::field::name:
		return "name";
	case Series::field::path:
		return "path";
	case Series::field::season:
		return "season";
	case Series::field::episode:
		return "episode";
	default:
		return "invalid field";
	}
}
