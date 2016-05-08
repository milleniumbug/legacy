#ifndef _OKNO_KONFIGURACJI_H_
#define _OKNO_KONFIGURACJI_H_
#define MAX_PATH 260
#define ILOSC_BAJTOW_W_UNIKODZIE 4
namespace konf
{
	//zmienne okna
	char sel[5 * ILOSC_BAJTOW_W_UNIKODZIE];
	char rozdx[6 * ILOSC_BAJTOW_W_UNIKODZIE];
	char rozdy[6 * ILOSC_BAJTOW_W_UNIKODZIE];
	//najdłuższa możliwa ścieżka do pliku + zero terminujące razy ilość bajtów w standardzie UTF-32, nieco na wyrost 
	char temp[MAX_PATH*ILOSC_BAJTOW_W_UNIKODZIE+1];
	char kula_sc[MAX_PATH*ILOSC_BAJTOW_W_UNIKODZIE+1];
	char paletka_sc[MAX_PATH*ILOSC_BAJTOW_W_UNIKODZIE+1];
	char exec[MAX_PATH*ILOSC_BAJTOW_W_UNIKODZIE+1];
	int ur = 120;
	
	int przycisk_proc(int msg, DIALOG *d, int c);
	int sel_mouse();
	const char* wart_listbox(int index, int *list_size); //do użytku okna na zdobywanie po kolei stringów do listboksa
	int zwroc_wartosc_ster(int gracz); // wstawia do jednej zmiennej całkowitej ustawienia sterowania danego gracza
	void load_wartosc_ster(bool gracz); // wstawia ustawienia sterowania danego gracza do programu z jednej zmiennej całkowitej 
	void handle_me(); //pokaż okno główne programu
	int save(void); //zapisz ustawienia z okna
	int kula_handler(); // wywołanie okna wyboru bitmapy kuli
	int paletka_handler(); // wywołanie okna wyboru bitmapy paletki

	DIALOG okno[] =
	{
		{ d_shadow_box_proc,   0,   0,  ::rozdx-100,  ::rozdy-100,  0,  0,     0,     0,       0,    0,   NULL, NULL, NULL },//0
		
		{ d_text_proc,        30,  20,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"Pierwszy gracz ",        NULL, NULL  },//1
		{ d_radio_proc,      160,  20,  120,   19,   0,  0,    0,      0,       0,   0,    (void*)"Klawiatura",          NULL, NULL  },//gracz_1
		{ d_radio_proc,      320,  20,  120,   19,   0,  0,    0,      0,       0,   0,    (void*)"Mysz",              NULL, NULL  },//3
		{ d_radio_proc,      480,  20,  120,   19,   0,  0,    0,      0,       0,   0,    (void*)"Druga mysz",              NULL, NULL  },//4
		{ d_check_proc,      160,  60,  160,   20,   0,  0,    0,      0,       0,   0,    (void*)"Odwróć mysz",           NULL, NULL  },//5
		{ d_check_proc,      330,  60,  160,   20,   0,  0,    0,      0,       0,   0,    (void*)"Oś Y",           NULL, NULL  },//6
		
		{ d_text_proc,        30, 100,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"Drugi gracz ",      NULL, NULL  },//7
		{ d_radio_proc,      160, 100,  120,   19,   0,  0,    0,      0,       1,   0,    (void*)"Klawiatura",          NULL, NULL  },//gracz_2
		{ d_radio_proc,      320, 100,  120,   19,   0,  0,    0,      0,       1,   0,    (void*)"Mysz",              NULL, NULL  },//9
		{ d_radio_proc,      480, 100,  120,   19,   0,  0,    0,      0,       1,   0,    (void*)"Druga mysz",              NULL, NULL  },//10
		{ d_check_proc,      160, 140,  160,   20,   0,  0,    0,      0,       0,   0,    (void*)"Odwróć mysz",           NULL, NULL  },//11
		{ d_check_proc,      330, 140,  160,   20,   0,  0,    0,      0,       0,   0,    (void*)"Oś Y",           NULL, NULL  },//12
		
		{ d_text_proc,       400, 200,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"Port myszy szeregowej ",        NULL, NULL  },//13
		{ d_list_proc,       400, 220,  160,   50,   0,  0,    0,      0,       0,   0,    (void*)wart_listbox, sel,  NULL  },//lista_portow
		
		{ d_text_proc,        20, 200,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"Rozdzielczość ekranu ",        NULL, NULL  },//15
		{ d_text_proc,        20, 240,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"UWAGA: Zmiana rozdzielczości ",        NULL, NULL  },//16
		{ d_text_proc,        20, 250,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"wymaga restartu programu ",        NULL, NULL  },//17
		{ d_edit_proc,        20, 220,   65,    8,   0,  0,    0,      0,      5,   0,    rozdx,             NULL, NULL  },//18
		{ d_text_proc,        90, 220,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"x",      NULL, NULL  },//19
		{ d_edit_proc,       100, 220,   65,    8,   0,  0,    0,      0,      5,   0,    rozdy,             NULL, NULL  },//20
		
		{ przycisk_proc,    160, 390,   160,   20,   0,  0,  's', D_EXIT,       0,   0,   (void*)"Zapisz",                NULL, (void *)save  },//21
		{ przycisk_proc,    330, 390,   160,   20,   0,  0,  'q', D_CLOSE,      0,   0,   (void*)"Wyjdź",                NULL, NULL  },//22
		{ przycisk_proc,    10,  280,   250,   20,   0,  0,   0, D_EXIT,       0,   0,   (void*)"Zmień wygląd paletki",                NULL, (void *)paletka_handler  },//23
		{ przycisk_proc,    300, 280,   250,   20,   0,  0,   0, D_EXIT,      0,   0,   (void*)"Zmień wygląd kulki",                NULL, (void *)kula_handler  },//24
		{ d_text_proc,        10, 305,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"Obrazki palet są akceptowane wyłącznie z katalogu programu ",        NULL, NULL  },//25
		
		{ d_slider_proc,     155, 325,  300,   12,   0,  0,    0,      0,     255,   0,    NULL,                   NULL, NULL  }, //suwaki_koloru
		{ d_slider_proc,     155, 325+13,  300,   12,   0,  0,    0,      0,     255,   0,    NULL,                   NULL, NULL  }, //27
		{ d_slider_proc,     155, 325+26,  300,   12,   0,  0,    0,      0,     255,   0,    NULL,                   NULL, NULL  }, //28
		{ d_text_proc,       20, 325,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"Kolor tła - RGB ",        NULL, NULL  },//29
		
		{ d_text_proc,        10, ::rozdy-120,    0,    0,   0,  0,    0,      0,       0,   0,    (void*)"",        NULL, NULL  },//autor
		
		{ d_yield_proc,        0,   0,    0,    0,   0,  0,    0,      0,       0,   0,    NULL,                   NULL, NULL  },//31
		{ NULL,                0,   0,    0,    0,   0,  0,    0,      0,       0,   0,    NULL,                   NULL, NULL  }//32
	};
}

#endif