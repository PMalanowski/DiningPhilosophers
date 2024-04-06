// g++ philosophers.cpp -lncurses -o main i ./main numOfPhilo
#include <iostream>
#include <cstdlib>
#include <thread>
#include <ncurses.h>
//#include <pthread.h>
#include <random>
#include <chrono>
#include <string>
#include <time.h>
#include <mutex>

using namespace std;



struct philosopher
{
    philosopher()
    {
        this->status = "Sitting down";
        this->numer = 1;
    };
    string status;
    int numer;
    void dine(mutex *forks,int numOfPhilo,bool *forkState)
    {
        while(true)
        {
            int czas = rand() % 10000 + 500;
            this->status = "Thinks";
            this_thread::sleep_for(chrono::milliseconds(czas));
            //czas = rand() % 1000 + 500;
            this->status = "Wants to eat";
            //this_thread::sleep_for(chrono::milliseconds(czas));
            //widelce[numer-1].lock();
            //widelce[numer%numOfPhilo].lock();
            lock(forks[numer-1], forks[numer%numOfPhilo]);
            forkState[numer-1] = 0;
            forkState[numer%numOfPhilo] = 0;
            czas = rand() % 10000 + 500;
            this->status = "Eating";
            this_thread::sleep_for(chrono::milliseconds(czas));
            forks[numer-1].unlock();
            forks[numer%numOfPhilo].unlock();
            forkState[numer-1] = 1;
            forkState[numer%numOfPhilo] = 1;
        }
    };

};

void uczta(philosopher *tab_filo, int l_filo, bool *forkState)
{
    initscr();
    
    while(true)
    {

        clear();
        printw("Dining Philosophers\n\n");
        printw("Philosopher:\t\tState:\tState of forks:\n");
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK); // ustawianie koloru tekstu i tla do wlaczania i wylaczania
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        for(int i = 0; i < l_filo; i++)
        {
            string wynik = "Philosopher " + to_string(tab_filo[i].numer) + "\t\t";
            printw("%s",wynik.c_str()); //c_str zmienia std::string na C_string i umozliwia wyswietlenie przy pomocy printw
            string status = tab_filo[i].status;
            if(tab_filo[i].status == "Thinks")
            {
                    attron(COLOR_PAIR(1));
                    printw("Thinks\t");
                    attroff(COLOR_PAIR(1));
            }
            else if(tab_filo[i].status == "Wants to eat")
            {
                    attron(COLOR_PAIR(2));
                    printw("Waiting\t");
                    attroff(COLOR_PAIR(2));
            }
            else if(tab_filo[i].status == "Eating")
            {
                    attron(COLOR_PAIR(3));
                    printw("Eating\t");
                    attroff(COLOR_PAIR(3));
            }
            if(forkState[i] == 1)
            {
                attron(COLOR_PAIR(1)); // wlaczenie koloru
                printw("%d",i+1);
                attroff(COLOR_PAIR(1)); // wylaczenie koloru
            }
            else
            {
                attron(COLOR_PAIR(3));
                printw("%d",i+1);
                attroff(COLOR_PAIR(3));
            }
            
            printw("\n");
        };
        this_thread::sleep_for(chrono::milliseconds(333));
        refresh();
    };
    endwin();
};

int main(int argc, char *argv[]){ // argc - ilosc argumentow+nazwa , argv - tablica argumentow 
    int numOfPhilo = atoi(argv[1]);
    srand (time(NULL));
    philosopher *philosophers = new philosopher[numOfPhilo];
    thread watki_filo[numOfPhilo];
    mutex *forks = new mutex[numOfPhilo];
    bool forkState[numOfPhilo];
    for(int i = 0; i < numOfPhilo; i++)
    {
        forkState[i] = 1;
        philosophers[i].numer = i+1;
        watki_filo[i] = thread([&,i]{ // [] - lambda expression & oznacza pobranie zmiennych przez referencje a i pobranie i poprzez kopie
            philosophers[i].dine(forks,numOfPhilo,forkState);
        });
    }
    uczta(philosophers,numOfPhilo,forkState);
    /*
    initscr(); // inicjalizacja ekranu ncurses
    printw("%d",numOfPhilo); // wyswietlanie na ekran ncurses
    refresh(); // odswierzenie ekranu ncurses
    getch(); // czekanie na input do ncurses
    endwin(); // dealokacja pamieci i zakonczenie ncurses
    */

return 0;
}

