//ALGO2 IS1 221B LAB06
//Igor Stodolny
//si46655@zut.edu.pl
#include <iostream>
#include <time.h>
#include <ctime>
#include <random>
#include <math.h>
using namespace std;
template<typename T>

struct Wezel
{
    T klucz;
    Wezel* wsk_rodzic;
    Wezel* wsk_lewy;
    Wezel* wsk_prawy;
    bool kolor; //0-czarny, 1-czerwony
};
template<typename T>
class Drzewo
{
public:
    Wezel<T>* korzen;
    Wezel<T>* NIL;
    T size;
    Drzewo()
    {
        NIL = new Wezel<T>;
        NIL->wsk_lewy = NULL;
        NIL->wsk_prawy = NULL;
        NIL->klucz = 0;
        NIL->kolor = 0;


        korzen = new Wezel<T>;
        korzen->wsk_rodzic = NIL;
        korzen->wsk_lewy = NIL;
        korzen->wsk_prawy = NIL;
        korzen->klucz = 50;
        korzen->kolor = 0;

        
        size = 1;
    }
    
    void dodanie_wezla(int k)
    {
        Wezel<T>* temp = NIL;
        Wezel<T>* temp2 = korzen;
        Wezel<T>* nowy_wezel = new Wezel<T>;
        nowy_wezel->klucz = k;
        nowy_wezel->wsk_lewy = NIL;
        nowy_wezel->wsk_prawy = NIL;
        nowy_wezel->kolor = 1;
        while (temp2 != NIL)
        {
            temp = temp2;
            if (k < temp2->klucz)
            {
                temp2 = temp2->wsk_lewy;
            }
            else
            {
                temp2 = temp2->wsk_prawy;
            }
        }
        nowy_wezel->wsk_rodzic = temp;
        if (temp == NIL)
        {
            korzen = nowy_wezel;
        }
        else if (nowy_wezel->klucz < temp->klucz)
        {
            temp->wsk_lewy = nowy_wezel;
        }
        else
        {
            temp->wsk_prawy = nowy_wezel;
        }
        balance(nowy_wezel);
        size++;
    }

    Wezel<T>* wyszukiwanie_wezla(int k)
    {
        Wezel<T>* temp = korzen;

        while (true)
        {

            if (k == temp->klucz)
            {
                cout << "Klucz o podanej wartosci znajduje sie w drzewie!" << endl;
                return temp;
            }
            else if (k < temp->klucz && temp->wsk_lewy == NULL)
            {
                cout << "Nie znaleziono klucza!" << endl;
                temp = NULL;
                return temp;
            }
            else if (k > temp->klucz && temp->wsk_prawy == NULL)
            {
                cout << "Nie znaleziono klucza" << endl;
                temp = NULL;
                return temp;
            }
            else if (k < temp->klucz && temp->wsk_lewy != NULL)
            {
                temp = temp->wsk_lewy;
            }
            else if (k > temp->klucz && temp->wsk_prawy != NULL)
            {
                temp = temp->wsk_prawy;
            }
        }
    }



    int preorder(Wezel<T>* wezel)
    {
        int wysokosc = 0;
        if (wezel != NULL)
        {
            cout << "Klucz= " << wezel->klucz << endl;
            preorder(wezel->wsk_lewy);
            preorder(wezel->wsk_prawy);
        }
        return wysokosc;
    }

    void inorder(Wezel<T>* wezel)
    {
        if (wezel != NULL)
        {
            inorder(wezel->wsk_lewy);
            cout << "klucz= " << wezel->klucz << "\t kolor=" << wezel->kolor << endl;
            inorder(wezel->wsk_prawy);
        }
    }

    void usuwanie(Wezel<T>* wezel)
    {
        if (wezel == NIL)
        {
            return;
        }
        usuwanie(wezel->wsk_lewy);
        usuwanie(wezel->wsk_prawy);
        korzen = NULL;
        size = 0;
        delete wezel;
    }

    int get_balance(Wezel<T>* wezel)
    {
        if (wezel == NULL)
        {
            return -1;
        }


        int lewe_poddrzewo = get_balance(wezel->wsk_lewy);
        int prawe_poddrzewo = get_balance(wezel->wsk_prawy);

        if (lewe_poddrzewo > prawe_poddrzewo)
        {
            return lewe_poddrzewo + 1;
        }
        else
        {
            return prawe_poddrzewo + 1;
        }
    }

    void lewa_rotacja(Wezel<T>* nowy_wezel)
    {
        Wezel<T>* temp = nowy_wezel->wsk_prawy;
        nowy_wezel->wsk_prawy = temp->wsk_lewy;
        if (temp->wsk_lewy != NIL)
        {
            temp->wsk_lewy->wsk_rodzic = nowy_wezel;
        }
        temp->wsk_rodzic = nowy_wezel->wsk_rodzic;
        if (nowy_wezel->wsk_rodzic == NIL)
        {
            korzen = temp;
        }
        else if (nowy_wezel == nowy_wezel->wsk_rodzic->wsk_lewy)
        {
            nowy_wezel->wsk_rodzic->wsk_lewy = temp;
        }
        else
        {
            nowy_wezel->wsk_rodzic->wsk_prawy = temp;
        }
        
        nowy_wezel->wsk_rodzic = temp;
        temp->wsk_lewy = nowy_wezel;
    }

    void prawa_rotacja(Wezel<T>* nowy_wezel)
    {
        Wezel<T>* temp = nowy_wezel->wsk_lewy;
        nowy_wezel->wsk_lewy = temp->wsk_prawy;
        if (temp->wsk_prawy != NIL)
        {
            temp->wsk_prawy->wsk_rodzic = nowy_wezel;
        }
        temp->wsk_rodzic = nowy_wezel->wsk_rodzic;
        if (nowy_wezel->wsk_rodzic == NIL)
        {
            korzen = temp;
        }
        else if (nowy_wezel == nowy_wezel->wsk_rodzic->wsk_prawy)
        {
            nowy_wezel->wsk_rodzic->wsk_prawy = temp;
        }
        else
        {
            nowy_wezel->wsk_rodzic->wsk_lewy = temp;
        }
        
        nowy_wezel->wsk_rodzic = temp;
        temp->wsk_prawy = nowy_wezel;
    }


    void balance(Wezel<T>* nowy_wezel)
    {
        Wezel<T>* temp = new Wezel<T>;
        while (nowy_wezel->wsk_rodzic->kolor == 1)
        {
            if (nowy_wezel->wsk_rodzic == nowy_wezel->wsk_rodzic->wsk_rodzic->wsk_lewy)
            {
                temp = nowy_wezel->wsk_rodzic->wsk_rodzic->wsk_prawy;
                if (temp->kolor == 1)
                {
                    nowy_wezel->wsk_rodzic->kolor = 0;
                    temp->kolor = 0;
                    nowy_wezel->wsk_rodzic->wsk_rodzic->kolor = 1;
                    nowy_wezel = nowy_wezel->wsk_rodzic->wsk_rodzic;
                }
                else
                {
                    if (nowy_wezel == nowy_wezel->wsk_rodzic->wsk_prawy)
                    {
                        nowy_wezel = nowy_wezel->wsk_rodzic;
                        lewa_rotacja(nowy_wezel);
                    }
                    nowy_wezel->wsk_rodzic->kolor = 0;
                    nowy_wezel->wsk_rodzic->wsk_rodzic->kolor = 1;
                    prawa_rotacja(nowy_wezel->wsk_rodzic->wsk_rodzic);
                }
                    
            }
            else
            {
                temp = nowy_wezel->wsk_rodzic->wsk_rodzic->wsk_lewy;
                if (temp->kolor == 1)
                {
                    nowy_wezel->wsk_rodzic->kolor = 0;
                    temp->kolor = 0;
                    nowy_wezel->wsk_rodzic->wsk_rodzic->kolor = 1;
                    nowy_wezel = nowy_wezel->wsk_rodzic->wsk_rodzic;
                }
                else
                {
                    if (nowy_wezel == nowy_wezel->wsk_rodzic->wsk_lewy)
                    {
                        nowy_wezel = nowy_wezel->wsk_rodzic;
                        prawa_rotacja(nowy_wezel);
                    }
                    nowy_wezel->wsk_rodzic->kolor = 0;
                    nowy_wezel->wsk_rodzic->wsk_rodzic->kolor = 1;
                    lewa_rotacja(nowy_wezel->wsk_rodzic->wsk_rodzic);
                }
                    
            }
        }
        korzen->kolor = 0;
    }
    int get_wysokosc(Wezel<T>* wezel)
    {
        if(wezel == NIL)
        {
            return 0;
        }
        else
        {
            int lewa_wysokosc = get_wysokosc(wezel->wsk_lewy);
            int prawa_wysokosc = get_wysokosc(wezel->wsk_prawy);
            if (lewa_wysokosc > prawa_wysokosc)
            {
                return (lewa_wysokosc + 1);
            }
            else
            {
                return (prawa_wysokosc + 1);
            }
        }
    }

};
int main()
{
    
    Drzewo<int> drzewo;
    
    srand(time(NULL));
    const int MAX_ORDER = 7;

    for (int i = 0; i < MAX_ORDER; i++)
    {
        int n = pow(10, i);
        double begin = clock();
        for (int j = 0; j < n; j++)
        {
            drzewo.dodanie_wezla(rand() % 100000);
        }
        double end = clock();
        cout << "Czas dodawania " << (end - begin) / CLOCKS_PER_SEC << endl;

        int m = pow(10, 4);
        double begin2 = clock();
        for (int j = 0; j < m; j++)
        {
            drzewo.wyszukiwanie_wezla(rand() % 1000);
        }
        double end2 = clock();
        cout << "Czas wyszukiwania " << (end2 - begin2) / CLOCKS_PER_SEC << endl;
    }
    cout << "Wysokosc: " << drzewo.get_wysokosc(drzewo.korzen) << endl;
    drzewo.usuwanie(drzewo.korzen);
    return 0;
}