#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

class Series {
public:
	enum class field {name, path, season, episode};
	Series(){};
	Series(std::string n, std::string p, int s, int e) :
		name(n),
		path(p),
		season(s),
		episode(e){}
	Series(Series&& s): 
		name(std::move(s.name)),
		path(std::move(s.path)),
		season(std::move(season)),
		episode(std::move(episode)){}

	Series& operator=(Series&& s) {
		name = std::move(s.name);
		path = std::move(s.path);
		season = std::move(s.season);
		episode = std::move(s.episode);
		return *this;
	}

	void setName(std::string);
	void setPath(boost::filesystem::path);
	//void setPath(std::string);
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
private:
	std::string name;
	boost::filesystem::path path;
	int season, episode;

	std::string fieldName(field);
};
