#ifndef _SECOND_MOUSE_CPP_
#define _SECOND_MOUSE_CPP_

void I_ShutdownMouse2()
{
	if(mouse2filehandle)
	{
		SetCommMask(mouse2filehandle, 0) ;

		EscapeCommFunction(mouse2filehandle, CLRDTR) ;
		EscapeCommFunction(mouse2filehandle, CLRRTS) ;
		PurgeComm(mouse2filehandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		CloseHandle(mouse2filehandle);

		mouse2filehandle = 0;
	}
}

int I_PoolMouse2()
{
	byte buffer[MOUSECOMBUFFERSIZE];
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;
	char       dx,dy;

	static int     bytenum;
	static byte    combytes[4];
	DWORD      i;

	ClearCommError(mouse2filehandle, &dwErrorFlags, &ComStat) ;
	dwLength = min(MOUSECOMBUFFERSIZE, ComStat.cbInQue) ;

	if(dwLength > 0)
	{
		if(!ReadFile(mouse2filehandle, buffer, dwLength, &dwLength, NULL))
		{
			alert(("Błąd odczytu na porcie szeregowym " + portszeregowy).c_str(), "Funkcja gry na drugiej myszcze nie będzie działać", NULL, "OK", NULL, 'T', 'N');
			blad_myszki = true;
			return 2;
		}

		// parse the mouse packets
		for(i = 0; i < dwLength; i++)
		{
			if((buffer[i] & 64) == 64) bytenum = 0;
			
			if(bytenum < 4) combytes[bytenum]=buffer[i];
			bytenum++;

			if(bytenum==1)
			{
				handlermouse2buttons &= ~3;
				handlermouse2buttons |= ((combytes[0] & (32+16)) >>4);
			}
			else
			if(bytenum==3)
			{
				dx = ((combytes[0] &  3) << 6) + combytes[1];
				dy = ((combytes[0] & 12) << 4) + combytes[2];
				handlermouse2x += dx * sensitivity;
				handlermouse2y += dy * sensitivity;
			}
			else
			if(bytenum==4) // fourth byte (logitech mouses)
			{
				if(buffer[i] & 32)
					handlermouse2buttons |= 4;
				else
					handlermouse2buttons &= ~4;
			}
		}
	}
	return 0;
}

void I_StartupMouse2()
{
    DCB dcb;
	
	if(mouse2filehandle)
		I_ShutdownMouse2();

    if(!mouse2filehandle)
    {
        // COM file handle
        mouse2filehandle = CreateFile( portszeregowy.c_str(), GENERIC_READ | GENERIC_WRITE,
                                       0,                     // exclusive access
                                       NULL,                  // no security attrs
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL, 
                                       NULL );
		
		//error handling
        if(mouse2filehandle == INVALID_HANDLE_VALUE)
        {
            int e = GetLastError();
            if(e == 5)
			{
				if(alert(("Nie można uruchomić portu szeregowego " + portszeregowy).c_str(), "Prawdopodobnie inne urządzenie już używa tego portu.", "Czy chcesz dowiedzieć się więcej na temat błędu?", "TAK", "NIE", 'T', 'N') == 1)
				{
					clear_to_color(screen, makecol(0,83,0));
					informacja_o_bledzie = load_bmp("blad.bmp",default_palette);
					masked_blit(informacja_o_bledzie, screen, 0,0, (rozdx - informacja_o_bledzie->w)/2, (rozdy - informacja_o_bledzie->h)/2, informacja_o_bledzie->w, informacja_o_bledzie->h);
					destroy_bitmap(informacja_o_bledzie);
					show_mouse(screen);
					while(1) if(key[KEY_ENTER]) break;
					show_mouse(NULL);
				}
			}
            else
			{
                if(alert(("Nie można uruchomić portu szeregowego " + portszeregowy).c_str(), "Prawdopodobnie myszka nie jest podłączona lub", "taki port nie istnieje.", "OK", NULL, 'T', 'N') == 1);
			}
			blad_myszki = true;
            mouse2filehandle = 0;
            return;
        }
    }

    
    // buffers
    SetupComm(mouse2filehandle, MOUSECOMBUFFERSIZE, MOUSECOMBUFFERSIZE);
    // purge buffers
    PurgeComm(mouse2filehandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
    // setup port to 1200 7N1
    dcb.DCBlength = sizeof(DCB);
    GetCommState(mouse2filehandle, &dcb);

    dcb.BaudRate = CBR_1200;
    dcb.ByteSize = 7;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fBinary = true;
    dcb.fParity = true;
	
    SetCommState(mouse2filehandle, &dcb);
}
#endif