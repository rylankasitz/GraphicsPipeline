// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "gui.h"

void GUI::cb_DBG_i(Fl_Button*, void*) {
  DBG_cb();
}
void GUI::cb_DBG(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_DBG_i(o,v);
}

void GUI::cb_Calculate_i(Fl_Button*, void*) {
  Calculate_cb();
}
void GUI::cb_Calculate(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_Calculate_i(o,v);
}

void GUI::cb_Demo_i(Fl_Button*, void*) {
  Demo_cb();
}
void GUI::cb_Demo(Fl_Button* o, void* v) {
  ((GUI*)(o->parent()->user_data()))->cb_Demo_i(o,v);
}
#include "scene.h"

GUI::GUI() {
  { uiw = new Fl_Double_Window(199, 197, "GUI");
    uiw->user_data((void*)(this));
    { Fl_Button* o = new Fl_Button(15, 125, 170, 40, "DBG");
      o->selection_color(FL_DARK_RED);
      o->callback((Fl_Callback*)cb_DBG);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(15, 70, 170, 40, "Calculate");
      o->selection_color(FL_DARK_RED);
      o->callback((Fl_Callback*)cb_Calculate);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(15, 15, 170, 40, "Demo");
      o->selection_color(FL_DARK_RED);
      o->callback((Fl_Callback*)cb_Demo);
    } // Fl_Button* o
    uiw->end();
  } // Fl_Double_Window* uiw
}

int main(int argc, char **argv) {
  scene = new Scene;
  return Fl::run();
}

void GUI::show() {
  uiw->show();
}

void GUI::DBG_cb() {
  scene->DBG();
}

void GUI::Calculate_cb() {
  scene->Calc();
}

void GUI::Demo_cb() {
  scene->Demo();
}
