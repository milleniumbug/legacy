#ifndef _PILKA_CPP_
#define _PILKA_CPP_

void pilka::odbij_pilke(int gracz)
{
	double kat_padania;
	kat_padania = atan2(pozycja.y - ypol[gracz],pozycja.x - xpol[gracz]);
	if(kolizja(pozycja.x - szerkulki/2, pozycja.y - szerkulki/2, pozycja.x + szerkulki/2, pozycja.y + szerkulki/2, xpol[gracz] - polszerpal, ypol[gracz] - polwyspal, xpol[gracz] + polszerpal, ypol[gracz] + polwyspal))
	{
		szybkosc = wektor(-cos(kat_padania)*modul_szybkosci, -sin(kat_padania)*modul_szybkosci);
		// poniższy warunek jest po to by piłka nie odbijała się pod złym kątem
		if(abs(szybkosc.real()) < rozdx/292.5) szybkosc = wektor((szybkosc.real() > 0 ? 1 : -1)*(rozdx/292.5), szybkosc.imag());
		modul_szybkosci += 0.1;
	}
}

void pilka::trafiony(int gracz)
{
	gracz_wynik[gracz]++;
	pozycja.x = rozdx / 2;
	pozycja.y = rozdy / 2;
	szybkosc = wektor((gracz == 1 ? 1 : -1)*2.5,0);
	modul_szybkosci = rozdx/85.3; 
	// 1024 : 85.3 w zaokrągleniu równe 12. Szybkość piłki jest zależna od długości stołu
	// by czas lotu z jednej krawędzi do drugiej był porównywalny na różnych rozdzielczościach
}

void pilka::reset()
{
	pozycja.x = rozdx / 2;
	pozycja.y = rozdy / 2;
	szybkosc = wektor(2.5 * ((rand()%2) ? -1 : 1),3);
	modul_szybkosci = rozdx/85.3;
	kat = 0;
}

pilka::pilka()
{
	reset();
}

void pilka::handle()
{
	pozycja.x -= szybkosc.real(); // -= to jest bug który wślizgnął się
	pozycja.y -= szybkosc.imag(); // do kodu i trzeba go naprawić (wartości dodatnie i ujemne
	                              // oznaczają zupełnie co innego dla wymiarów x i y)
								  //
	odbij_pilke(0);
	odbij_pilke(1);
	
	//sprawdzamy czy nie przekroczyło dołu ekranu
	if(pozycja.y > (rozdy - szerkulki/2)) { pozycja.y = rozdy - szerkulki/2; szybkosc = wektor(szybkosc.real(), -szybkosc.imag());}
	//sprawdzamy czy nie przekroczyło góry ekranu
	if(pozycja.y < szerkulki/2) { pozycja.y = szerkulki/2; szybkosc = wektor(szybkosc.real(), -szybkosc.imag());}
	//sprawdzamy czy nie przekroczyło prawej strony ekranu
	if(pozycja.x > (rozdx - szerkulki/2)) trafiony(1);
	//sprawdzamy czy nie przekroczyło lewej strony ekranu
	if(pozycja.x < szerkulki/2) trafiony(0);
	//zwiększamy kąt - kąt jest zależny od prędkości poziomej piłki.
	kat -= (1/szybkosc.real())*20;
}

#endif