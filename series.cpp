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
	//if we've updated the season, we might as well update the path to the new season
	setSeasonPath();
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
}
void series::incrementEpisode() {
	++episode;
}
void series::addSeriesToFile(fs::path p) {
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
void series::updateSeriesFile(fs::path p) {
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
void series::populateFields(fs::path seriesFile) {
	fs::ifstream ifs(seriesFile);
	string line;
	while(ifs && !ifs.eof()) {
		if(line == ("name=" + name)) {
			string temp;
			ifs >> temp;
			path = temp.substr(temp.find("=") + 1);
			ifs >> temp;
			season = atoi(temp.substr(temp.find("=") + 1).c_str());
			ifs >> temp;
			episode = atoi(temp.substr(temp.find("=") + 1).c_str());
			ifs.close();
			return;
		}
		ifs >> line;
	}
}
fs::path series::getNextEpisode() {
	if(seasonPath == "")
		setSeasonPath();

	string rgxString(R"(*(\s|\.|-|_))" + to_string(episode) + R"((\s|\.|-|_)*.)");
	rgxString += "(";
	unsigned int count = 0;
	for(string type : validTypes) {
		++count;
		rgxString += type;
		if(count != validTypes.size())
			rgxString += "|";
	}
	rgxString += ")";

	return getPathFromRegex(seasonPath, rgxString);
}	
void series::setSeasonPath() {
	fs::path p = getPathFromRegex(path, R"(*(season|Season|s|S).*)" + to_string(season));
	//if we found no result, lets assume that we're watching an anime or something that has no seasons
	//so let's use the current path as the wanted season directory
	seasonPath = p == "" ? path : p;
}
fs::path series::getPathFromRegex(fs::path p, string regexString) {
	regex rgx(regexString);
	fs::directory_iterator dirIt(p);
	for(fs::directory_entry file : dirIt)
		if(regex_search(file.path().string(), rgx))
				return file.path();
	return fs::path();
}
