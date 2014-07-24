#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <cstdlib>
//for directory control
#include <dirent.h>

//amount of lines in each series
int lineMultiplier = 4;

std::string getEpisodeFile(std::string, int);
std::string getSeasonDir(std::string, int);
bool hasMatch(std::string, std::string);
void incrementEpisode(int, int);
void incrementSeason(int, int);
void writeVectorToFile(std::vector<std::string>&);
std::vector<std::string> getSeries();
bool seriesExists(std::string, std::vector<std::string>&);
int getSeriesNumber(std::string, std::vector<std::string>&);
std::string getPath(int, std::vector<std::string>&);
int getSeason(int, std::vector<std::string>&);
int getEpisode(int, std::vector<std::string>&);
int stringToInt(std::string);
int charToInt(char);
std::string getFullPath(std::string, std::string, std::string);
void createNewSeries(std::string, std::string, std::string, std::string);
void printHelp();
void printError();
std::string getInput(int, char* [], std::string);
std::string getInputName(int ,char* []);
std::string getInputPath(int ,char* []);
std::string getInputSeason(int ,char* []);
std::string getInputEpisode(int ,char* []);
bool badExtension(std::string);

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "You need to specify a series to watch or create a new one" << std::endl;
		printHelp();
	}
	else if(argc == 2)
	{
		std::string arg1 (argv[1]);
		if(arg1 == "-h" || arg1 == "--help")
			printHelp();
		else
		{
			while(true)
			{
				std::string name (argv[1]);

				std::vector<std::string> series = getSeries();

				if(seriesExists(name, series))
				{
					int seriesNumber = getSeriesNumber(name, series);
					std::string parentDir ( getPath(seriesNumber, series) );
					int season = getSeason(seriesNumber, series);
					int episode = getEpisode(seriesNumber, series);

					std::string seasonDir ( getSeasonDir(parentDir, season));

					if(!seasonDir.empty())
					{
						std::string episodeFile ( getEpisodeFile( (parentDir + seasonDir), episode));
						
						if(!episodeFile.empty())
						{
							std::string fullPath = getFullPath(parentDir, seasonDir, episodeFile);

							std::string command ( "mpv --fs \"" + fullPath + "\"");

							int returnValue = system(command.c_str());
						
							if(returnValue == 0)
							{
								incrementEpisode(seriesNumber, episode);
							}
							else
							{
								//TODO ask if we want to increment the episode
								std::cout << "There was a problem playing this file" << std::endl;
								std::cout << "Would you like to increment the episode number? (y/n)" << std::endl;
								char c;
								std::cin >> c;
								if(c == 'y')
									incrementEpisode(seriesNumber, episode);
								else
									break;
							}	
						}
						else
						{
							//TODO ask if we want to increment the season
							std::cout << "Could not find episode number " << episode << std::endl;
							std::cout << "Would you like to increment the season number? (y/n)" << std::endl;
							char c;
							std::cin >> c;
							if(c == 'y')
								incrementSeason(seriesNumber, season);
							else
								break;
						}

					}
					else
					{
						//TODO ask if we want to increment the season
						std::cout << "Could not find season number " << season << std::endl;

						std::cout << "Would you like to increment the season number? (y/n)" << std::endl;
						char c;
						std::cin >> c;
						if(c == 'y')
							incrementSeason(seriesNumber, season);
						else
							break;
					}
				}
				else
				{
					std::cout << "You have not specified this series" << std::endl;
				}
			}
		}
	}
	else
	{
		std::string name = getInputName(argc, argv);
		std::string path = getInputPath(argc, argv);
		std::string season = getInputSeason(argc, argv);
		std::string episode = getInputSeason(argc, argv);
		
		//if they have not entered the required fields
		if(name.empty() || path.empty())
		{
			printError();
			printHelp();
		}
		createNewSeries(name, path, season, episode);
	}
	return 0;
}
//return NULL if no episode was found
std::string getEpisodeFile(std::string path, int episodeNumber)
{
	DIR* dir;
	struct dirent* ent;

	dir = opendir(path.c_str());
	if(dir != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			//name of a episode
			std::string episode (ent->d_name);
			//x amount of characters and then either a whitespace or dot or an e
			//and sometimes single digit episodes are named e.g. 04
			std::string rgx (".*(\\s|\\.|e|E)0?");
			//the episode number
			rgx += std::to_string(episodeNumber);
			//whitespace, dot and not a number after the episodenumber
			//then end with dot and the extension
			rgx += "(\\s|\\.|\\D).*(\\.\\w{3})$";
			if(hasMatch(episode, rgx))
			{
				//get the extension
				std::string extension = episode.substr(episode.length()-3);
				//make sure it's not a bad one, like .txt or .nfo
				if(!badExtension(extension))
				{
					closedir(dir);
					return episode;
				}
			}
		}
	}
	closedir(dir);
	return std::string();
}
//return NULL if no dir was found
std::string getSeasonDir(std::string path, int seasonNumber)
{
	DIR* dir;
	struct dirent* ent;
	
	dir = opendir(path.c_str());
	if(dir != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			//get the name a directory
			std::string innerDir (ent->d_name);
			//x amount of characters and then either a whitespace or dot
			std::string rgx (".*(\\s|\\.)");
			//the season number
			rgx += std::to_string(seasonNumber);
			//optionally a whitespace or dot and then x amount of characters
			rgx += "(\\s|\\.)?.*";
			if(hasMatch(innerDir, rgx))
			{
				closedir(dir);

				innerDir += "/";
				return innerDir;
			}
		}
	}
	closedir(dir);
	//return NULL if no dir was found
	return std::string();
}
bool hasMatch(std::string searchString, std::string regexString)
{
	std::smatch m;
	std::regex rgx (regexString);
	//returns true if we find a match
	bool returnVal = std::regex_match(searchString, rgx);
	return returnVal;
}
void incrementEpisode(int seriesNumber, int episode)
{
	std::vector<std::string> series = getSeries();
	
	int pos = seriesNumber*lineMultiplier + 3;
	series.erase(series.begin() + pos);
	std::string newEpisode = std::to_string(episode+1);
	if(series.begin() + pos == series.end())
		series.push_back(newEpisode);
	else
		series.insert(series.begin() + pos, newEpisode);

	writeVectorToFile(series);
}	
void incrementSeason(int seriesNumber, int season)
{
	std::vector<std::string> series = getSeries();
	int pos = seriesNumber*lineMultiplier +2;
	series.erase(series.begin() + pos);
	std::string newSeason = std::to_string(season+1);
	if(series.begin() + pos == series.end())
		series.push_back(newSeason);
	else
		series.insert(series.begin() + pos, newSeason);

	writeVectorToFile(series);
	//set the episode number to 0
	incrementEpisode(seriesNumber, 0);
}
void writeVectorToFile(std::vector<std::string>& series)
{
	std::ofstream seriesFile ("series", std::fstream::out | std::fstream::trunc);

	if(seriesFile.is_open())
	{
		std::vector<std::string>::iterator it = series.begin();
		while(it != series.end())
		{
			seriesFile << "name=" << *it << "\n";
			it++;
			seriesFile << "path=" << *it << "\n";
			it++;
			seriesFile << "season=" << *it << "\n";
			it++;
			seriesFile << "episode=" << *it << "\n";
			it++;
			//lets add another newline for easier readability
			seriesFile << "\n";
		}
		seriesFile.close();
	}
}
//return the series file 
std::vector<std::string> getSeries()
{
	std::vector<std::string> series;
	std::string line;
	std::ifstream seriesFile ("series");

	if(seriesFile.is_open())
	{
		int lineNumber = 0;
		while(std::getline(seriesFile, line))
		{
			//if it's an empty line, just skip it
			if(line.empty())
				continue;

			switch(lineNumber++)
			{
				case 0:
					//remove name=
					line.erase(0, 5);
					break;
				case 1:
					//remove path=
					line.erase(0, 5);
					break;
				case 2:
					//remove season=
					line.erase(0, 7);
					break;
				case 3:
					//remove episode=
					line.erase(0, 8);
					break;
			}
			series.push_back(line);
			if(lineNumber == 4)
				lineNumber = 0;
		}
		seriesFile.close();
	}
	return series;
}

bool seriesExists(std::string name, std::vector<std::string>& series)
{
	if(getSeriesNumber(name, series) != -1)
		return true;

	return false;
}
//returns -1 if no series was found
int getSeriesNumber(std::string name, std::vector<std::string>& series)
{	
	for(unsigned int i = 0; i < series.size(); i += lineMultiplier)
	{
		if(series.at(i) == name)
			return i/lineMultiplier;
	}
	return -1;
}
std::string getPath(int seriesNumber, std::vector<std::string>& series)
{
	std::string path = series.at(seriesNumber*lineMultiplier +1);
	//to add '/'
	path = getFullPath(path, "", "");
	return path;
}
int getSeason(int seriesNumber, std::vector<std::string>& series)
{
	std::string seasonString = series.at(seriesNumber*lineMultiplier + 2);
	return stringToInt(seasonString);
}
int getEpisode(int seriesNumber, std::vector<std::string>& series)
{
	std::string episodeString = series.at(seriesNumber*lineMultiplier + 3);
	return stringToInt(episodeString);
}
int stringToInt(std::string number)
{
	int season = 0;	
	for(unsigned int i = 0; i < number.size(); i++)
	{
		season *= 10;
		season += charToInt(number.at(i));
	}
	return season;
}
int charToInt(char number)
{
	//48 in ascii = 0
	return number - 48;
}
char intToChar(int number)
{
	//48 in ascii = 0
	return number + 48;
}
std::string intToString(int number)
{
	char* c = new char[sizeof(number)/sizeof(int)];
	
	int pos = sizeof(number)/sizeof(int) - 1;
	while(number)
	{
		c[pos] = intToChar(number%10);
		number /= 10;
		pos--;
	}
	std::string numberString (c);
	delete[] c;
	return numberString;
}
std::string getFullPath(std::string parent, std::string seasonDir, std::string episodeFile)
{
	if(!parent.empty() && parent.back() != '/')
		parent.push_back('/');
	if(!seasonDir.empty() && seasonDir.back() != '/')
		seasonDir.push_back('/');

	return std::string(parent + seasonDir + episodeFile);
}
void createNewSeries(std::string name, std::string path, std::string season = "1", std::string episode = "1")
{
	std::vector<std::string> series = getSeries();

	series.push_back(name);
	series.push_back(path);
	series.push_back(season);
	series.push_back(episode);

	writeVectorToFile(series);
}
std::string getInput(int argc, char* input[], std::string X)
{	
	X = "--" + X;

	for(int i = 1; i < argc; i++)
	{
		if(input[i] == X)
			return std::string(input[i+1]);
	}
	return std::string();
}
std::string getInputName(int argc, char* input[])
{
	return getInput(argc, input, "name");
}
std::string getInputPath(int argc, char* input[])
{
	return getInput(argc, input, "path");
}
std::string getInputSeason(int argc, char* input[])
{
	std::string season = getInput(argc, input, "season");
	if(season.empty())
		return "1";
	else
		return season;
}
std::string getInputEpisode(int argc, char* input[])
{
	std::string episode = getInput(argc, input, "episode");
	if(episode.empty())
		return "1";
	else
		return episode;
}
void printHelp()
{
	std::cout << "Usage: " << std::endl;
	std::cout << "series-player [series] or series-player [args]" << std::endl << std::endl;
	std::cout << "Required arguments: " << std::endl;
	std::cout << "--name [NAME OF SERIES]" << std::endl;
	std::cout << "--path [PARENT DIRECTORY]" << std::endl << std::endl;
	std::cout << "Optional arguments: " << std::endl;
	std::cout << "--season [SEASON NUMBER] " << std::endl;
	std::cout << "--episode [EPISODE NUMBER] " << std::endl;
}
void printError()
{
	std::cout << "Wrong usage of series-player" << std::endl;
}
bool badExtension(std::string extension)
{
	std::string badExtensions[] = {"txt", "nfo"};
	for(unsigned int i = 0; i < sizeof(badExtensions)/sizeof(*badExtensions); i++)
	{
		if(extension == badExtensions[i])
				return true;
	}
	return false;
}
