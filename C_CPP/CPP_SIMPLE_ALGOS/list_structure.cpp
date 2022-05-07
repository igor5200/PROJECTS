ALGO2 IS1 221B LAB03
Igor Stodolny
si46655@zut.edu.pl
#include <iostream>
#include <time.h>
#include <random>
#include <fstream>

using namespace std;

struct Wezel
{
    int klucz;
    double liczba;
    char slowo;
    Wezel* nastepny;
};


class Lista
{
public:
    Wezel* glowa = new Wezel;
    int size = 0;
    Lista()
    {
        glowa->klucz = 1;
        glowa->liczba = 1;
        glowa->slowo = 'A';
        glowa->nastepny = NULL;
        size = 1;
    }

    void wstawianie_wezla(int k)
    {
        Wezel* nowy_wezel = new Wezel;
        nowy_wezel->klucz = k;
        nowy_wezel->liczba = rand() % 100;
        nowy_wezel->slowo = 'T';
        nowy_wezel->nastepny = NULL;
        
        if (size == 1)
        {
            glowa->nastepny = nowy_wezel;
            size++;
            return;
        }
        else
        {
            Wezel* temp_p = new Wezel;
            temp_p = glowa;
            Wezel* temp_b = new Wezel;
            bool koniec = 1;
            for (int i = 0; i < size ; i++)
            {
                if (k == temp_p->klucz)
                {
                    cout << "Blad!!" << endl;
                    return;
                }
                else if (k > temp_p->klucz && temp_p->nastepny != NULL)
                {
                    temp_b = temp_p;
                    temp_p = temp_p->nastepny;
                }
                else if (k < temp_p->klucz && temp_p->nastepny != NULL)
                {
                    nowy_wezel->nastepny = temp_p;
                    temp_b->nastepny = nowy_wezel;
                    koniec = 0;
                    size++;
                    return;
                }
            }
            if (koniec == 1)
            {
                temp_p->nastepny = nowy_wezel;
                nowy_wezel->nastepny = NULL;
                size++;
                return;
            }
 
        }
     
    }

    void usuwanie()
    {
       Wezel* temp = NULL;
       while (size != 0)
       {
           if (size < 2)
           {
               delete glowa;
               size--;
           }
           else
           {
                temp = glowa->nastepny;
                delete glowa;
                glowa = temp;
                size--;
           }
        }
    }

    

    void wstawianie_X_wezlow(int X)
    {
        
        for (int i = 0; i < X; i++)
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> distribution(99, 99999);
            int random = distribution(gen);
            wstawianie_wezla(random);
        }
    }


    void szukanie(int k)
    {
        Wezel* temp;
        temp = glowa;
        bool znaleziono = 1;
        for (int i = 0; i < size; i++)
        {
            if (k == temp->klucz)
            {
                znaleziono = 0;
                cout << "znaleziono!" << endl;
                return;
            }
            else
            {
                temp = temp->nastepny;
            }

        }
        if (znaleziono == 1)
        {
            cout << "Nie ma takiego klucza!" << endl;
            
        }
    }

    void usuwanie_wezla(int k)
    {
        Wezel* temp = new Wezel;
        Wezel* temp2 = new Wezel;
        temp = glowa;
        bool usunieto = 1;
        for (int i = 0; i < size; i++)
        {

            if (k == temp->klucz)
            {
                usunieto = 0;
                temp2->nastepny = temp->nastepny;
                delete temp;
                size--;
                return;
            }
            else
            {
                temp2 = temp;
                temp = temp->nastepny;
            }
        }
        if (usunieto == 1)
        {
            cout << "Brak elementu o podanym kluczu!" << endl;
        }
    }

    void wyswietlanie_poczatkowe(int x)
    {
        Wezel* temp = new Wezel;
        temp = glowa;
        if (size < x)
        {
            cout << "Nie ma tylu wezlow!" << endl;
            
        }
        else
        {
            for (int i = 0; i < x; i++)
            {
                cout << temp->klucz << endl;
                if (temp->nastepny != NULL)
                {
                   temp = temp->nastepny;
                }
                
            }
        }
    }

    void wyswietlanie_koncowe(int x)
    {
        Wezel* temp_p = new Wezel;
        temp_p = glowa;
        int schowek = 0;
        if (size < x)
        {
            cout << "Nie ma tyle wezlow!" << endl;
        }
        else
        {
            schowek = size - x;
            for (int i = 0; i < schowek; i++)
            {
                temp_p = temp_p->nastepny;
            }
            for (int i = 0; i < x; i++)
            {
                cout << temp_p->klucz << endl;
                if (temp_p->nastepny != NULL)
                {
                    temp_p = temp_p->nastepny;
                }
            }
        }
    }
};



int main()
{
    int k1, k2, k3, k4, k5, X;
    
    ifstream plik("inlab02.txt");
    
    if (plik.is_open())
    {
        plik >> X  >> k1 >> k2 >> k3 >> k4 >> k5;
        plik.close();
    }
    else {
        cout << "Blad" << endl;
        return 0;
    }

    plik.close();


    double begin, end;
    begin = clock();
    Lista* lista = new Lista;
    Wezel* wezel = new Wezel;
    lista->szukanie(k1);
    lista->wstawianie_X_wezlow(X);
    cout << lista->size << endl;
    lista->wyswietlanie_poczatkowe(20);
    lista->szukanie(k2);
    lista->wyswietlanie_poczatkowe(20);
    lista->szukanie(k3);
    lista->wyswietlanie_poczatkowe(20);
    lista->szukanie(k4);
    lista->wyswietlanie_poczatkowe(20);
    lista->szukanie(k5);
    lista->usuwanie_wezla(k3);
    lista->wyswietlanie_poczatkowe(20);
    lista->usuwanie_wezla(k2);
    lista->wyswietlanie_poczatkowe(20);
    lista->usuwanie_wezla(k5);
    cout << lista->size << endl;
    lista->szukanie(5);
    lista->wyswietlanie_koncowe(11);
    cout << lista->size << endl;
    lista->usuwanie();
    
    
    
    lista->usuwanie();
    end = clock();
    cout << "Czas: " << double(end - begin) / CLOCKS_PER_SEC;
}