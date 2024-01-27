#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include "carThread.h"

int amoutnCarsOnHarbor = 0;						//iloœæ samochodów na rampie portu, oczekuj¹cych na za³adunek
int waitFerry = 3;								//czas jaki prom oczekuje na wjazd nastepnego samochodu. Po tym czsie odp³ywa z t¹ iloœci¹ samochodów jak¹ ma na pok³adzie.
int delayCars = 100;							//opóŸnienie z jakim losowane jest pojawienie siê samochodu (w milisekundach)
int delayFerry = 20;							//opóŸnienie z jakim prom siê przemieszcza. Im mniejsza wartoœæ tym prom krócej p³ynie
mutex binarySemaphore;



using namespace std;

void harbor(vector<int>& ramp);					//funkcja odpowiedzialna za pojawianie siê aut na rampie portu

int main()
{
	string decyzja;
	cout << "PROM I SAMOCHODY" << endl;
	cout << "Czy chcesz zmienic parametry symulacji? (tak/nie): ";
	cin >> decyzja;
	if (decyzja == "tak" || decyzja == "TAK" || decyzja == "t" || decyzja == "T")
	{
		cout << "Podaj maksymalny czas oczekiwania promu (w sekundach) na przyjazd samochodu (domyslnie 3): ";
		cin >> waitFerry;
		cout << "Podaj czas opoznienia (w milisekundach) w losowym pojawianiu sie samochodow (domyslnie 100): ";
		cin >> delayCars;
		cout << "Podaj czas opoznienia (w milisekundach) w plynieciu promu (im mniejsza wartosc tym prom szybciej plynie, domyslnie 20): ";
		cin >> delayFerry;
	}
	Ferry ferry(5);								//stworzenie obiektu prom
	vector<int> ramp;							//inicjalizacja rampy portu

	std::thread carsArrive(harbor,ref(ramp));	//inicjalizacja w¹tku odpowiedzialnego za pojawianie siê samochodów w porcie
	carsArrive.detach();						//w¹tek pojawiania siê samochodów w porcie dzia³a niezale¿nie				

	cout << "Ilosc samochodow oczekujacych na zaladunek(na rampie w porcie): " << amoutnCarsOnHarbor << endl << endl;

	//pocz¹tkowe rysowanie promu
	const int riverWidth = 40;  // Szerokoœæ rzeki
	std::string river(riverWidth, '=');  // Reprezentacja rzeki
	std::string ferryLabel = "PROM";   // Etykieta promu
	std::cout << river << std::endl;
	int padding;
	padding = 1;
	std::cout << std::string(padding, ' ') << ferryLabel << std::endl;
	// Rysuj pusty prom
	std::cout << "[    ]" << std::endl;
	std::cout << river << std::endl;


	//g³ówna pêtla programu
	while (true)			
	{
		ferry.boardFerry(ramp);
	}

	return 0;

}


void harbor(vector<int>& ramp)
{
	// Ustawienie ziarna generatora na podstawie czasu
	unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
	mt19937 generator(seed);
	// Ustawienie rozk³adu (tutaj: liczby ca³kowite od 1 do 100)
	uniform_int_distribution<int> distribution(1, 1000);

	mutex mutexCars;

	while (true)
	{
		for (int i = 5; i > 0; i--)
		{
			int random = distribution(generator);
			if (random % 2 == 0 && random % 3 == 0 && random % 10 == 0)
			{
				binarySemaphore.lock();			//zablokowanie dostêpu do rampy
				carThread car;						//stworzenie nowego auta
				ramp.push_back(car.getID());		//wjazd auta na rampê
				amoutnCarsOnHarbor = ramp.size();	//aktualizacja informacji o iloœci aut na rampie
				binarySemaphore.unlock();					//odblokowanie dostêpu do rampy
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(delayCars));		//opóŸnienie potrzebne dla wiêkszej przejrzystoœci
		}
	}
}