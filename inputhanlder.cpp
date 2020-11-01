#include "stdafx.h"
#include "inputhandler.h"

#include <iostream>

using namespace std;

int ppx, ppy;
WorldView* wv;

void InputHandler::Instatiate(WorldView* _wv) {

	wv = _wv;
	ppx = 0; ppy = 0;
}

int InputHandler::event_handler(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		handleKeyboard(Fl::event_key());
		return 0;
	}
	case FL_MOVE: {
		int x = Fl::event_x(), y = Fl::event_y();
		//cerr << x << " " << y << "        \r";

		ppx = x; ppy = y;
		return 0;
	}
	case FL_DRAG: {
		int x = Fl::event_x(), y = Fl::event_y();
		int state = Fl::event_button();
		//cerr << x << " " << y << " " << state << "        \r";

		handleMouseDrag(x, y, state);

		ppx = x; ppy = y;
		return 0;
	}
	case FL_MOUSEWHEEL: {
		int x = Fl::event_dx(), y = Fl::event_dy();

		handleMouseScroll(x, y);

		return 0;
	}
	default:
		return 0;
	}
	return 0;
}

void InputHandler::handleKeyboard(int key) {

	switch (key) {
	case FL_Up: {
		wv->ppc->RotateAround(Vector::ZERO, Vector::XAXIS, 5);
		scene->Render();
		break;
	}
	case FL_Down: {
		wv->ppc->RotateAround(Vector::ZERO, Vector::XAXIS, -5);
		scene->Render();
		break;
	}
	case FL_Left: {
		wv->ppc->RotateAround(Vector::ZERO, Vector::YAXIS, 5);
		scene->Render();
		break;
	}
	case FL_Right: {
		wv->ppc->RotateAround(Vector::ZERO, Vector::YAXIS, -5);
		scene->Render();
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

void InputHandler::handleMouseDrag(int x, int y, int event) {

	float xd = ppx - x; float yd = ppy - y;

	switch (event)
	{
	case FL_LEFT_MOUSE: {
		wv->ppc->PanRight(xd * ROTATE_SENSITIVITY);
		wv->ppc->TiltUp(-yd * ROTATE_SENSITIVITY);
		scene->Render();
		break;
	}
	case FL_MIDDLE_MOUSE: {
		wv->ppc->TranslateRight(xd * PAN_SENSITIVITY);
		wv->ppc->TranslateUp(-yd * PAN_SENSITIVITY);
		scene->Render();
	}
	}
}

void InputHandler::handleMouseScroll(int x, int y) {

	wv->ppc->TranslateForward(-y * ZOOM_SENSITIVITY);
	scene->Render();
}