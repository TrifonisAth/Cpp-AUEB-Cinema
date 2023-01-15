#pragma once
#include <string>
#include <functional>
#include <vector>
#include "defines.h"
#include <unordered_set>
#include "widget.h"

class Gui
{
public:
	void search();
	void draw();
	void init();
	void update();
	static Gui* m_instance;
	static Gui* getInstance();
	static void releaseInstance();
	void setBackground(float r, float g, float b);
	char getKeyPressed();
	void clearFilters();
	unsigned int m_button_timer = 0;
private:
	// Could add more states in the future, like login, register, settings, etc.
	enum gui_state
	{
		STATE_INIT,
		STATE_MAIN_MENU
	};
	// Could add states for secondary filter windows.
	enum filter_state
	{
		STATE_NO_FILTERS,
		STATE_HAS_FILTERS
	};
	gui_state m_state = STATE_INIT;
	filter_state m_filter_state = STATE_NO_FILTERS;
	static bool searchTriggered;
	std::vector<MovieCard> m_movieCards{};
	std::unordered_set<std::string> m_selectedGenres{};
	FilterDrawer m_drawer;
	float global_timer = 0;
	void eventHandler();
	Gui() {}
	~Gui() {};
};
