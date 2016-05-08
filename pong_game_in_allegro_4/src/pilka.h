#ifndef _PILKA_H_
#define _PILKA_H_

class pilka
{
	private:
	void odbij_pilke(int gracz);
	void trafiony(int gracz);
	
	public:	
	polozenie pozycja;
	wektor szybkosc;
	float modul_szybkosci;
	int kat; //kąt obrotu piłki - nie powiązane z liczbami zespolonymi
	
	void reset(); //ustawia piłkę na ekranie, jej szybkość i zeruje kąt
	pilka(); //konstruktor piłki - standardowo robi to samo co metoda reset.
	void handle(); //przemieszcza piłkę, uchrania przed przekroczeniem ekranu i wywołuje różne podprogramy zajmujące się
	//wykrywaniem kolizji
	
};

#endif