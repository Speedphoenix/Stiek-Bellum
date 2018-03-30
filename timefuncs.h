#ifndef TIMEFUNCS_H_INCLUDED
#define TIMEFUNCS_H_INCLUDED

/*
    Ce fichier contient les prototypes des sous programmes utilisés pour les mesures du temps
    Adaptés en fonction du système d'exploitation

*/

#include <stdio.h>
#include <time.h>

#ifdef _WINDOWS

// the struct that should be passed as param of the functions here
#define TIMESTRUCT dunno

#else

// the struct that should be passed as param of the functions here
#define TIMESTRUCT struct timespec

#endif // _WINDOWS

///À faire, une fonction isElapsed

///prend la valeur temps maintenant
void getTime(TIMESTRUCT* watch);

///renvoie le nombre de secondes passées entre first et second
int getSecInt(TIMESTRUCT* first, TIMESTRUCT* second);

///renvoie le nombre de secondes passées entre first et second
double getSec(TIMESTRUCT* first, TIMESTRUCT* second);

///renvoie le nombre de milisecondes passées entre first et second
int getMilisec(TIMESTRUCT* first, TIMESTRUCT* second);

///renvoie le nombre de microsecondes passées entre first et second
long int getMicrosec(TIMESTRUCT* first, TIMESTRUCT* second);

///ajoute (ou enlève) un certain nombre de seconde changeTime
void addSec(TIMESTRUCT* changeTime, int howMuch);

#endif // TIMEFUNCS_H_INCLUDED

