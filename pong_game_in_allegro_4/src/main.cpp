#include <allegro.h>
#include <winalleg.h> //obsługa WINAPI dla drugiej myszy
#include <fstream>
#include <ctime>
#include <cmath>
#include <complex>
using namespace std;

//liczba zespolona doskonale odwzorowuje wektor w przestrzeni dwuwymiarowej
typedef complex<double> wektor;
void init();
void deinit();

#include "misc.h"
#include "second_mouse.h"
#include "pilka.h"
#include "okno_konfiguracji.h"

#include "okno_konfiguracji.cpp"
#include "second_mouse.cpp"
#include "pilka.cpp"
#include "misc.cpp"

int main()
{
	init();
	pilka kula;
	while(1)
	{
		//część logiczna programu
		while(speed_counter > 0)
		{
			if(key[KEY_F5])
			{
				clear_keybuf();  //losowo wciskane klawisze czasami uniemożliwiają wyświetlenie okna - to uniemożliwia występowanie problemu
				konf::ur = pauza;  //ustalamy poprzednio czy zostało zapauzowane
				pauza = true;  //robimy pauzę niezależnie od tego czy jest pauza czy nie,
				konf::handle_me(); //gdyż wyświetlanie okna nie blokuje działania reszty programu
			}
			if(key[KEY_P] and key_lock[0] == false) { pauza = !pauza; key_lock[0] = true; }
			if(!key[KEY_P] and key_lock[0] == true) key_lock[0] = false;
			if(key[KEY_F10])
			{
				konf::ur = pauza; 
				pauza = true;
				if(alert("Na pewno chcesz wyjść?", NULL, NULL, "TAK", "NIE", 't', 'n') == 1)
				{
					deinit();
					exit(0);
				}
			}
			if(!pauza)
			{
				if(key[KEY_R] and key_lock[1] == false) { kula.reset(); gracz_wynik[0] = gracz_wynik[1] = 0; key_lock[1] = true; }
				if(!key[KEY_R] and key_lock[1] == true) key_lock[1] = false;
				if(!blad_myszki) I_PoolMouse2();
				
				handle_controls();
				for(int i = 0; i < 2; i++)
				{
					if(ypol[i] < polwyspal) ypol[i] = polwyspal;
					if(ypol[i] > rozdy - 1 - polwyspal) ypol[i] = rozdy - 1 - polwyspal;
				}
				
				if(*ster < 0) *ster = 0; else if(*ster > rozdy) *ster = rozdy;
				kula.handle();	
			}
			speed_counter--;
		}
		
		//część graficzna programu
		//czyścimy ekran
		clear_to_color(bufor[obecny_bufor], tlo.zwroc_kolor());
		//przedział pośrodku
		rectfill(bufor[obecny_bufor], rozdx / 2 - 3, 0, rozdx / 2 + 3, rozdy, makecol(255-tlo.r, 255, 255-tlo.b));
		//piłka
		rotate_sprite( bufor[obecny_bufor], kula_img, kula.pozycja.x - szerkulki/2, kula.pozycja.y - szerkulki/2, itofix(kula.kat));
		//paletki
		draw_sprite_h_flip(bufor[obecny_bufor], paletka, xpol[0] - polszerpal, ypol[0] - polwyspal);
		draw_sprite(bufor[obecny_bufor], paletka, xpol[1] - polszerpal, ypol[1] - polwyspal);
		//wyniki
		textout_ex(bufor[obecny_bufor],czcionka_wynikow,tostr(gracz_wynik[1]).c_str(),300,30 ,makecol(255-tlo.r,255,255-tlo.b),-1);
		textout_ex(bufor[obecny_bufor],czcionka_wynikow,tostr(gracz_wynik[0]).c_str(),rozdx - 301,30 ,makecol(255-tlo.r,255,255-tlo.b),-1);
		//tekst "PAUZA"
		if(pauza) textout_centre_ex(bufor[obecny_bufor],czcionka_wynikow,"PAUZA",rozdx/2, rozdy/2 ,makecol(255,0,0),-1);
		//na ekran!
		show_video_bitmap(bufor[obecny_bufor]);
		obecny_bufor = (obecny_bufor+1)%2;
		
		if(konf::ur != 120) { pauza = konf::ur; konf::ur = 120; } //przywracamy poprzedni stan pauzy
	}
	deinit();
	return 0;
}
END_OF_MAIN()

void init()
{
	set_uformat(U_UTF8);
    get_uformat();
	
	bufor[0] = NULL;
	bufor[1] = NULL;
	
	conf.open("pong.cfg", ios::in);
	if(conf.good())
		load_settings();
	else
		save_settings();
	conf.close();
	
	if(rodz_ster[1] == 3 or rodz_ster[0] == 3) ster = &handlermouse2x; else ster = &handlermouse2y;
	allegro_init();
	
	int depth, res;
	depth = desktop_color_depth();
	if(depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, rozdx, rozdy, 0, 0);
	if(res != 0)
	{
		allegro_message(allegro_error);
		exit(-1);
	}
	
	install_timer();
	install_keyboard();
	install_mouse();
	enable_hardware_cursor();
	clear_to_color(screen, makecol(0,0,0));
	for(int i = 0; i < 25; i++) key_lock[i] = false;
	// ładowanie plików - pamiętaj by je zniszczyć potem
	paletka = load_bmp(paletka_sc.c_str(),default_palette);
	kula_img = load_bmp(kula_sc.c_str(),default_palette);
	czcionka_wynikow = load_font("font.pcx", default_palette, NULL);
	bufor[0] = create_video_bitmap(rozdx, rozdy);
    bufor[1] = create_video_bitmap(rozdx, rozdy);
	if(!bufor[0] or !bufor[1] or !paletka or !kula_img or !czcionka_wynikow) { alert("Błąd alokacji pamięci.", "Sprawdź czy masz wystarczająco dużo wolnej pamięci RAM", NULL, "Zamknij program", NULL, 'T', 'N'); exit(-1); }
	if(abs(rodz_ster[1]) == 3 or abs(rodz_ster[1]) == 4 or abs(rodz_ster[0]) == 3 or abs(rodz_ster[0]) == 4)
	{
		I_StartupMouse2();
		handlermouse2x = rozdy / 2;
		handlermouse2y = rozdy / 2;
	}
	else blad_myszki = true;
	LOCK_VARIABLE(speed_counter);
    LOCK_FUNCTION(zwiekszaj_licznik);
	install_int_ex(zwiekszaj_licznik, BPS_TO_TIMER(60));
	gui_bg_color = tlo.zwroc_kolor();
	gui_fg_color = makecol(255-tlo.r,255,255-tlo.b);
	//kopiowanie stringów C++ do null-terminated stringów C, których używa Allegro
	ustrncpy(konf::rozdx, tostr(rozdx).c_str(), 5);
	ustrncpy(konf::rozdy, tostr(rozdy).c_str(), 5);
	ustrncpy(konf::paletka_sc, tostr(paletka_sc).c_str(), MAX_PATH);
	ustrncpy(konf::kula_sc, tostr(kula_sc).c_str(), MAX_PATH);
	ustrncpy(konf::sel, portszeregowy.c_str(), 5);
}

void deinit()
{
	clear_keybuf();
	I_ShutdownMouse2();
	//uwalniamy zasoby
	destroy_bitmap( bufor[0] );
	destroy_bitmap( bufor[1] );
	destroy_bitmap( kula_img );
	destroy_font(czcionka_wynikow);
	destroy_bitmap( paletka );
}
