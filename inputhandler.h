#pragma once

#include "worldview.h";
#include "scene.h"

#include <FL/Fl.H>

#define ROTATE_SENSITIVITY 0.02f
#define PAN_SENSITIVITY 0.04f
#define ZOOM_SENSITIVITY 2.0f

class InputHandler {
public:
	static void Instatiate(WorldView* _wv);
	static int event_handler(int event);
	static void SetEnabled(bool e);

private:

	InputHandler() {}

	static void handleKeyboard(int key);
	static void handleMouseDrag(int x, int y, int event);
	static void handleMouseScroll(int x, int y);
};
