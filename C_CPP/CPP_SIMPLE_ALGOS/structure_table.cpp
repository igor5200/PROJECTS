#include <iostream>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <cstdlib>
using namespace std;

//ALGO2 IS1 221B LAB01
//Igor Stodolny
//si46655@zut.edu.pl
//igor.stodolny@gmail.com

struct struktura
{
    int liczba_calk;
    float liczba_zmienno;
    char slowo;
};

struktura** losowanie(int N)
{   
    srand(time(NULL));
    struktura** wsk;
    wsk = new struktura * [N];

    for (int i = 0; i < N; i++)
    {   
        wsk[i] = new struktura{}; 
        wsk[i]->liczba_calk = rand() % 10001 - 1000;
        wsk[i]->liczba_zmienno = 1000 + i+1;
        wsk[i]->slowo = 'B' + (rand() % (18));
    }

    return wsk;
}

void sortowanie(int N, struktura** wsk)
{   
    struktura* wsk_2;
    bool flaga;
    for (int i = 0; i < N; i++)
    {
        flaga = true;
        for (int j = 1; j < N - i; j++)
        {
            if (wsk[j - 1]->liczba_calk > wsk[j]->liczba_calk)
            {
                //swap(wsk[j - 1], wsk[j]);
                wsk_2 = wsk[j - 1];
                wsk[j - 1] = wsk[j];
                wsk[j] = wsk_2;
                flaga = false;

            }
        }

        if (flaga)
        {
            break;
        }

    }

    if(flaga)
    {
        cout << "Tablica jest posortowana!" << endl;
    }
    else
    {
        cout << "Tablica nie jest posortowana!" << endl;
    }

    for (int i = 0; i < 20; i++)
    {
        cout << wsk[i]->liczba_calk << endl;
    }
}

int zliczanie_znakow(int N, struktura** wsk, char znak)
{
    int licznik = 0;
    for (int i = 0; i < N; i++)
    {   
        if (wsk[i]->slowo == znak)
        {
            licznik += 1;
        }
    }

    return licznik;
}

void usuwanie(int N, struktura** wsk)
{
    for (int i = 0; i < N; i++)
    {
        delete wsk[i];
    }
    delete[] wsk;
}


int main()
{   

    int N;
    char X;
    srand(time(NULL));

    fstream plik;
    plik.open("inlab01.txt", ios::in);
    plik >> N;
    plik >> X;
    plik.close();

    clock_t begin;
    double time_spent;
    begin = clock();

    struktura** magazynek;
    magazynek = losowanie(N);
    sortowanie(N, magazynek);
    int zliczanie = zliczanie_znakow(N, magazynek, X);
    cout << "Znak: " << X << endl;
    usuwanie(N, magazynek);

    clock_t end;
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   
    cout << "Liczba wystapien znaku: " << zliczanie << endl;
    cout << "Czas programu: " <<time_spent << endl;
    
}



