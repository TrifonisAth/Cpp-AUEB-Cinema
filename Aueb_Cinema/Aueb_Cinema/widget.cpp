#include "widget.h"
// initialize static variables in Widget class.
int Widget::m_next_uid = 1;
int Widget::m_focus = 0;
float MovingComponent::m_offsetY = -140;

bool Widget::contains(float x, float y) const
{
	return m_bounds.contains(x, y);
}

void Button::activate()
{
	setActive(!isActivated());
}

void GenreButton::draw() const
{
	graphics::Brush br;
	Trio color = isActivated() ? getColorHighlight() : getColor();
	SETCOLOR(br.fill_color, color.x, color.y, color.z);
	graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), getWidth(), getHeight(), br);
	SETCOLOR(br.fill_color, 0, 0, 0);
	graphics::drawText(getCenterX() - 30, getCenterY() + getOffsetY() + 5, TEXT_SMALL, getLabel(), br);
}

void GenreButton::onclick()
{
	activate();
}

void SearchBox::draw() const
{
	graphics::Brush br;
	Trio col;
	if (isFocused())
	{
		col = getColorHighlight();
		SETCOLOR(br.fill_color, col.x, col.y, col.z);
		SETCOLOR(br.outline_color, 0, 0, 0);
		graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), getWidth() * 1.02f, getHeight() * 1.25f, br);
		graphics::drawText(getCenterX() - 186 - 4 * getPrompt().length(), getCenterY() + 5 + getOffsetY(), TEXT_SMALL, getPrompt() + ":", br);
		SETCOLOR(br.fill_color, 0, 0, 0);
		graphics::drawText(getCenterX() - getWidth() / 2 + 20, getCenterY() + 5 + getOffsetY(), TEXT_SMALL + 1, getText(), br);
	}
	else
	{
		SETCOLOR(br.fill_color, 1, 1, 1);
		graphics::drawText(getCenterX() - 186 - 4 * getPrompt().length(), getCenterY() + 5 + getOffsetY(), TEXT_SMALL, getPrompt() + ":", br);
	}
	col = getColor();
	SETCOLOR(br.fill_color, col.x, col.y, col.z);
	graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), getWidth(), getHeight(), br);
	SETCOLOR(br.fill_color, 0, 0, 0);
	graphics::drawText(getCenterX() - getWidth() / 2 + 20, getCenterY() + 5 + getOffsetY(), TEXT_SMALL + 1, getText(), br);
}

void SearchBox::onfocus(char c)
{
	if (c == 0) return;
	if (c == 8)
	{
		if (m_text.length() > 0)
		{
			// play delete sound.
			m_text.pop_back();
			graphics::playSound(AUDIO_PATH + std::string("delete.wav"), 1.0f);
		}
	}
	else if (m_text.length() <= 35)
	{
		// play type sound.
		m_text += c;
		if (c == ' ') graphics::playSound(AUDIO_PATH + std::string("spacebar.wav"), 1.0f);
		else graphics::playSound(AUDIO_PATH + std::string("typing.wav"), 1.0f);
	}
	else return;
}

void MovieCard::draw() const
{
	graphics::Brush br;
	graphics::Brush br1;
	SETCOLOR(br1.fill_color, 0.7f, 0.4f, 0.1f);
	std::string path = IMAGE_PATH + getFilm().getPath() + ".png";
	br.texture = path;
	br.outline_opacity = 0.0f;
	graphics::drawRect(getCenterX(), getCenterY(), getWidth(), getHeight(), br);
	if (isFocused())
	{	
		graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.21f, TEXT_BIG, getFilm().getTitle(), br);
		graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.21f + 30, TEXT_MEDIUM, std::to_string(getFilm().getYear()), br1);
		graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.21f + 60, TEXT_MEDIUM, "Director:    ", br1);
		graphics::drawText(CANVAS_WIDTH * 0.4f + 80, CANVAS_HEIGHT * 0.21f + 60, TEXT_MEDIUM, getFilm().getDirector(), br);
		graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.21f + 90, TEXT_MEDIUM, "Actors: ", br1);
		for (int i = 0; i < getFilm().getActors().size(); i++)
		{
			graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.21f + 120 + i * 25, TEXT_MEDIUM, getFilm().getActors()[i], br);
		}
		graphics::drawText(CANVAS_WIDTH * 0.4f, CANVAS_HEIGHT * 0.21f + 210, TEXT_MEDIUM, "Plot:", br1);
		printParagraph(getFilm().getDescription());
		// Image changing here.
		if (getCounter() != 0)
		{
			path = IMAGE_PATH + getFilm().getPath() + std::to_string(getCounter()) + ".png";
		}
		
		br.texture = path;
		br.fill_opacity = getTimer() > 2 ? (3 - getTimer()) : 1;
		graphics::drawRect(getButton().getCenterX(), getButton().getCenterY(), getButton().getWidth(), getButton().getHeight(), br);
	}
}

void MovieCard::onhover()
{	
	setFocus();
}

void MovieCard::onclick()
{
}

void ClearFiltersButton::onclick()
{	
	setActive(true);
}

void ClearFiltersButton::draw() const
{
	graphics::Brush br;
	SETCOLOR(br.fill_color, getColor().x, getColor().y, getColor().z);
	SETCOLOR(br.outline_color, 0, 0, 0);
	graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), getWidth(), getHeight(), br);
	SETCOLOR(br.fill_color, getColorHighlight().x, getColorHighlight().y, getColorHighlight().z);
	graphics::drawText(getCenterX() - 22, getCenterY() + getOffsetY() + 5, TEXT_MEDIUM, getLabel(), br);
}

void FilterDrawer::draw() const
{
	graphics::Brush br;
	SETCOLOR(br.fill_color, getColor().x, getColor().y, getColor().z);
	SETCOLOR(br.outline_color, RED.x, RED.y, RED.z);
	br.outline_width = 3.0f;
	br.fill_opacity = 0.3f;
	graphics::drawRect(getCenterX(), getCenterY() + getOffsetY(), getWidth(), getHeight(), br);
	if (!isActivated())
	{
		graphics::Brush br1;
		int counter = (int) graphics::getGlobalTime() / 20 % 40;
		br1.outline_opacity = 0.0f;
		br1.texture = IMAGE_PATH + std::string("arrow.png");
		graphics::setOrientation(90.0f);
		graphics::drawRect(getCenterX(), CANVAS_HEIGHT * 0.07f - counter, 30, 30, br1);
		graphics::resetPose();
	}
	// draw clear filters button.
	m_clearButton.draw();
	// draw search boxes.
	for (const SearchBox& sb : m_searchBoxes)
	{
		sb.draw();
	}
	// draw genre buttons.
	for (const GenreButton& gb : m_genreButtons)
	{
		gb.draw();
	}
	// draw sliders.
	for (const Slider& s : m_sliders)
	{
		s.draw();
	}
}

void FilterDrawer::checkHovering(float x, float y)
{

	if (isActivated())
	{
		if (!(x >= getCenterX() - getWidth() / 2 && x <= getCenterX() + getWidth() / 2 && y >= getCenterY() - getHeight() / 2 && y <= getCenterY() + getHeight() / 2) || m_state == STATE_SLIDING_IN)
		{
			slideIn();
		}
	}
	else
	{
		if ((x >= getCenterX() - getWidth() / 2 && x <= getCenterX() + getWidth() / 2 && y <= getCenterY() + getHeight() / 3 && y > 2) || m_state == STATE_SLIDING_OUT)
		{
			slideOut();
		}
	}
}

void FilterDrawer::slideOut()
{
	m_state = STATE_SLIDING_OUT;
	m_animation_timer += graphics::getDeltaTime();
	float coord = getOffsetY() + m_animation_timer / 10;
	if (coord >= 40)
	{
		coord = 40;
		setActive(true);
		m_animation_timer = 0;
		m_state = STATE_EXPANDED;
	}
	setOffsetY(coord);
}

void FilterDrawer::slideIn()
{
	m_state = STATE_SLIDING_IN;
	m_animation_timer += graphics::getDeltaTime();
	float coord = getOffsetY() - m_animation_timer / 10;
	if (coord <= -140)
	{
		coord = -140;
		setActive(false);
		m_animation_timer = 0;
		m_state = STATE_COLLAPSED;
	}
	setOffsetY(coord);
}