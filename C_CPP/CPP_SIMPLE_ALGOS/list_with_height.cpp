//ALGO2 IS1 221B LAB04
//Igor Stodolny
//si46655@wi.zut.edu.pl igor.stodolny@gmail.com
#include <iostream>
#include <time.h>
#include <random>
#include <fstream>
using namespace std;

struct Wezel
{
    int klucz;
    int h;
    Wezel** nastepny;
};

int wysokosc()
{
    

    int h = 1;
    while ((rand() % 100 < 0.5 * 100) && (h < 10)) {
        h++;
    }
    return h;
}


class Lista
{
public:
    Wezel* glowa;
    Wezel* ogon;
    int size;

    Lista()
    {
        glowa = new Wezel;
        ogon = new Wezel;

        glowa->klucz = 0;
        glowa->h = 10;
        glowa->nastepny = new Wezel * [glowa->h];
        for (int i = 0; i < glowa->h; i++)
        {
            glowa->nastepny[i] = new Wezel;
        }

        ogon->klucz = 100000;
        ogon->h = 10;
        ogon->nastepny = new Wezel * [ogon->h];
        for (int i = 0; i < ogon->h; i++)
        {
            ogon->nastepny[i] = NULL;
            glowa->nastepny[i] = ogon;
        }

        size = 2;
    }

    void wstawianie_wezla(int k)
    {
        Wezel* temp = new Wezel;
        Wezel* temp2 = new Wezel[10];
        temp = glowa;
        for (int i = 9; i >= 0; i--)
        {
            if (temp->nastepny[i] != NULL)
            {
                while (temp->nastepny[i]->klucz < k)
                {
                    temp = temp->nastepny[i];
                }
            }
            temp2[i] = *temp;
        }

            temp = temp->nastepny[0];
        
            if (temp->klucz == k)
            {

                return;
            }
            int high = wysokosc();
            Wezel* nowy_wezel = new Wezel;
            nowy_wezel->klucz = k;
            nowy_wezel->h = high;
            nowy_wezel->nastepny = new Wezel * [high];
            for (int i = 0; i < high; i++)
            {
                nowy_wezel->nastepny[i] = temp2[i].nastepny[i];
                temp2[i].nastepny[i] = nowy_wezel;
            }
            size++;
    }


    void wstawianie_X_wezlow(int X)
    {
        int size_temp = size + X;
        while(size < size_temp)
        {
            wstawianie_wezla(rand() % 10000 + 2);
        }
    }

    void szukanie_klucza(int k)
    {
        Wezel* temp = new Wezel;
        temp = glowa;
        for (int i = 9; i >= 0; i--)
        {
            if (temp->nastepny[i] != NULL)
            {
                while (temp->nastepny[i]->klucz < k)
                {
                    temp = temp->nastepny[i];
                }
            }
        }
        temp = temp->nastepny[0];
        if (temp->klucz == k)
        {
            cout << "Klucz znajduje sie w liscie!" << endl;
        }
        else
        {
            cout << "Klucz NIE znajduje sie w liscie!" << endl;
        }
    }

    void  wyswietlanie(int N, int Y)
    {
        Wezel* temp;
        temp = glowa;
        int size2 = 0;
        for (int i = 0; i < Y; i++)
        {
            if (temp->h >= N)
            {
                cout << temp->klucz << endl;
                size2++;
            }
            temp = temp->nastepny[0];
        }
        cout << "Widoczne wezly na podanej wysokosci: " << size2 << endl;
    }


    void wyswietlenie_wezla(int Y)
    {
        Wezel* temp;
        temp = glowa->nastepny[0];
        
            for (int i = 0; i < Y; i++)
            {
                for (int j = 0; j < temp->h; j++)
                {
                    cout << temp->klucz << " ";
                }
                temp = temp->nastepny[0];
                cout << endl;
            
        }
    }

   

    void usuwanie_wezlow(int k)
    {
        Wezel* temp;
        Wezel* temp2 = new Wezel[10];
        temp = glowa;

        for (int i = 9; i >= 0; i--)
        {
            while (temp->nastepny[i]->klucz < k)
            {
                temp = temp->nastepny[i];
            }
            temp2[i] = *temp;
        }
        temp = temp->nastepny[0];
        if (temp->klucz > k)
        {
            cout << "Brak klucza" << endl;
            return;
        }
        for (int i = 0; i < temp->h; i++)
        {
            temp2[i].nastepny[i] = temp->nastepny[i];
        }

        delete[] temp->nastepny;
        delete temp;
        delete[] temp2;
        size--;
    }

    void usuwanie_listy()
    {
        Wezel* temp;
        
        for (int i = size-1; i >= 0; i--)
        {
            temp = glowa->nastepny[0];
            delete[] glowa->nastepny;
            delete glowa;
            glowa = temp;
            size--;
        }
        
    }
};




int main()
{
    int k1, k2, k3, k4, k5, X, LMAX;

    ifstream plik("inlab03.txt");

    if (plik.is_open())
    {
        plik >> X >> LMAX >> k1 >> k2 >> k3 >> k4 >> k5;
        plik.close();
    }
    else {
        cout << "Blad" << endl;
        return 0;
    }

    plik.close();
    double begin, end;
    begin = clock();
    Lista lista;
    lista.szukanie_klucza(k1);
    lista.wstawianie_X_wezlow(X);
    cout << lista.size - 2 << endl;
    lista.wyswietlanie(1, 20);
    lista.wstawianie_wezla(k2);
    lista.wyswietlanie(1, 20);
    lista.wstawianie_wezla(k3);
    lista.wyswietlanie(1, 20);
    lista.wstawianie_wezla(k4);
    lista.wyswietlanie(1, 20);
    lista.wstawianie_wezla(k5);
    cout << lista.size - 2 << endl;
    lista.wyswietlenie_wezla(20);
    lista.usuwanie_wezlow(k3);
    lista.usuwanie_wezlow(k2);
    lista.usuwanie_wezlow(k5);
    cout << lista.size - 2 << endl;
    lista.wyswietlenie_wezla(20);
    lista.usuwanie_listy();
    end = clock();
    cout << "Czas: " << double(end - begin) / CLOCKS_PER_SEC;
    

}


