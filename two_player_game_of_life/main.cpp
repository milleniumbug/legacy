#include <string>
#include <vector>

#include <cstdio>
#include <cstdarg>
bool format(std::string &str, const char* format, ...)
{
	va_list ap;
	if(str.empty())
		str.resize(2);
	
	va_start(ap, format);
	std::string::size_type liczba = vsnprintf(&str[0], 0, format, ap);
	va_end(ap);
	
	str.resize(liczba+1);
	
	va_start(ap, format);
	vsnprintf(&str[0], str.size(), format, ap);
	va_end(ap);
	return true;
}

#include "class_life.cpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Positioner.H>

const size_t max_size = 120;
const size_t granica = 2;
const size_t max_visible_size = 50;
GameOfLife gra(max_size);
#define ACC(i,j) (b_arr[(i)*max_visible_size+(j)])

Fl_Light_Button* b_arr[max_visible_size*max_visible_size];
Fl_Multiline_Output* status_tekst = NULL;
Fl_Scrollbar* pionowy_scrollbar = NULL;
Fl_Scrollbar* poziomy_scrollbar = NULL;
Fl_Positioner* minimapa = NULL;
Fl_Button* reset_button = NULL;
Fl_Button* mode_button = NULL;

unsigned int offsetx = (max_size/2)-(max_visible_size/2), offsety = (max_size/2)-(max_visible_size/2);
int countdown_value;
std::string status_tekst_bufor;

int editing_mode = 1;
const char* mode_names[] = { "AKCJA!!!", "Gracz 1", "Gracz 2" };

void redraw();
void scrollbar_callback(Fl_Widget* wid, void* s);
void mode_change(Fl_Widget* wid, void* s);
void countdown_callback(void* s);
void set_alive(Fl_Widget* wid, long tag);
void timer_callback(void* s);

void redraw()
{
	for(size_t i = 0; i < max_visible_size; i++)
	{
		for(size_t j = 0; j < max_visible_size; j++)
		{
			if(editing_mode and j+offsety >= (max_size/2)-granica and j+offsety <= (max_size/2)+granica)
			{
				ACC(i,j)->selection_color(FL_BLACK);
				ACC(i,j)->value(1);
				continue;
			}
			if(editing_mode == 1 and j+offsety >= (max_size/2)-granica)
			{
				ACC(i,j)->selection_color(FL_YELLOW);
				ACC(i,j)->value(0);
				continue;
			}
			if(editing_mode == 2 and j+offsety <= (max_size/2)+granica)
			{
				ACC(i,j)->selection_color(FL_YELLOW);
				ACC(i,j)->value(0);
				continue;
			}
			if(gra.at(i+offsetx,j+offsety).is_alive())
			{
				short int color = gra.at(i+offsetx,j+offsety).get_color();
				ACC(i,j)->selection_color(color ? FL_RED : FL_BLUE);
				ACC(i,j)->value(1);
			}
			else
			{
				ACC(i,j)->selection_color(FL_YELLOW);
				ACC(i,j)->value(0);
			}
		}
	}
}

void scrollbar_callback(Fl_Widget* wid, void* s)
{
	Fl_Scrollbar* w = static_cast<Fl_Scrollbar*>(wid);
	unsigned int* val = static_cast<unsigned int*>(s);
	*val = w->value();
	minimapa->value(offsetx, offsety);
	redraw();
}

void reset_callback(Fl_Widget* wid, void* s)
{
	mode_button->activate();
	editing_mode = 1;
	mode_button->label(mode_names[editing_mode]);
	gra.reset();
	status_tekst->value("");
	redraw();
}

void minimapa_callback(Fl_Widget* wid, void* s)
{
	Fl_Positioner* w = static_cast<Fl_Positioner*>(wid);
	offsetx = w->xvalue();
	poziomy_scrollbar->value(offsetx);
	offsety = w->yvalue();
	pionowy_scrollbar->value(offsety);
	redraw();
}

void mode_change(Fl_Widget* wid, void* s)
{
	Fl_Button* w = static_cast<Fl_Button*>(wid);
	editing_mode = (editing_mode+1)%3;
	if(editing_mode == 0)
	{
		w->deactivate();
		countdown_value = 3;
		status_tekst_bufor = "";
		Fl::add_timeout(0.01, countdown_callback);
	}
	status_tekst->value("");
	w->label(mode_names[editing_mode]);
	redraw();
}

void countdown_callback(void* s)
{
	format(status_tekst_bufor, "%s%d\n", &status_tekst_bufor[0], countdown_value);
	status_tekst->value(&status_tekst_bufor[0]);
	if(countdown_value != 0)
		Fl::repeat_timeout(1.0, countdown_callback);
	else
		Fl::add_timeout(0.01, timer_callback);
	countdown_value--;
}

void set_alive(Fl_Widget* wid, long tag)
{
	if(!editing_mode) return;
	//translate tag into (x,y) GameOfLife coordinates
	size_t x = tag / max_visible_size + offsetx;
	size_t y = tag % max_visible_size + offsety;
	if(editing_mode == 1 and y >= (max_size/2)-granica) return;
	if(editing_mode == 2 and y <= (max_size/2)+granica) return;
	gra.at(x,y) = Cell(editing_mode-1, !gra.at(x,y).is_alive());
	if(editing_mode != 0)
	{
		format(status_tekst_bufor, "Masz teraz %d komórek.", gra.ilosc_komorek_gracza(editing_mode-1));
		status_tekst->value(&status_tekst_bufor[0]);
	}
	redraw();
}

void timer_callback(void* s)
{
	if(!editing_mode)
	{
		redraw();
		format(status_tekst_bufor, "Gracz 1\n- liczba komórek: %d\nGracz 2\n- liczba komórek:  %d", gra.ilosc_komorek_gracza(0), gra.ilosc_komorek_gracza(1));
		status_tekst->value(&status_tekst_bufor[0]);
		gra.refresh();
		Fl::repeat_timeout(0.05, timer_callback);
	}
}

int main(int argc, char* argv[])
{
	Fl_Window window(800,600, "Gra w życie");
	for(size_t j = 0; j < max_visible_size; j++)
		for(size_t i = 0; i < max_visible_size; i++)
		{
			ACC(i,j) = new Fl_Light_Button(5+i*11, 5+j*11, 10, 10);
			ACC(i,j)->callback(set_alive, i*max_visible_size+j);
		}

	pionowy_scrollbar = new Fl_Scrollbar(max_visible_size*11+10, 5, 32, 550);
	pionowy_scrollbar->value(offsety, 1, 0, max_size-max_visible_size);
	pionowy_scrollbar->type(FL_VERTICAL);
	pionowy_scrollbar->linesize(4);
	pionowy_scrollbar->callback(scrollbar_callback, &offsety);

	poziomy_scrollbar = new Fl_Scrollbar(5, max_visible_size*11+10, 550, 32);
	poziomy_scrollbar->value(offsetx, 1, 0, max_size-max_visible_size);
	poziomy_scrollbar->type(FL_HORIZONTAL);
	poziomy_scrollbar->linesize(4);
	poziomy_scrollbar->callback(scrollbar_callback, &offsetx);

	mode_button = new Fl_Button(max_visible_size*11+60, 5, 100, 32, mode_names[editing_mode]);
	mode_button->callback(mode_change, NULL);
	//mode_button->take_focus();

	reset_button = new Fl_Button(max_visible_size*11+60, 5+32+1, 100, 32, "RESET");
	reset_button->callback(reset_callback, NULL);

	minimapa = new Fl_Positioner(max_visible_size*11+60, 400, 150, 150);
	minimapa->xbounds(0, max_size-max_visible_size);
	minimapa->ybounds(0, max_size-max_visible_size);
	minimapa->xstep(2);
	minimapa->ystep(2);
	minimapa->value(offsetx, offsety);
	minimapa->callback(minimapa_callback, NULL);

	status_tekst = new Fl_Multiline_Output(max_visible_size*11+60, 5+32+32+5, 170, 300, &status_tekst_bufor[0]);

	window.show(argc, argv);
	redraw();
	Fl::run();
	for(size_t j = 0; j < max_visible_size; j++)
		for(size_t i = 0; i < max_visible_size; i++)
			delete ACC(i,j);
	delete status_tekst;
	delete minimapa;
	delete reset_button;
	delete mode_button;
	delete poziomy_scrollbar;
	delete pionowy_scrollbar;
}
