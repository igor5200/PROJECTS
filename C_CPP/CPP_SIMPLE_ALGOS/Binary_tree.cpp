//ALGO2 IS1 221B LAB05
//Igor Stodolny
//si46655@zut.edu.pl
#include <iostream>
#include <time.h>
#include <ctime>
#include <random>
using namespace std;
static mt19937_64 nowy_random;
template<typename T>

struct Wezel
{
    int wysokosc;
    T klucz;
    Wezel* wsk_rodzic;
    Wezel* wsk_lewy;
    Wezel* wsk_prawy;
};
template<typename T>
class Drzewo
{
public:
    T size;
    Wezel<T>* korzen;
    Drzewo()
    {
        korzen = new Wezel<T>;
        korzen->wsk_rodzic = NULL;
        korzen->wsk_lewy = NULL;
        korzen->wsk_prawy = NULL;
        korzen->klucz = 50;
        korzen->wysokosc = 1;
        size = 1;
    }

    void dodanie_wezla(int k)
    {
        int wysokosc = 1;
        Wezel<T>* temp = korzen;
        while (true)
        {
            if (k < temp->klucz && temp->wsk_lewy == NULL)
            {
                wysokosc++;
                Wezel<T>* nowy_wezel = new Wezel<T>;
                nowy_wezel->klucz = k;
                nowy_wezel->wsk_rodzic = temp;
                nowy_wezel->wsk_lewy = NULL;
                nowy_wezel->wsk_prawy = NULL;
                nowy_wezel->wysokosc = wysokosc;
                temp->wsk_lewy = nowy_wezel;
                //cout << "Dodano lewy wezel o wartosci: " << k << endl;
                size++;
                break;
            }
            else if (k > temp->klucz && temp->wsk_prawy == NULL)
            {
                wysokosc++;
                Wezel<T>* nowy_wezel = new Wezel<T>;
                nowy_wezel->klucz = k;
                nowy_wezel->wsk_rodzic = temp;
                nowy_wezel->wsk_lewy = NULL;
                nowy_wezel->wsk_prawy = NULL;
                nowy_wezel->wysokosc = wysokosc;
                temp->wsk_prawy = nowy_wezel;
                //cout << "Dodano prawy wezel o wartosci: " << k << endl;
                size++;
                break;
            }
            else if (k < temp->klucz && temp->wsk_lewy != NULL)
            {
                wysokosc++;
                temp = temp->wsk_lewy;
            }
            else if (k > temp->klucz && temp->wsk_prawy != NULL)
            {
                wysokosc++;
                temp = temp->wsk_prawy;
            }
            else if (k == temp->klucz)
            {
                //cout << "Klucz o podanej wartosci juz istnieje!" << endl;
                break;
            }
        }
        
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

    Wezel<T>* find_nastepnik(Wezel<T>* wezel)
    {
        Wezel<T>* temp = wezel;
        while (temp->wsk_lewy != NULL)
        {
            temp = temp->wsk_lewy;
        }
        return temp;
    }

    Wezel<T>* find_poprzednik(Wezel<T>* wezel)
    {
        Wezel<T>* temp = wezel;
        while (temp->wsk_prawy != NULL)
        {
            temp = temp->wsk_prawy;
        }
        return temp;
    }

    void usuwanie_wezla(Wezel<T>* wezel)
    {
        if (wezel == NULL)
        {
            cout << "Blad! wezel == NULL" << endl;
            return;
        }
        Wezel<T>* temp = korzen;
        
        while (true)
        {
            if (temp->klucz > wezel->klucz)
            {
                temp = temp->wsk_lewy;
            }
            else if (temp->klucz < wezel->klucz)
            {
                temp = temp->wsk_prawy;
            }
            else if (temp->klucz == wezel->klucz)
            {
                if (wezel->wsk_lewy == NULL && wezel->wsk_prawy == NULL)
                {
                    if (wezel->klucz < wezel->wsk_rodzic->klucz)
                    {
                        wezel->wsk_rodzic->wsk_lewy = NULL;
                        delete wezel;
                        size--;
                        return;
                    }
                    else
                    {
                        wezel->wsk_rodzic->wsk_prawy = NULL;
                        delete wezel;
                        size--;
                        return;
                    }
                }
                else if ((wezel->wsk_lewy != NULL && wezel->wsk_prawy == NULL) || (wezel->wsk_lewy == NULL && wezel->wsk_prawy != NULL))
                {
                    if (wezel->klucz > wezel->wsk_rodzic->klucz && wezel->wsk_lewy != NULL && wezel->wsk_prawy == NULL)
                    {
                        wezel->wsk_rodzic->wsk_prawy = wezel->wsk_lewy;
                        wezel->wsk_lewy = wezel->wsk_rodzic;
                        delete wezel;
                        size--;
                        return;
                    }
                    else if (wezel->klucz < wezel->wsk_rodzic->klucz && wezel->wsk_lewy != NULL && wezel->wsk_prawy == NULL)
                    {
                        wezel->wsk_rodzic->wsk_lewy = wezel->wsk_lewy;
                        wezel->wsk_lewy = wezel->wsk_rodzic;
                        delete wezel;
                        size--;
                        return;
                    }
                    else if (wezel->klucz > wezel->wsk_rodzic->klucz && wezel->wsk_lewy == NULL && wezel->wsk_prawy != NULL)
                    {
                        wezel->wsk_rodzic->wsk_prawy = wezel->wsk_prawy;
                        wezel->wsk_prawy = wezel->wsk_rodzic;
                        delete wezel;
                        size--;
                        return;
                    }
                    else if (wezel->klucz < wezel->wsk_rodzic->klucz && wezel->wsk_lewy == NULL && wezel->wsk_prawy != NULL)
                    {
                        wezel->wsk_rodzic->wsk_lewy = wezel->wsk_prawy;
                        wezel->wsk_prawy = wezel->wsk_rodzic;
                        delete wezel;
                        size--;
                        return;
                    }
                }
                else if (wezel->wsk_lewy != NULL && wezel->wsk_prawy != NULL)
                {
                    if (wezel->klucz < wezel->wsk_rodzic->klucz)
                    {
                        Wezel<T>* nastepnik = find_nastepnik(wezel->wsk_prawy);
                        if (nastepnik->wsk_lewy == NULL && nastepnik->wsk_prawy == NULL)
                        {
                            wezel->wsk_rodzic->wsk_lewy = nastepnik;
                            nastepnik = wezel;
                            delete wezel;
                            return;
                        }
                        else if (nastepnik->wsk_lewy == NULL && nastepnik->wsk_prawy != NULL)
                        {
                            nastepnik->wsk_rodzic->wsk_lewy = nastepnik->wsk_prawy;
                            nastepnik->wsk_prawy = nastepnik->wsk_rodzic;
                            wezel->wsk_rodzic->wsk_lewy = nastepnik;
                            delete wezel;
                            return;
                        }
                    }
                    else
                    {
                        Wezel<T>* nastepnik = find_nastepnik(wezel->wsk_prawy);
                        if (nastepnik->wsk_lewy == NULL && nastepnik->wsk_prawy == NULL)
                        {
                            nastepnik->wsk_rodzic->wsk_prawy = nastepnik;
                            nastepnik = wezel;
                            delete wezel;
                            return;
                        }
                        else if (nastepnik->wsk_lewy == NULL && nastepnik->wsk_prawy != NULL)
                        {
                            nastepnik->wsk_rodzic->wsk_lewy = nastepnik->wsk_prawy;
                            nastepnik->wsk_prawy = nastepnik->wsk_rodzic;
                            wezel->wsk_rodzic->wsk_prawy = nastepnik;
                            delete wezel;
                            return;
                        }
                    }
                }
                
            }


        }
            
    }

    void preorder(Wezel<T>* wezel)
    {
        if (wezel != NULL)
        {
            cout << "Klucz= " << wezel->klucz << endl;
            preorder(wezel->wsk_lewy);
            preorder(wezel->wsk_prawy);
        }
    }

    void inorder(Wezel<T>* wezel)
    {
        if (wezel != NULL)
        {
            inorder(wezel->wsk_lewy);
            cout << "Klucz= " << wezel->klucz << endl;
            inorder(wezel->wsk_prawy);
        }
    }

    void usuwanie(Wezel<T>* wezel)
    {
        
        if (wezel == NULL)
        {
            return;
        }
        usuwanie(wezel->wsk_lewy);
        usuwanie(wezel->wsk_prawy);
        delete wezel;
    }

    int h = 0;
    int get_wysokosc(Wezel<T>* wezel)
    {
        
        if (wezel != NULL)
        {
            if (wezel->wysokosc > h)
            {
                h = wezel->wysokosc;
            }
            get_wysokosc(wezel->wsk_lewy);
            get_wysokosc(wezel->wsk_prawy);
            return h;
        }
    }
};
int main()
{
    nowy_random.seed(time(NULL));
    int MAX_ORDER = 7;
    int liczba = pow(10, 4);
    Drzewo<int> drzewo;
    double begin = clock();

    while (drzewo.size != liczba)
    {
        drzewo.dodanie_wezla(nowy_random() % liczba);
    }

    drzewo.dodanie_wezla(20000);
    drzewo.wyszukiwanie_wezla(20000);
    drzewo.usuwanie_wezla(drzewo.wyszukiwanie_wezla(20000));
    drzewo.wyszukiwanie_wezla(20000);
    drzewo.preorder(drzewo.wyszukiwanie_wezla(50));
    cout << '\n' << endl;
    cout << '\n' << endl;
    drzewo.inorder(drzewo.wyszukiwanie_wezla(50));
    cout << "Wysokosc= " << drzewo.get_wysokosc(drzewo.wyszukiwanie_wezla(50)) << endl;
    drzewo.usuwanie(drzewo.wyszukiwanie_wezla(50));

    double end = clock();
    cout << (end - begin) / CLOCKS_PER_SEC << endl;
    
    

}


