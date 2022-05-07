//ALGO2 IS1 221B LAB02
//Igor Stodolny
//si46655@zut.edu.pl igor.stodolny@gmail.com
#include <iostream>
//#include "Tablica.h"
#include <time.h>
using namespace std;
template<typename T>
class Tablica
{
private:
    int size;
    int max_size;
    //template<typename T>
    T* tablica;

public:

    Tablica()
    {
        size = 0;
        max_size = 1;
        tablica = new T[max_size];
    }

    void dodanie_powiekszenie(T dane)
    {

        if (size < max_size)
        {
            tablica[size] = dane;
            size += 1;
        }

        else
        {
            max_size = max_size * 2;
            T* nowa_tablica = new T[max_size];
            for (int i = 0; i < size; i++)
            {
                nowa_tablica[i] = tablica[i];
            }
            delete[]tablica;
            tablica = nowa_tablica;
            tablica[size] = dane;
            size += 1;
        }

    }
    void zwrocenie_wartosci(int i)
    {

        if (i < size)
        {
            cout << tablica[i];
        }
        else
        {
            cout << "BLAD!" << endl;
        }
    }
    void podmiana(int i, T dane)
    {
        if (i < max_size)
        {
            tablica[i] = dane;
        }
        else
        {
            cout << "Niepowodzenie" << endl;
        }
    }

    void sortowanie()
    {
        for (int i = 0; i < size; i++)
        {
            T schowek = 0;
            for (int j = 1; j < size - i; j++)
            {
                if (tablica[j - 1] > tablica[j])
                {
                    schowek = tablica[j - 1];
                    tablica[j - 1] = tablica[j];
                    tablica[j] = schowek;
                }
            }
        }
    }



    void usuniecie()
    {
        delete[] tablica;
    }

    void wyswietl_dane()
    {
        cout << max_size << endl;
        cout << size << endl;
        for (int i = 0; i < 20; i++)
        {
            cout << tablica[i] << endl;
        }
    }
};


int main()
{
    
    srand(time(NULL));
    Tablica<int> obiekt;
    clock_t begin, end;
    double time_spent;
    begin = clock();
    for (int i = 0; i < pow(10, 5); i++)
    {
        obiekt.dodanie_powiekszenie(rand()%1000 + 1);
    }
    obiekt.zwrocenie_wartosci(1001);
    obiekt.podmiana(1001, 2);
    obiekt.zwrocenie_wartosci(1001);
    obiekt.sortowanie();
    obiekt.zwrocenie_wartosci(1001);
    obiekt.wyswietl_dane();
    obiekt.usuniecie();
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << time_spent;
}