#include "Ferry.h"
#include "string"

string vehicles;
extern int amoutnCarsOnHarbor;
extern int delayFerry;
extern mutex binarySemaphore;

Ferry::Ferry(int capacity) : capacity(capacity)
{
}

void Ferry::boardFerry(vector<int>& ramp)
{
	if (capacity != 0)
	{

		if (!ramp.empty())
		{
            int carID_display;   //zmienna do wy�wietlenia, kt�ry samoch�d wjecha� na ramp�
            binarySemaphore.lock();                  //zablokowanie dost�pu do sekcji krytycznej
            cars.push_back(ramp.front());   //za�adunek auta na prom
            carID_display = ramp.front();
			lastCarTime = std::chrono::steady_clock::now();     //zaktualizowanie czasu ostatniego za�adunku
            ramp.erase(ramp.begin());   //usuni�cie z rampy samochodu kt�ry ju� wjecha� na prom
            amoutnCarsOnHarbor = ramp.size();	//aktualizacja informacji o ilo�ci aut na rampie
            binarySemaphore.unlock();    //odblokowanie dost�pu do sekcji krytycznej

            //Dodanie animacji wjazdu na prom w czasie rzeczywistym
            const int riverWidth = 40;  // Szeroko�� rzeki, zwi�kszona dla lepszego wy�wietlania
            const int carsCount = this->cars.size();    // Liczba samochod�w
            std::string river(riverWidth, '=');  // Reprezentacja rzeki
            std::string ferryLabel = "PROM";   // Etykieta promu
            // std::string cars(carsCount, ' + ');    // Reprezentacja samochod�w
            string cars = "+(" + to_string(this->cars[0]) + ")";
            for (int i = 1; i < this->cars.size(); i++)
            {
                cars = cars + " +(" + to_string(this->cars[i]) + ")";
            }
            cars = "[ " + cars + " ]";          // Dodaj ramk� dooko�a samochod�w
            system("cls");
            cout << "Ilosc samochodow oczekujacych na zaladunek(na rampie w porcie): " << amoutnCarsOnHarbor << endl << endl;
            std::cout << river << std::endl;
            int padding;
            padding = (cars.length() - ferryLabel.length()) / 2;
            std::cout << std::string(padding, ' ') << ferryLabel << std::endl;
            // Rysuj samochody lub pusty prom
            std::cout << (cars) << std::endl;
            std::cout << river << std::endl;

            if (capacity == 5)
            {
                vehicles = "Pojazd " + std::to_string(carID_display) + " wjezdza na poklad \n";
            }
            else vehicles += "Pojazd " + std::to_string(carID_display) + " wjezdza na poklad \n";
            cout << vehicles;
            this_thread::sleep_for(chrono::milliseconds(200));  //op�nienie w�tku dla lepszej przejrzysto�ci programu
			capacity--;     //dekrementacja zmiennej odpowiedzialnej za pojemno�� promu
		}

		if (capacity == 0 || 
            chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - lastCarTime) >= maxWaitTime 
            && !cars.empty())
		{
			sail();
		}

	}
}


void Ferry::sail()
{
    if (cars.size() == 1)
    {
        cout << "Prom plynie przez rzeke z " << cars.size() << " autem" << endl;
    }
    else
    {
        cout << "prom plynie przez rzeke z " << cars.size() << " autami" << endl;
    }
	this_thread::sleep_for(chrono::seconds(1));     //wstrzymanie w�tku dla lepszej przejrzysto�ci
    sailAnimation(cars);                            //animacja
	cars.clear();                                   //roz�adunek promu
	capacity = 5;                                   //prom zn�w ma 5 wolnych miejsc
    vehicles = "";
}

void Ferry::sailAnimation(vector<int> carsOnBoard) {
    const int riverWidth = 40;  // Szeroko�� rzeki, zwi�kszona dla lepszego wy�wietlania
    const int carsCount = carsOnBoard.size();    // Liczba samochod�w
    std::string river(riverWidth, '=');  // Reprezentacja rzeki
    std::string ferryLabel = "PROM";   // Etykieta promu
    string cars = "+(" + to_string(carsOnBoard[0]) + ")";
    for (int i = 1; i < carsOnBoard.size(); i++)
    {
        cars = cars + " +(" + to_string(carsOnBoard[i]) + ")";
    }
    cars = "[ " + cars + " ]";          // Dodaj ramk� dooko�a samochod�w

    bool isReturning = false;  // Zmienna kontroluj�ca, czy prom jest w drodze powrotnej
    bool isSailing = true;
    int help = 0;

    while (isSailing) {
        int startPosition = isReturning ? riverWidth - (cars.length() / 2) : 0;
        int endPosition = isReturning ? 0 : riverWidth - (cars.length() / 2);
        int step = isReturning ? -1 : 1;

        for (int i = startPosition; isReturning ? i >= endPosition : i <= endPosition; i += step)
        {
            // Czy�� ekran
            system("cls"); // Dla system�w Windows, u�yj "clear" dla Unix/Linux

            cout << "Ilosc samochodow oczekujacych na zaladunek(na rampie w porcie): " << amoutnCarsOnHarbor << endl << endl;

            // Rysuj rzek�, prom i samochody
            std::cout << river << std::endl;
            // Oblicz i wy�wietl padding, aby wy�rodkowa� napis "PROM"
            int padding;
            if (!isReturning)
            {
                padding = (cars.length() - ferryLabel.length()) / 2;
            }
            else padding = 2;
            std::cout << std::string(i + padding, ' ') << ferryLabel << std::endl;
            // Rysuj samochody lub pusty prom
            std::cout << std::string(i, ' ') << (isReturning ? "[     ]" : cars) << std::endl;
            std::cout << river << std::endl;

            // Op�nienie symuluj�ce ruch
            std::this_thread::sleep_for(std::chrono::milliseconds(delayFerry));
        }
        help++;
        if (help >= 2)
        {
            isSailing = false;
        }

        // Zmiana kierunku podr�y
        isReturning = !isReturning;
    }
}
