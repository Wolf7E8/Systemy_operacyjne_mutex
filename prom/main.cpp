#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include "carThread.h"

int amoutnCarsOnHarbor = 0;						//ilo�� samochod�w na rampie portu, oczekuj�cych na za�adunek
int waitFerry = 3;								//czas jaki prom oczekuje na wjazd nastepnego samochodu. Po tym czsie odp�ywa z t� ilo�ci� samochod�w jak� ma na pok�adzie.
int delayCars = 100;							//op�nienie z jakim losowane jest pojawienie si� samochodu (w milisekundach)
int delayFerry = 20;							//op�nienie z jakim prom si� przemieszcza. Im mniejsza warto�� tym prom kr�cej p�ynie
mutex binarySemaphore;



using namespace std;

void harbor(vector<int>& ramp);					//funkcja odpowiedzialna za pojawianie si� aut na rampie portu

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

	std::thread carsArrive(harbor,ref(ramp));	//inicjalizacja w�tku odpowiedzialnego za pojawianie si� samochod�w w porcie
	carsArrive.detach();						//w�tek pojawiania si� samochod�w w porcie dzia�a niezale�nie				

	cout << "Ilosc samochodow oczekujacych na zaladunek(na rampie w porcie): " << amoutnCarsOnHarbor << endl << endl;

	//pocz�tkowe rysowanie promu
	const int riverWidth = 40;  // Szeroko�� rzeki
	std::string river(riverWidth, '=');  // Reprezentacja rzeki
	std::string ferryLabel = "PROM";   // Etykieta promu
	std::cout << river << std::endl;
	int padding;
	padding = 1;
	std::cout << std::string(padding, ' ') << ferryLabel << std::endl;
	// Rysuj pusty prom
	std::cout << "[    ]" << std::endl;
	std::cout << river << std::endl;


	//g��wna p�tla programu
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
	// Ustawienie rozk�adu (tutaj: liczby ca�kowite od 1 do 100)
	uniform_int_distribution<int> distribution(1, 1000);

	mutex mutexCars;

	while (true)
	{
		for (int i = 5; i > 0; i--)
		{
			int random = distribution(generator);
			if (random % 2 == 0 && random % 3 == 0 && random % 10 == 0)
			{
				binarySemaphore.lock();			//zablokowanie dost�pu do rampy
				carThread car;						//stworzenie nowego auta
				ramp.push_back(car.getID());		//wjazd auta na ramp�
				amoutnCarsOnHarbor = ramp.size();	//aktualizacja informacji o ilo�ci aut na rampie
				binarySemaphore.unlock();					//odblokowanie dost�pu do rampy
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(delayCars));		//op�nienie potrzebne dla wi�kszej przejrzysto�ci
		}
	}
}