#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <graphics.h>
#include "film.h"

constexpr unsigned int CANVAS_WIDTH = 1280;
constexpr unsigned int CANVAS_HEIGHT = 1280;
constexpr auto ASSET_PATH = ".\\assets\\";
constexpr auto IMAGE_PATH = ".\\assets\\images\\";
constexpr auto AUDIO_PATH = ".\\assets\\audio\\";
constexpr auto FONT_PATH = ".\\assets\\fonts\\nun.ttf";
constexpr auto DATA_PATH = ".\\assets\\data\\";
#define SETCOLOR(c,r,g,b) {c[0] = r; c[1] = g; c[2] = b;}
constexpr unsigned int TEXT_BIG = 40;
constexpr unsigned int TEXT_MEDIUM = 20;
constexpr unsigned int TEXT_SMALL = 14;
constexpr unsigned int BUTTON_DELAY = 150;
// Colors.
#define RED Trio(0.7f, 0.1f, 0.2f)
#define LIGHT_BROWN Trio(0.8f, 0.7f, 0.4f)
#define YELLOW Trio(0.9f, 0.9f, 0.2f)
#define GREEN Trio(0.3f, 0.7f, 0.3f)
#define DODGERBLUE Trio(0.2f, 0.5f, 0.8f)
#define AUEB_RED Trio(0.9f, 0.1f, 0.4f)
#define ORANGE Trio(0.7f, 0.7f, 0.2f)
const unsigned int LINE_LENGTH = 75;

class Pair
{
public:
	Pair() : x(0), y(0) {}
	Pair(float x, float y) : x(x), y(y) {}
	Pair(Pair const& v) : x(v.x), y(v.y) {}
	float x, y;
};

class Trio
{
public:
	Trio() : x(0), y(0), z(0) {}
	Trio(float x, float y, float z) : x(x), y(y), z(z) {}
	Trio(Trio const& v) : x(v.x), y(v.y), z(v.z) {}
	float x, y, z;
};

class Rectangle
{
private:
	Pair m_size;
	Pair m_center;
public:
	Rectangle(float x, float y, float w, float h)
		: m_center(x, y), m_size(w, h) {}
	Rectangle(Pair centers, Pair size)
		: m_center(centers), m_size(size) {}
	Rectangle() : m_center(0, 0), m_size(0, 0) {}
	float getMinX() const { return m_center.x - m_size.x / 2; }
	float getMinY() const { return m_center.y - m_size.y / 2; }
	float getMaxX() const { return m_center.x + m_size.x / 2; }
	float getMaxY() const { return m_center.y + m_size.y / 2; }
	float getCenterX() const { return m_center.x; }
	float getCenterY() const { return m_center.y; }
	float getWidth() const { return m_size.x; }
	float getHeight() const { return m_size.y; }
	bool contains(float x, float y) const { return (x >= getMinX() && x <= getMaxX() && y >= getMinY() && y <= getMaxY()); }
};

inline void printParagraph(std::string str)
{
	graphics::Brush br;
	SETCOLOR(br.fill_color, 1,1,1);
	std::stringstream stream(str);
	std::string line;
	char lineCount = 0;
	while (std::getline(stream, line))
	{
		std::stringstream line_stream(line);
		std::string word;
		std::string output;
		while (line_stream >> word)
		{
			if (LINE_LENGTH < word.length() + output.length()) 
			{
				graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.23f + 210 + lineCount * 25, TEXT_MEDIUM, output, br);
				output.clear();
				++lineCount;
			}
			output += word + " ";
		}
		graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.23f + 210 + lineCount * 25, TEXT_MEDIUM, output, br);
		++lineCount;
	}
}