#ifndef _OKNO_KONFIGURACJI_CPP_
#define _OKNO_KONFIGURACJI_CPP_

//w przypadku jakichkolwiek błędów w programie najpierw proszę sprawdzić czy poniższe wartości odpowiadają pozycji w tablicy konf::okno[]
//jeśli nie, poprawić
#define lista_portow 14
#define gracz_1 8
#define gracz_2 2
#define suwaki_koloru 26
#define autor 30
namespace konf
{
	
	int przycisk_proc(int msg, DIALOG *d, int c)
	{
		int ret = d_button_proc(msg, d, c);
		if(ret == D_CLOSE and d->dp3)
			return ((int (*)(void))d->dp3)();
		return ret;
	}
	
	int sel_mouse()
	{
		return toint(portszeregowy[3])-1;
	}
	
	const char* wart_listbox(int index, int *list_size)
	{
		const char* strings[] =
		{
			"COM1", "COM2", "COM3", "COM4"
		};

		if(index < 0)
		{
			*list_size = 4;
			return NULL;
		}
		else
		{
			return strings[index];
		}
	}
	
	int kula_handler()
	{
		file_select_ex("Wybierz plik *.bmp z grafiką kuli. Zalecany rozmiar: 64x64", temp, "PCX;BMP", MAX_PATH, 0, 0);
		if(exists(get_filename(temp))) ustrncpy(kula_sc, get_filename(temp), MAX_PATH);
		return D_O_K;
	}
	
	int paletka_handler()
	{
		file_select_ex("Wybierz plik *.bmp z grafiką paletki. Zalecany rozmiar: 25x91", temp, "PCX;BMP", MAX_PATH, 0, 0);
		if(exists(get_filename(temp))) ustrncpy(paletka_sc, get_filename(temp), MAX_PATH);
		return D_O_K;
	}
	
	int zwroc_wartosc_ster(int gracz)
	{
		int temp = 0;
		gracz = (gracz ? gracz_2 : gracz_1);
		if(okno[gracz].flags & D_SELECTED) temp = 0;
		if(okno[gracz+1].flags & D_SELECTED) temp = 1;
		if(okno[gracz+2].flags & D_SELECTED) temp = 3;
		if(temp and okno[gracz+4].flags & D_SELECTED) temp++;
		if(okno[gracz+3].flags & D_SELECTED) temp = -temp;
		return temp;
	}
	
	void load_wartosc_ster(bool gracz)
	{
		int wart_gr = (gracz ? gracz_2 : gracz_1);
		if(rodz_ster[gracz] == 0) okno[wart_gr].flags |= D_SELECTED;
		if(abs(rodz_ster[gracz]) == 1 or abs(rodz_ster[gracz]) == 2) okno[wart_gr+1].flags |= D_SELECTED;
		if(abs(rodz_ster[gracz]) == 3 or abs(rodz_ster[gracz]) == 4) okno[wart_gr+2].flags |= D_SELECTED;
		if(abs(rodz_ster[gracz]) == 2 or abs(rodz_ster[gracz]) == 4) okno[wart_gr+4].flags |= D_SELECTED;
		if(rodz_ster[gracz] < 0) okno[wart_gr+3].flags |= D_SELECTED;
	}
	
	int save(void)
	{
		::rozdx = toint(rozdx);
		::rozdy = toint(rozdy);
		::kula_sc = tostr(kula_sc);
		::paletka_sc = tostr(paletka_sc);
		portszeregowy = wart_listbox(konf::okno[lista_portow].d1, NULL);
		rodz_ster[0] = zwroc_wartosc_ster(0);
		rodz_ster[1] = zwroc_wartosc_ster(1);
		tlo.r = okno[suwaki_koloru].d2;
		tlo.g = okno[suwaki_koloru+1].d2;
		tlo.b = okno[suwaki_koloru+2].d2;
		conf.open("pong.cfg", ios::trunc);
		save_settings();
		return D_CLOSE;
	}
	
	void handle_me()
	{
		gui_bg_color = tlo.zwroc_kolor();
		gui_fg_color = makecol(255-tlo.r,255,255-tlo.b);
		set_dialog_color(okno, gui_fg_color, gui_bg_color);
		okno[lista_portow].d1 = sel_mouse();
		okno[0].w = ::rozdx-100;
		okno[0].h = ::rozdy-100;
		okno[autor].h = ::rozdy-120;
		load_wartosc_ster(0);
		load_wartosc_ster(1);
		okno[suwaki_koloru].d2 = tlo.r;
		okno[suwaki_koloru+1].d2 = tlo.g;
		okno[suwaki_koloru+2].d2 = tlo.b;
		position_dialog(konf::okno, 50, 50);
		do_dialog(konf::okno, -1);
	}
}
#endif