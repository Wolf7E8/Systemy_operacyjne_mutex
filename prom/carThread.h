#pragma once
#include "Ferry.h"

class carThread
{
private:
	int ID;	//indywidualny identyfikator samochodu
	static int counter;	//licznik u¿ywany do nadawania IDc

public:
	carThread();
	int getID();
};