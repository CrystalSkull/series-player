#include <string>

class Series {
public:
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
	void setPath(std::string);
	void setSeason(int);
	void setEpisode(int);
	
	std::string getName();
	std::string getPath();
	int getSeason();
	int getEpisode();

	void incrementSeason();
	void incrementEpisode();

private:
	std::string name, path;
	int season, episode;
};
