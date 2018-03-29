#include "timefuncs.h"

/*
    Ce fichier contient les prototypes des sous programmes utilisés pour les mesures du temps
    Adaptés en fonction du système d'exploitation

    Merci à Dietrich Epp pour l'explication de timespec et clock_gettime(CLOCK_MONOTONIC...
    http://stackoverflow.com/questions/43295845/is-the-clocks-per-sec-value-wrong-inside-a-virtual-machine/43296016#43296016
*/

#ifdef _WINDOWS

void getTime(TIMESTRUCT* watch)
{
    //what to do?
}

//renvoie le nombre de secondes passées entre first et second
int getSecInt(TIMESTRUCT* first, TIMESTRUCT* second)
{
    //what to do?
}

//renvoie le nombre de secondes passées entre first et second
double getSec(TIMESTRUCT* first, TIMESTRUCT* second)
{
    //what to do?
}

//renvoie le nombre de milisecondes passées entre first et second
int getMilisec(TIMESTRUCT* first, TIMESTRUCT* second)
{
    //what to do?
}

//renvoie le nombre de microsecondes passées entre first et second
long int getMicrosec(TIMESTRUCT* first, TIMESTRUCT* second)
{
    //what to do?
}

//ajoute (ou enlève) un certain nombre de seconde changeTime
void addSec(TIMESTRUCT* changeTime, int howMuch)
{

}


#else

void getTime(TIMESTRUCT* watch)
{
    clock_gettime(CLOCK_MONOTONIC, watch);
}


//renvoie le nombre de secondes passées entre first et second
int getSecInt(TIMESTRUCT* first, TIMESTRUCT* second)
{
    //le nombre de secondes passées (troncature)
    return (int) (second->tv_sec - first->tv_sec);
    //1e-9: on multiplie par 0.000 000 001 (nanosecondes)
}

//renvoie le nombre de secondes passées entre first et second
double getSec(TIMESTRUCT* first, TIMESTRUCT* second)
{
    //le nombre de secondes passées (troncature)
    return (second->tv_sec - first->tv_sec) + 1e-9 * (second->tv_nsec - first->tv_nsec);
    //1e-9: on multiplie par 0.000 000 001 (nanosecondes)
}

//renvoie le nombre de milisecondes passées entre first et second
int getMilisec(TIMESTRUCT* first, TIMESTRUCT* second)
{
    int sec, usec;

    //le nombre de secondes passées (est généralement 0)
    sec = second->tv_sec - first->tv_sec;

    //le nombre de microsec passées (géneralement négatif quand sec = 1)
    usec = (int)(second->tv_nsec/1000) - (int)(first->tv_nsec/1000);

    //le temps passé en milisec
    return (1000*sec + (int)(usec/1000));
}

//renvoie le nombre de microsecondes passées entre first et second
long int getMicrosec(TIMESTRUCT* first, TIMESTRUCT* second)
{
    int sec, usec;

    //le nombre de secondes passées (est généralement 0)
    sec = second->tv_sec - first->tv_sec;

    //le nombre de microsec passées (géneralement négatif quand sec = 1)
    usec = (int)(second->tv_nsec/1000) - (int)(first->tv_nsec/1000);

    //le temps passé en microsec
    return ( (1e6 * sec) + usec);
}

//ajoute (ou enlève) un certain nombre de seconde changeTime
void addSec(TIMESTRUCT* changeTime, int howMuch)
{
    changeTime->tv_sec += howMuch;
}


#endif // _WINDOWS
