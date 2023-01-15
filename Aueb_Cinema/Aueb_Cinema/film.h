#pragma once
#include <string>
#include <vector>

class Film
{
private:
	std::vector<std::string> m_selectedGenres{};
	std::vector<std::string> m_actors{};
	std::string m_title;
	std::string m_director;
	std::string m_path;
	std::string m_description;
	int m_year;
public:
	Film(std::string title, std::string director, std::vector<std::string> actors, std::vector<std::string> genre, int year, std::string path, std::string description)
		: m_title(title), m_director(director), m_path(path), m_description(description), m_year(year)
	{
		for (int i = 0; i < genre.size(); i++)
		{
			m_selectedGenres.push_back(genre[i]);
		}
		for (int i = 0; i < actors.size(); i++)
		{
			m_actors.push_back(actors[i]);
		}
	}
	std::string getTitle() const { return m_title; };
	std::string getDirector() const { return m_director; };
	std::vector<std::string> getActors() const { return m_actors; };
	std::vector<std::string> getGenres() const { return m_selectedGenres; };
	int getYear() const { return m_year; };
	std::string getPath() const { return m_path; };
	std::string getDescription() const { return m_description; };
	~Film(){};
};

