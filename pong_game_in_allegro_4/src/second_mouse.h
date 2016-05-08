#ifndef _SECOND_MOUSE_H_
#define _SECOND_MOUSE_H_
//obsługa drugiej myszy pożyczona i przystosowana do potrzeb programu 
//od Doom Legacy zgodnie z licenją GNU GPL
#define MOUSECOMBUFFERSIZE 256
double sensitivity = 1.5; //czułość drugiej myszy
string portszeregowy("COM1"); //port szeregowy myszy
HANDLE mouse2filehandle = 0; //uchwyt myszy
//zmienne określające położenie myszy - ale nie na ekranie, co oznacza, że mogą osiągnąć wartości ujemne
int handlermouse2x, handlermouse2y, handlermouse2buttons;

// wskaźnik na zmienną która określa położenie paletki - w zależności od ustawień to handlermouse2x lub handlermouse2y
// "Program ma problemy przy ustawieniu myszy szeregowej jako sposób sterowania dwóch graczy. Ale po co to komu?" - readme.txt
int *ster = NULL;
bool blad_myszki = false;

void I_StartupMouse2();
int I_PoolMouse2();
void I_ShutdownMouse2();

#endif