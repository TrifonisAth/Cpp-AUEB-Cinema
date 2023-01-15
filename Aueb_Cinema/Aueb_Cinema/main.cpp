/*
*	2022-2023
	Athanasios Trifonis p3200298
	Christoforos Papapostolou p3150208
*/
#include "graphics.h"
#include "gui.h"
#include "defines.h"
#include "movieCollection.h"

void draw()
{
	Gui* gui = Gui::getInstance();
	gui->draw();
}

void update(float ms)
{
	Gui* gui = Gui::getInstance();
	gui->update();
}

int main()
{	
	graphics::createWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "It's Movie Time!");
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);
	graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);   
	graphics::setFont(FONT_PATH);
	graphics::preloadBitmaps(IMAGE_PATH);
    graphics::startMessageLoop();
	Gui::releaseInstance();
	graphics::destroyWindow();
    return 0;
}