#include "series.h"

using namespace std;
namespace fs = boost::filesystem;

//public
void Series::setName(string n) {
	name = n;
}	
/*void Series::setPath(string p) {
	path = p;
}*/
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
	//string() to get rid of the quotation marks
	fs << "path=" << path.string() << "\n";
	fs << "season=" << season << "\n";
	fs << "episode=" << episode << flush;
	fs.close();
}
void Series::updateSeriesFile(fs::path p) {
	fs::ifstream ifstream(p);
	string line;
	vector<string> lines;
	while(ifstream && !ifstream.eof()) {
		ifstream >> line;
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
				ifstream >> line;
		}
		else
			lines.push_back(line);
	}

	fs::ofstream ofstream(p, fs::fstream::out | fs::fstream::trunc);
	for(string a : lines) {
		if(!ofstream)
			cout << "something wrong with stream\n";
		ofstream << a << "\n";
	}
	ofstream.close();
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
