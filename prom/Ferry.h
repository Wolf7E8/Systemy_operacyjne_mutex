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
	int capacity;		//³adownoœæ promu (ile samochodów mo¿e wjechaæ na pok³ad)
	vector<int> cars;	//wektor który przechowuje ID samochodów które wjecha³y na pok³ad

public:
	chrono::steady_clock::time_point lastCarTime;	//zmienna przechowuj¹ca czas jaki up³yn¹ od wjazdu ostatniego auta na prom
	const chrono::seconds maxWaitTime{ waitFerry };			//masymalny czas jaki prom czeka na samochód
	Ferry(int capacity);							//konstruktor
	void boardFerry(vector<int>& ramp);				//metoda odpowiedzialna za za³adunek aut na prom
	void sailAnimation(vector<int> carsOnBoard);	//animacja przeprawy przez rzekê
	void sail();									//metoda która odpowiada za przeprawê przez rzekê

};

