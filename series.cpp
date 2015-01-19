#include "series.h"

using namespace std;

//public
void Series::setName(string n) {
	name = n;
}	
void Series::setPath(string p) {
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
string Series::getPath() {
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
