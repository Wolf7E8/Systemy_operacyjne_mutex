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
            int carID_display;   //zmienna do wyœwietlenia, który samochód wjecha³ na rampê
            binarySemaphore.lock();                  //zablokowanie dostêpu do sekcji krytycznej
            cars.push_back(ramp.front());   //za³adunek auta na prom
            carID_display = ramp.front();
			lastCarTime = std::chrono::steady_clock::now();     //zaktualizowanie czasu ostatniego za³adunku
            ramp.erase(ramp.begin());   //usuniêcie z rampy samochodu który ju¿ wjecha³ na prom
            amoutnCarsOnHarbor = ramp.size();	//aktualizacja informacji o iloœci aut na rampie
            binarySemaphore.unlock();    //odblokowanie dostêpu do sekcji krytycznej

            //Dodanie animacji wjazdu na prom w czasie rzeczywistym
            const int riverWidth = 40;  // Szerokoœæ rzeki, zwiêkszona dla lepszego wyœwietlania
            const int carsCount = this->cars.size();    // Liczba samochodów
            std::string river(riverWidth, '=');  // Reprezentacja rzeki
            std::string ferryLabel = "PROM";   // Etykieta promu
            // std::string cars(carsCount, ' + ');    // Reprezentacja samochodów
            string cars = "+(" + to_string(this->cars[0]) + ")";
            for (int i = 1; i < this->cars.size(); i++)
            {
                cars = cars + " +(" + to_string(this->cars[i]) + ")";
            }
            cars = "[ " + cars + " ]";          // Dodaj ramkê dooko³a samochodów
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
            this_thread::sleep_for(chrono::milliseconds(200));  //opóŸnienie w¹tku dla lepszej przejrzystoœci programu
			capacity--;     //dekrementacja zmiennej odpowiedzialnej za pojemnoœæ promu
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
	this_thread::sleep_for(chrono::seconds(1));     //wstrzymanie w¹tku dla lepszej przejrzystoœci
    sailAnimation(cars);                            //animacja
	cars.clear();                                   //roz³adunek promu
	capacity = 5;                                   //prom znów ma 5 wolnych miejsc
    vehicles = "";
}

void Ferry::sailAnimation(vector<int> carsOnBoard) {
    const int riverWidth = 40;  // Szerokoœæ rzeki, zwiêkszona dla lepszego wyœwietlania
    const int carsCount = carsOnBoard.size();    // Liczba samochodów
    std::string river(riverWidth, '=');  // Reprezentacja rzeki
    std::string ferryLabel = "PROM";   // Etykieta promu
    string cars = "+(" + to_string(carsOnBoard[0]) + ")";
    for (int i = 1; i < carsOnBoard.size(); i++)
    {
        cars = cars + " +(" + to_string(carsOnBoard[i]) + ")";
    }
    cars = "[ " + cars + " ]";          // Dodaj ramkê dooko³a samochodów

    bool isReturning = false;  // Zmienna kontroluj¹ca, czy prom jest w drodze powrotnej
    bool isSailing = true;
    int help = 0;

    while (isSailing) {
        int startPosition = isReturning ? riverWidth - (cars.length() / 2) : 0;
        int endPosition = isReturning ? 0 : riverWidth - (cars.length() / 2);
        int step = isReturning ? -1 : 1;

        for (int i = startPosition; isReturning ? i >= endPosition : i <= endPosition; i += step)
        {
            // Czyœæ ekran
            system("cls"); // Dla systemów Windows, u¿yj "clear" dla Unix/Linux

            cout << "Ilosc samochodow oczekujacych na zaladunek(na rampie w porcie): " << amoutnCarsOnHarbor << endl << endl;

            // Rysuj rzekê, prom i samochody
            std::cout << river << std::endl;
            // Oblicz i wyœwietl padding, aby wyœrodkowaæ napis "PROM"
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

            // OpóŸnienie symuluj¹ce ruch
            std::this_thread::sleep_for(std::chrono::milliseconds(delayFerry));
        }
        help++;
        if (help >= 2)
        {
            isSailing = false;
        }

        // Zmiana kierunku podró¿y
        isReturning = !isReturning;
    }
}
