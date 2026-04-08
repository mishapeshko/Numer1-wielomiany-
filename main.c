#include <stdio.h>   // używane dla printf, fgets.
#include <stdlib.h>  // używane dla atoi.
#include <ctype.h>   // używane dla isspace, isdigit.

#define MAX_POTEGA 10    // Maksymalny stopień wielomianu.
#define MAX_DLUGOSC 1000 // Maksymalna długość danych w jednej linijce.

// Bardzo przydatna funkcja, która ignoruje spacje. Przesuwa wskaźnik, dopóki nie skończą się spacje.
static const char *skip_spacje(const char *p)
{
    while (*p && isspace((unsigned char) *p))
    {
        p++;
    }
    return p;
}

// Funkcja wczytuje wielomian, zapisuje współczynniki do tablicy oraz określa rodzaj działania.
int wczytaj_wielomian(const char *linia, int wsp[MAX_POTEGA + 1], char *dzialanie)
{
    // Zerujemy całą tablicę współczynników.
    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        wsp[i] = 0;
    }

    const char *poz = linia;
    poz = skip_spacje(poz);

    // Rozpoznanie działania ('+' lub '*').
    if (*poz == '+' || *poz == '*')
    {
        *dzialanie = *poz;
        poz++;
    }

    poz = skip_spacje(poz);

    // Znajdujemy jednomiany - znak, wspoółczynnik oraz potęgę.
    while (*poz && *poz != '.')
    {
        int znak = 1; // Znak jednomianu (plus lub minus).
        int wspol = 0; // Wartość współczynnika.
        int potega = 0; // Wartość potęgi.
        int ma_wsp = 0; // Informacja, czy współczynnik został podany jawnie ( jesli nie to jest 1 lub -1 ).

        poz = skip_spacje(poz);

        // Określenie znaku jednomianu.
        if (*poz == '-')
        {
            znak = -1;
            poz++;
        }
        else if (*poz == '+')
        {
            poz++;
        }

        poz = skip_spacje(poz);

        // Wczytanie współczynnika, jeśli jest liczbą.
        if (isdigit((unsigned char) *poz))
        {
            wspol = atoi(poz);
            // Przesunięcie wskaźnika na koniec liczby.
            while (isdigit((unsigned char) *poz))
            {
                poz++;
            }
            ma_wsp = 1;
        }

        poz = skip_spacje(poz);

        // Wczytanie zmiennej i jej potęgi.
        if (*poz == 'x')
        {
            poz++;
            potega = 1;  // Jeżeli występuje x - domyślnie potęga jest pierwsza.
            poz = skip_spacje(poz);

            if (*poz == '^')
            {
                poz++;
                poz = skip_spacje(poz);
                potega = atoi(poz);  // atoi - funkcja, wczytująca liczbę.
                while (isdigit((unsigned char) *poz))
                {
                    poz++;
                }
            }

            if (!ma_wsp)
            {
                wspol = 1;
            }
        }

        // Dopisujemy jednomian do tablicy współczynników.
        wsp[potega] += znak * wspol;

        poz = skip_spacje(poz);
    }

    return 0;
}

// Funkcja dodaje dwa wielomiany, zapisując wynik do trzeciej tablicy.
int dodaj_wielomiany(int w1[MAX_POTEGA + 1], int w2[MAX_POTEGA + 1], int wynik[MAX_POTEGA + 1])
{
    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        wynik[i] = w1[i] + w2[i];
    }
    return 0;
}

// Funkcja mnoży dwa wielomiany w sposób szkolny.
int mnoz_wielomiany(int w1[MAX_POTEGA + 1], int w2[MAX_POTEGA + 1], int wynik[MAX_POTEGA + 1])
{
    // Zerujemy tablicę wynik, ponieważ w każdym kroku dodajemy kolejne wartości.
    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        wynik[i] = 0;
    }

    for (int i = 0; i <= MAX_POTEGA; i++)
    {
        for (int j = 0; j <= MAX_POTEGA; j++)
        {
            if (i + j <= MAX_POTEGA)
            {
                wynik[i + j] += w1[i] * w2[j];
            }
        }
    }

    return 0;
}

// Funkcja wypisuje wielomian w poprawnej postaci matematycznej.
void wypisz_wielomian(int p[MAX_POTEGA + 1])
{
    int pierwsze = 1; // Zmienna określa, czy wypisujemy pierwszy jednomian.
    int puste = 1; // Zmienna informuje, czy wielomian jest zerowy.

    for (int j = MAX_POTEGA; j >= 0; j--)
    {
        // Przechodzimy od najwyższej potęgi.
        if (p[j] != 0)
        {
            // Jeżeli współczynnik jest niezerowy, wypisujemy jednomian.
            puste = 0;

            // Obsługa znaków między jednomianami.
            if (!pierwsze)
            {
                if (p[j] < 0)  // Jeżeli jednomian nie jest pierwszy - wypisujemy ' - ' lub ' + ' z odstępami.
                {
                    printf(" - ");
                }
                else
                {
                    printf(" + ");
                }
            }
            else if (p[j] < 0)
            {
                printf("-");  // Jeżeli jednomian jest pierwszy - wypisujemy '-' bez odstępów.
            }

            pierwsze = 0;

            int wartosc = p[j];
            if (wartosc < 0)
            {
                wartosc *= -1;  // Wartość bezwzględna współczynnika.
            }

            // Wypisywanie jednomianów w zależności od potęgi.
            if (j == 0)
            {
                printf("%d", wartosc); // Potęga zerowa.
            }
            else if (j == 1)
            {
                if (wartosc != 1)
                {
                    printf("%d", wartosc);
                }
                printf("x"); // Potęga pierwsza.
            }
            else
            {
                if (wartosc != 1)
                {
                    printf("%d", wartosc);
                }
                printf("x^%d", j); // Potęga większa niż 1.
            }
        }
    }

    if (puste)
    {
        // Wypisujemy zero, jeśli wszystkie współczynniki były zerowe.
        printf("0");
    }
}

int main(void)
{
    char linia[MAX_DLUGOSC]; // Bufor na wczytywaną linię.
    int akumulator[MAX_POTEGA + 1] = {0}; // Wyzerowana tablica przechowująca wynik po każdym działaniu.
    int wynik[MAX_POTEGA + 1] = {0}; // Wyzerowana tablica pomocnicza do obliczeń.
    int wsp[MAX_POTEGA + 1]; // Tablica współczynników wczytywanego wielomianu.
    char dzialanie; // Rodzaj operacji: + lub *.

    // Wczytujemy wiersze wejścia, aż pojawi się kropka.
    while (fgets(linia, sizeof(linia), stdin) && linia[0] != '.')
    {
        // Pomijamy puste linie.
        const char *p = skip_spacje(linia);
        if (*p == '\0' || *p == '\n')
        {
            continue;
        }

        wczytaj_wielomian(linia, wsp, &dzialanie); // Wczytujemy wielomian i rodzaj działania.

        // W zależności od znaku wykonujemy działanie na wielomianach.
        if (dzialanie == '+')
        {
            dodaj_wielomiany(akumulator, wsp, wynik);
        }
        else if (dzialanie == '*')
        {
            mnoz_wielomiany(akumulator, wsp, wynik);
        }

        wypisz_wielomian(wynik); // Wypisujemy wynik działania.
        printf("\n");  // Dodajemy znak nowej linii.

        // Przepisujemy wynik do akumulatora.
        for (int i = 0; i <= MAX_POTEGA; i++)
        {
            akumulator[i] = wynik[i];
        }
    }

    return 0;
}
