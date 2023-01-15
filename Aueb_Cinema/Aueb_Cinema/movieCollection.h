#include <vector>
#include "film.h"
#include <unordered_set>
#include "defines.h"

class MovieCollection
{
public:
	static MovieCollection* getInstance();
	static void releaseInstance();
	std::unordered_set<std::string> getGenres() const { return m_genres_set; }
	std::vector<Film> getSearchedMovies(std::string title, std::string director, std::string actor, std::unordered_set<std::string> genre, int yearMIN, int yearMAX);
	std::vector<Film> getAllMovies() const { return m_movies; }
	int getYearMin() const { return m_yearMin; }
	int getYearMax() const { return m_yearMax; }
private:
	static MovieCollection* mc_instance;
	void parseXML();
	std::vector<Film> m_movies{};
	std::unordered_set<std::string> m_genres_set{};
	// Movies matching the search criteria.
	std::vector<Film> m_selectedMovies{};
	int m_yearMin = 10000;
	int m_yearMax = 0;
	Pair m_yearRange;
	MovieCollection() {};
	~MovieCollection() {}

};