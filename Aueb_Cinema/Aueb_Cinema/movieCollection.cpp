#include "movieCollection.h"
#include "defines.h"
#include <unordered_map>
#include <set>
#include <vector>
#include "film.h"
#include <string>

MovieCollection* MovieCollection::mc_instance = nullptr;

MovieCollection* MovieCollection::getInstance()
{
	if (!mc_instance)
	{
		mc_instance = new MovieCollection();
		mc_instance->parseXML();
	}
	return mc_instance;
}

void MovieCollection::releaseInstance()
{
	if (mc_instance) delete mc_instance;
	mc_instance = nullptr;
}

void MovieCollection::parseXML()
{
	// Open XML.
	std::ifstream file(DATA_PATH + std::string("movies.xml"));
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("<movie>") != std::string::npos)
		{
			std::getline(file, line);
			std::string title = line.substr(8, line.length() - 16);
			std::getline(file, line);
			std::string director = line.substr(11, line.length() - 22);
			std::getline(file, line);
			std::string actorsStr = line.substr(9, line.length() - 18);
			std::getline(file, line);
			std::string genreStr = line.substr(8, line.length() - 16);
			std::getline(file, line);
			int year = std::stoi(line.substr(7, line.length() - 14));
			std::getline(file, line);
			std::string path = line.substr(9, line.length() - 18);
			std::getline(file, line);
			std::string description = line.substr(14, line.length() - 28);

			std::stringstream ss(actorsStr);
			std::vector<std::string> actors;
			std::string actor;
			while (std::getline(ss, actor, ','))
			{
				actors.push_back(actor);
			}
			std::stringstream ss2(genreStr);
			std::vector<std::string> genres;
			std::string genre;
			while (std::getline(ss2, genre, ','))
			{
				genres.push_back(genre);
				
			}
			Film film(title, director, actors, genres, year, path, description);
			m_movies.emplace_back(title, director, actors, genres, year, path, description);
			for (std::string str : genres)
			{
				m_genres_set.insert(str);
			}
			if (year < m_yearMin) m_yearMin = year;
			if (year > m_yearMax) m_yearMax = year;
		}
	}
	file.close();
	m_yearRange.x = m_yearMin;
	m_yearRange.y = m_yearMax;
}

std::vector<Film> MovieCollection::getSearchedMovies(std::string title, std::string director, std::string actor, std::unordered_set<std::string> genre, int yearMIN, int yearMAX)
{
	m_selectedMovies.clear();
	for (auto it = m_movies.begin(); it != m_movies.end(); ++it)
	{
		if (title != "" && it->getTitle().find(title) == std::string::npos) continue;
		if (director != "" && it->getDirector().find(director) == std::string::npos) continue;
		bool actorFound = actor == "" ? true : false;
		if (actorFound == false)
		{
			for (std::string str : it->getActors())
			{
				if (str.find(actor) != std::string::npos)
				{
					actorFound = true;
					break;
				}
			}
			if (actorFound == false) continue;
		}
		if (yearMIN > it->getYear() || it->getYear() > yearMAX) continue;
		bool genreFound = genre.empty() ? true : false;
		if (genreFound == false)
		{
			for (std::string str : it->getGenres())
			{
				if (genre.find(str) != genre.end())
				{
					genreFound = true;
					break;
				}
			}
			if (genreFound == false) continue;
		}
		m_selectedMovies.push_back(*it);
		
	}
	return m_selectedMovies;
}
