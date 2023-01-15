#pragma once
#include "defines.h"

class Widget
{
private:
	Rectangle m_bounds;
	Trio m_color;
	Trio m_color_highlight;
	static int m_next_uid;
	// Focused widget id.
	static int m_focus;
	const int m_uid = m_next_uid++;
	// Could use enum if there are more than 2 states. For now, it's just 2.
	bool m_activated = false;
protected:
	void setFocus() { m_focus = m_uid; }
public:
	virtual void draw() const = 0;
	virtual void onclick() {};
	static int getFocusedId() { return m_focus; }
	Trio getColor() const { return m_color; }
	Trio getColorHighlight() const { return m_color_highlight; }
	float getCenterX() const { return m_bounds.getCenterX(); }
	float getCenterY() const { return m_bounds.getCenterY(); }
	float getWidth() const { return m_bounds.getWidth(); }
	float getHeight() const { return m_bounds.getHeight(); }
	bool isFocused() const { return m_focus == m_uid; }
	bool contains(float x, float y) const;
	void setActive(bool h) { m_activated = h; }
	bool isActivated() const { return m_activated; }
	int getId() const { return m_uid; }
	virtual ~Widget() {}
	Widget() {};
	Widget(float x, float y, float w, float h, Trio col, Trio col_h)
		: m_color(col), m_color_highlight(col_h), m_bounds(x, y, w, h) {}
	Widget(const Widget& other)
		: m_bounds(other.m_bounds), m_color(other.m_color), m_color_highlight(other.m_color_highlight), m_activated(other.m_activated), m_uid(other.m_uid) {}
};


class MovingComponent: public Widget
{
public:
	void setOffsetY(float y) { m_offsetY = y; }
	float getOffsetY() const { return m_offsetY; }
	// You can change the initial offset here.
	MovingComponent(float x, float y, float w, float h, Trio col, Trio col_h)
	: Widget( x, y, w, h, col, col_h) {};
	MovingComponent() {};
	static float m_offsetY;
};


class Button : public MovingComponent
{
private:
	std::string m_label;
public:
	void activate();
	void draw() const override;
	std::string getLabel() const { return m_label; }
	Button(float x, float y, float w, float h, Trio col, Trio col_h, std::string label)
		: MovingComponent(x, y, w, h, col, col_h), m_label(label) {}
	Button() {}
	~Button() {};
};

class MovieCard : public Widget
{
private:
	Film m_film;
	Button m_button = Button(CANVAS_WIDTH * 0.2f, CANVAS_HEIGHT * 0.32f, 350, 350, Trio(1,1,1), Trio(0,0,0), "Album");
	int m_counter;
	float m_timer = 0;
public:
	MovieCard(float x, float y, float w, float h, Trio col, Trio col_h, Film film)
		: Widget(x, y, w, h, col, col_h), m_film(film), m_counter(0) {}
	void draw() const override;
	void focusedDraw();
	float getTimer() const { return m_timer; }
	void setTimer(float t) { m_timer = t; }
	void resetCounter() { m_counter = 0; }
	void onhover();
	void onclick() override;
	void increaseCounter() { m_counter = (m_counter + 1) % 3; }
	int getCounter() const { return m_counter; }
	Film getFilm() const { return m_film; }
	Button getButton() const { return m_button; }
};

class ClearFiltersButton : public Button
{
public:
	ClearFiltersButton()
		: Button(CANVAS_WIDTH * 0.7f, 120, 120, 35, GREEN, Trio(0, 0, 0), "Clear") {}
	void onclick() override;
	void draw() const override;
};

class GenreButton : public Button
{
private:
	std::string const m_genre;
public:
	void draw() const override;
	GenreButton(float x, float y, float w, float h, Trio col, Trio col_h, std::string label)
		: Button(x, y, w, h, col, col_h, label), m_genre(label) {}
	void onclick() override;
	GenreButton() {}
	~GenreButton() {}
	std::string getGenre() { return m_genre; }
};

class SearchBox : public MovingComponent
{
private:
	std::string m_prompt;
	std::string m_text = "";
public:
	void draw() const override;
	void onclick() override { setFocus(); }
	void onfocus(char c);
	void setText(std::string text) { m_text = text; }
	std::string getText() const { return m_text; }
	SearchBox(float x, float y, float w, float h, Trio col, Trio col_h, std::string prompt)
		: MovingComponent(x, y, w, h, col, col_h), m_prompt(prompt) {}
	std::string getPrompt() const { return m_prompt; }
};

class Slider : public Button
{
private:
	int m_year;
	float m_coordinateX;
public:
	void draw() const override;
	Slider(float x, float y, float w, float h, Trio col, Trio col_h, std::string label, int initYear, float initX)
		: Button(x, y, w, h, col, col_h, label), m_year(initYear), m_coordinateX(initX) {}
	int getYear() const { return m_year; }
	void setYear(int val) { m_year = val; }
	void setCoordinateX(float x) { m_coordinateX = x; }
	float getCoordinateX() const { return m_coordinateX; }
	void onclick(float x, float y);
};


class FilterDrawer : public MovingComponent
{
public:
	void draw() const override;
	void checkHovering(float x, float y);
	void slideOut();
	void slideIn();
	FilterDrawer()
		: MovingComponent(CANVAS_WIDTH * 0.5f, 70, 900, 210, Trio(0.2f, 0.2f, 0.2f), Trio(0.6f, 0.6f, 0.6f))
	{
		initWidgets();
	}
	enum drawer_state
	{
		STATE_EXPANDED,
		STATE_SLIDING_OUT,
		STATE_SLIDING_IN,
		STATE_COLLAPSED
	};
	drawer_state m_state = STATE_COLLAPSED;
	std::vector<GenreButton> m_genreButtons{};
	std::vector<SearchBox> m_searchBoxes{};
	std::vector<Slider> m_sliders{};
	ClearFiltersButton m_clearButton;
private:
	void initWidgets();
	float m_animation_timer = 0;
};
