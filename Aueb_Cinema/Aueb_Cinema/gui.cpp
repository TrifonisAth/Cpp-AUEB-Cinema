#include "gui.h"
#include <graphics.h>
#include "defines.h"
#include <vector>
#include "movieCollection.h"
#include <scancodes.h>

Gui* Gui::m_instance = nullptr;
MovieCollection* mc = MovieCollection::getInstance();
bool Gui::searchTriggered = false;
graphics::MouseState ms;
void Gui::search()
{
	int i = 0;
	if (m_filter_state == STATE_NO_FILTERS)
	{
		m_movieCards.clear();
		std::vector<Film> movies = mc->getAllMovies();
		for (Film& m : movies)
		{
			m_movieCards.emplace_back(MovieCard(CANVAS_WIDTH * 0.24f + i % 4 * 230, CANVAS_HEIGHT * 0.55f + i / 4 * 230, 210, 210, Trio(1,1,1), Trio(0,0,0), m));
			++i;
		}
		return;
	}
	m_movieCards.clear();
	std::vector<Film> resultMovies = mc->getSearchedMovies(m_drawer.m_searchBoxes[0].getText(), m_drawer.m_searchBoxes[1].getText(), m_drawer.m_searchBoxes[2].getText(), m_selectedGenres, m_drawer.m_sliders[0].getYear(), m_drawer.m_sliders[1].getYear());
	for (Film& m : resultMovies)
	{
		// Create a movie card for each returned movie.
		m_movieCards.emplace_back(MovieCard(CANVAS_WIDTH * 0.24f + i % 4 * 230, CANVAS_HEIGHT * 0.55f + i / 4 * 230, 210, 210, Trio(1,1,1), Trio(0,0,0), m));
		++i;
	}
}


void Gui::draw()
{
	if (m_state == STATE_MAIN_MENU)
	{
		graphics::Brush br1;
		SETCOLOR(br1.fill_color, AUEB_RED.x, AUEB_RED.y, AUEB_RED.z);
		graphics::Brush br2;
		SETCOLOR(br2.fill_color, 1, 1, 1);
		graphics::drawText(CANVAS_WIDTH * 0.028f, CANVAS_HEIGHT * 0.05f, TEXT_BIG, "AUEB", br1);
		graphics::drawText(CANVAS_WIDTH * 0.028f, CANVAS_HEIGHT * 0.08f, TEXT_BIG, "CINEMA", br2);
		m_drawer.draw();
		// Draw movie cards.
		for (MovieCard& m : m_movieCards)
		{
			m.draw();
		}
	}
}

void Gui::init()
{
	m_instance->setBackground(0.1f, 0.1f, 0.1f);
	m_state = STATE_MAIN_MENU;
	search();
}

// Gets called for every frame.
void Gui::update()
{
	switch(m_state)
	{
	case STATE_INIT:
		m_instance->init();
		return;
	case STATE_MAIN_MENU:
		eventHandler();
	}
}

void Gui::eventHandler()
{
	// use a variabe to store mouse coords.
	graphics::getMouseState(ms);
	float coordX = graphics::windowToCanvasX(ms.cur_pos_x);
	float coordY = graphics::windowToCanvasY(ms.cur_pos_y);
	m_drawer.checkHovering(coordX, coordY);
	if (m_drawer.m_state == FilterDrawer::STATE_EXPANDED)
	{
		coordY -= MovingComponent::m_offsetY;
		if (ms.button_left_pressed)
		{
			for (GenreButton& x : m_drawer.m_genreButtons)
			{
				if (x.contains(coordX, coordY))
				{
					x.onclick();
					graphics::playSound(AUDIO_PATH + std::string("activation.wav"), 0.7f);
					searchTriggered = true;
				}
				if (x.isActivated())
				{
					m_selectedGenres.insert(x.getLabel());
				}
				else
				{
					m_selectedGenres.erase(x.getLabel());
				}
			}
			for (SearchBox& b : m_drawer.m_searchBoxes)
			{
				if (b.contains(coordX, coordY))
				{
					b.onclick();
					graphics::playSound(AUDIO_PATH + std::string("activation.wav"), 0.7f);
				}
			}
			if (m_drawer.m_clearButton.contains(coordX, coordY))
			{
				m_drawer.m_clearButton.onclick();
				graphics::playSound(AUDIO_PATH + std::string("activation.wav"), 0.7f);
				clearFilters();
				search();
			}
		}
		if (ms.button_left_down)
		{
			for (Slider& s : m_drawer.m_sliders)
			{
				int last_year = s.getYear();
				s.onclick(coordX, coordY);
				if (last_year != s.getYear())
				{
					graphics::playSound(AUDIO_PATH + std::string("hover.wav"), 0.7f);
					searchTriggered = true;
				}
			}
		}
		char c = getKeyPressed();
		if (c != 0)
		{
			for (SearchBox& b : m_drawer.m_searchBoxes)
			{
				if (b.isFocused())
				{
					std::string text = b.getText();
					b.onfocus(c);
					if (text != b.getText()) searchTriggered = true;
				}
			}
		}
		if (searchTriggered)
		{
			m_filter_state = STATE_HAS_FILTERS;
			search();
			searchTriggered = false;
		}
		coordY += MovingComponent::m_offsetY;
	}
	for (MovieCard& m : m_movieCards)
	{
		if (m.contains(coordX, coordY) && !m.isFocused())
		{
			m.onhover();
			graphics::playSound(AUDIO_PATH + std::string("hover.wav"), 0.7f);
		}
		if (m.isFocused())
		{
			global_timer += graphics::getDeltaTime() / 1000;
			if (global_timer >= 3)
			{
				global_timer = 0;
				m.increaseCounter();
			}
			m.setTimer(global_timer);
		}
		else
		{
			m.resetCounter();
		}
	}
}

Gui* Gui::getInstance()
{
	if (!m_instance)
	{
		m_instance = new Gui();
	}
	return m_instance;
}

void Gui::releaseInstance()
{
	if (m_instance) delete m_instance;
	m_instance = nullptr;
	mc->releaseInstance();
}

void Gui::setBackground(float r, float g, float b)
{
	graphics::Brush br;
	SETCOLOR(br.fill_color, r, g, b);
	graphics::setWindowBackground(br);
}

char Gui::getKeyPressed()
{
	
	bool isShiftPressed = false;
	char c = 0;
	if (graphics::getKeyState(graphics::SCANCODE_LSHIFT) || graphics::getKeyState(graphics::SCANCODE_RSHIFT)) isShiftPressed = true;
	for (int i = graphics::SCANCODE_A; i <= graphics::SCANCODE_Z; i++)
	{
		if (graphics::getKeyState(graphics::scancode_t(i)) && m_button_timer >= BUTTON_DELAY)
		{
			c = isShiftPressed ? i + 61 : i + 93;
			m_button_timer = 0;
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && m_button_timer >= BUTTON_DELAY)
	{
		c = ' ';
		m_button_timer = 0;

	}
	if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE) && m_button_timer >= BUTTON_DELAY)
	{
		c = 8;
		m_button_timer = 0;
	}
	m_button_timer += (unsigned int) graphics::getDeltaTime();
	if (m_button_timer > BUTTON_DELAY)
		m_button_timer = BUTTON_DELAY;
	return c;
}

void Button::draw() const{}

void Gui::clearFilters()
{
	for (GenreButton& b : m_drawer.m_genreButtons)
	{
		b.setActive(false);
	}
	for (SearchBox& b : m_drawer.m_searchBoxes)
	{
		b.setText("");
	}
	m_drawer.m_sliders[0].setYear(mc->getYearMin());
	m_drawer.m_sliders[0].setCoordinateX(CANVAS_WIDTH * 0.26f - 75);
	m_drawer.m_sliders[1].setYear(mc->getYearMax());
	m_drawer.m_sliders[1].setCoordinateX(CANVAS_WIDTH * 0.45f + 75);
	m_filter_state = STATE_NO_FILTERS;
}

void Slider::draw() const
{
	graphics::Brush br;
	Trio col = getColor();
	Trio col2 = getColorHighlight();
	SETCOLOR(br.outline_color, 0, 0, 0);
	SETCOLOR(br.fill_color, col.x, col.y, col.z);
	// Draw the middle.
	graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), 3, 12, br);
	// Draw the axis.
	graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), 150, 3, br);
	SETCOLOR(br.fill_color, col2.x, col2.y, col2.z);
	// Draw label.
	graphics::drawText(getCenterX() - 130, getCenterY() + getOffsetY() + 5, TEXT_SMALL, getLabel(), br);
	// Draw selected year.
	graphics::drawText(getCoordinateX() - 15, getCenterY() + getOffsetY() - 20, TEXT_SMALL, std::to_string(getYear()), br);
	// Draw the slider.
	graphics::drawRect(getCoordinateX(), getCenterY() + getOffsetY(), getWidth(), getHeight(), br);
	// Draw min and max year.
	SETCOLOR(br.fill_color, 1, 1, 1);
	graphics::drawText(getCenterX() - 85, getCenterY() + getOffsetY() + 30, TEXT_SMALL, std::to_string(mc->getYearMin()), br);
	graphics::drawText(getCenterX() + 60, getCenterY() + getOffsetY() + 30, TEXT_SMALL, std::to_string(mc->getYearMax()), br);
}

void MovieCard::focusedDraw(){}

void Slider::onclick(float x, float y)
{
	if (x >= getCenterX() - 75 && x <= getCenterX() + 75 && y >= getCenterY() - getHeight() / 2 && y <= getCenterY() + getHeight() / 2)
	{
		setCoordinateX(x);
		setYear((getCoordinateX() - getCenterX() + 78) * (mc->getYearMax() - mc->getYearMin()) / 150.0f + mc->getYearMin());
	}
}

void FilterDrawer::initWidgets()
{
	// Create search boxes.
	m_searchBoxes.emplace_back(SearchBox(CANVAS_WIDTH * 0.71f, -10, 300, 25, Trio(1,1,1), YELLOW, "Title"));
	m_searchBoxes.emplace_back(SearchBox(CANVAS_WIDTH * 0.71f, 30, 300, 25, Trio(1,1,1), YELLOW, "Director"));
	m_searchBoxes.emplace_back(SearchBox(CANVAS_WIDTH * 0.71f, 70, 300, 25, Trio(1,1,1), YELLOW, "Actor"));
	// Create genre buttons.
	int i = 0;
	for (const auto& g : mc->getGenres())
	{
		m_genreButtons.emplace_back(GenreButton(CANVAS_WIDTH * 0.2f + i % 5 * 75, 60 + i / 5 * 30, 70.0f, 20.0f, Trio(1,1,1), GREEN, g));
		++i;
	}
	// Create sliders.
	m_sliders.emplace_back(Slider(CANVAS_WIDTH * 0.26f, 10, 15, 25, Trio(1,1,1), GREEN, "From:", mc->getYearMin(), CANVAS_WIDTH * 0.26f - 75));
	m_sliders.emplace_back(Slider(CANVAS_WIDTH * 0.45f, 10, 15, 25, Trio(1,1,1), GREEN, "To:", mc->getYearMax(), CANVAS_WIDTH * 0.45f + 75));

}
