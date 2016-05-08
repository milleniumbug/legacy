#ifndef _MISC_H_
#define _MISC_H_

const int polwyspal = 45;  //połowa wysokości paletki
const int polszerpal = 12; //połowa szerokości paletki
const int szerkulki = 64;  //szerokość kulki
const int odl_od_bokow = 96; //odległość od boków ekranu - ustala pozycję paletek

BITMAP *bufor[2], *kula_img = NULL, *paletka = NULL, *informacja_o_bledzie;
FONT *czcionka_wynikow = NULL;
int rozdx = 1024, rozdy = 768; //rozdzielczość ekranu - domyślnie to 1024 x 768
short int obecny_bufor = 0, rodz_ster[2] = { 0, 0 }; // 0 - klawiatura; 1 - myszka oś X; 2 - myszka oś Y; 3 - druga myszka oś X; 4 - druga myszka oś Y; 5,6,7,8 - (zarezerwowane) Sztuczna Inteligencja o rosnącym poziomie trudności
int xpol[2] = { rozdx - odl_od_bokow, odl_od_bokow }; //polożenie paletki - współrzędna X gracza 0,1
int ypol[2] = { rozdy / 2, rozdy / 2 }; //polożenie paletki - współrzędna Y gracza 0,1
string kula_sc("kula.bmp"), paletka_sc("paletka.bmp");
int gracz_wynik[2] = { 0, 0 };
const double pi = 3.14159;
bool key_lock[25];         //blokady klawiszy - by funkcja wykonywała się jeden raz
bool pauza = false;

fstream conf;
volatile int speed_counter = 0;
void zwiekszaj_licznik();
int min(int a, int b);
int max(int a, int b);
bool save_settings();
template<typename x> string tostr(x n);
template<typename x> int toint(x n);
template<typename x, typename y> void konw(x konwertuj_z, y &konwertuj_do);
bool kolizja(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
void handle_controls();

class polozenie 
{
	public:
	int x;
	int y;
};

class kolor
{
	public:
	short int r;
	short int g;
	short int b;
	
	kolor(short x, short y, short z)
	{
		r = x;
		g = y;
		b = z;
	}
	
	int zwroc_kolor()
	{
		return makecol(r,g,b);
	}
};

kolor tlo(0,83,0);

#endif