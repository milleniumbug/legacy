#ifndef _MISC_CPP_
#define _MISC_CPP_

void zwiekszaj_licznik()
{
	speed_counter++;
}
END_OF_FUNCTION(zwiekszaj_licznik)

int min(int a, int b)
{
	return (a > b ? b : a);
}

int max(int a, int b)
{
	return (a < b ? b : a);
}

bool save_settings()
{
	conf.open("pong.cfg", ios::out);
	conf << rozdx << "\n" << rozdy << "\n" << kula_sc << "\n"
	<< paletka_sc << "\n" << rodz_ster[1] << "\n" 
	<< rodz_ster[0] << "\n" << portszeregowy << "\n"
	<< tlo.r << "\n" << tlo.g << "\n" << tlo.b;
	return conf.good();
}

void load_settings()
{
	conf >> rozdx >> rozdy >> kula_sc >> paletka_sc >> rodz_ster[1] >> rodz_ster[0] >> portszeregowy;
	conf >> tlo.r >> tlo.g >> tlo.b;
	xpol[0] = rozdx - odl_od_bokow;
	ypol[0] = ypol[1] = rozdy / 2;
}

// magia wzorców - konwersja z dowolnego typu POD (+ te z zaprzyjaźnioną funkcją na ostream)
// na string przy pomocy "jednej" funkcji. Pascal i jego odmiany mogłyby o tym tylko marzyć.
template<typename x>
string tostr(x n)
{
	stringstream iss;
	iss << n;
	string i;
	iss >> i;
	return i;
}

template<typename x>
int toint(x n)
{
	stringstream iss;
	iss << n;
	int i;
	iss >> i;
	return i;
}

template<typename x, typename y>
void konw(x konwertuj_z, y &konwertuj_do)
{
	stringstream iss;
	iss << konwertuj_z;
	iss >> konwertuj_do;
}

bool kolizja(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
	if(ax2 < bx1 or ax1 > bx2 or ay1 > by2 or ay2 < by1) return false;
	return true;
}

void handle_controls()
{
	for(int i = 0; i < 2; i++)
	{
		switch(rodz_ster[i])
		{
			case 0:  
				if(key[KEY_W] and i) ypol[1]-=rozdx/64;
				if(key[KEY_S] and i) ypol[1]+=rozdx/64;
				if(key[KEY_UP] and !i) ypol[0]-=rozdx/64;
				if(key[KEY_DOWN] and !i) ypol[0]+=rozdx/64; 
			break;
			case 1:  ypol[i] = rozdy - mouse_x; break;
			case -1: ypol[i] = mouse_x; break;
			case 2:  ypol[i] = mouse_y; break;
			case -2: ypol[i] = rozdy - mouse_y; break;
			//UWAGA: fall-thru
			case 3:  
			case 4:  ypol[i] = rozdy - *ster; break;
			//UWAGA: fall-thru
			case -3: 
			case -4: ypol[i] = *ster; break;
		}
	}
}
				
#endif