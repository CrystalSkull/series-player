class series {
public:
	enum class field {name, path, season, episode};
	series(std::string n) :
		name(n){populateFields();};
	series(std::string n, std::string p, int s, int e) :
		name(n),
		path(p),
		season(s),
		episode(e){}

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

	void addSeriesToFile();
	//apparently the easiest way to handle the files is to read all of it into memory and then rewrite
	void updateSeriesFile();
	boost::filesystem::path getNextEpisode();	
private:
	std::vector<std::string> validTypes{"mkv", "mp4"};
	std::string name;
	boost::filesystem::path path = "", seasonPath = "";
	int season, episode;
	const boost::filesystem::path SERIES_FILE = std::string(std::getenv("HOME")) + "/.series";

	std::string fieldName(field);
	void populateFields();
	void setSeasonPath();
	//will return the path to the first hit from the regex
	boost::filesystem::path getPathFromRegex(boost::filesystem::path, std::string, boost::filesystem::file_type);
};
