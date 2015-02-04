#include <iostream>
#include <regex>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

class series {
public:
	enum class field {name, path, season, episode};
	series(std::string n, boost::filesystem::path p) :
		name(n){populateFields(p);};
	series(std::string n, std::string p, int s, int e) :
		name(n),
		path(p),
		season(s),
		episode(e){}
	series(series&& s): 
		name(std::move(s.name)),
		path(std::move(s.path)),
		season(std::move(season)),
		episode(std::move(episode)){}

	series& operator=(series&& s) {
		name = std::move(s.name);
		path = std::move(s.path);
		season = std::move(s.season);
		episode = std::move(s.episode);
		return *this;
	}

	void setName(std::string);
	void setPath(boost::filesystem::path);
	void setSeason(int);
	void setEpisode(int);
	
	std::string getName();
	boost::filesystem::path getPath();
	int getSeason();
	int getEpisode();

	void incrementSeason();
	void incrementEpisode();

	void addSeriesToFile(boost::filesystem::path);
	//apparently the easiest way to handle the files is to read all of it into memory and then rewrite
	void updateSeriesFile(boost::filesystem::path);
	boost::filesystem::path getNextEpisode();	
private:
	std::vector<std::string> validTypes{"mkv", "mp4"};
	std::string name;
	boost::filesystem::path path = "", seasonPath = "";
	int season, episode;

	std::string fieldName(field);
	void populateFields(boost::filesystem::path);
	void setSeasonPath();
	//will return the path to the first hit from the regex
	boost::filesystem::path getPathFromRegex(boost::filesystem::path, std::string);
};
