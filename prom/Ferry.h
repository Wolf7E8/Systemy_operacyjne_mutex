#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

extern int waitFerry;

using namespace std;

class Ferry
{
private:
	int capacity;		//�adowno�� promu (ile samochod�w mo�e wjecha� na pok�ad)
	vector<int> cars;	//wektor kt�ry przechowuje ID samochod�w kt�re wjecha�y na pok�ad

public:
	chrono::steady_clock::time_point lastCarTime;	//zmienna przechowuj�ca czas jaki up�yn� od wjazdu ostatniego auta na prom
	const chrono::seconds maxWaitTime{ waitFerry };			//masymalny czas jaki prom czeka na samoch�d
	Ferry(int capacity);							//konstruktor
	void boardFerry(vector<int>& ramp);				//metoda odpowiedzialna za za�adunek aut na prom
	void sailAnimation(vector<int> carsOnBoard);	//animacja przeprawy przez rzek�
	void sail();									//metoda kt�ra odpowiada za przepraw� przez rzek�

};

