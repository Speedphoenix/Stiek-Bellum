#include "chaine_b.h"

/*
    UNE GRANDE PARTIE DES SOUS-PROGRAMMES DE CE FICHIER ONT ÉTÉ RECICLÉS DU MINI PROJET TP COLLECTION SUR DREAM THEATER (par Leonardo Jeanteur)

    ce fichier contient les sous programmes necessaires à la manipulation des maillons
*/

//pour ajouter un maillon au début de la chaine
void ajout_debut_b(Ancre_b *ancre, Build *bat)
{
    Maillon_b *nouveau, *inter = ancre->debut; //on garde l'adresse de l'ancien premier maillon

    ancre->debut = nouveau = (Maillon_b*)malloc(sizeof(Maillon_b)); //on cree le nouveau premier maillon

    nouveau->batiment = bat; //on y met le pointeur

    nouveau->next = inter; //on y met l'adresse de l'ancien premier maillon
    nouveau->prev = NULL;

    if (inter!=NULL)
        inter->prev = nouveau;
    else
        ancre->fin = nouveau; //si la chaine était vide
}

//pour supprimer le maillon en paramètre
void supprimer_b(Ancre_b *ancre, Maillon_b *enlev, int del)
{
    Maillon_b *prev, *next;
    if (enlev!=NULL)
    {
        prev = enlev->prev;
        next = enlev->next;

        //on met les adresses des maillons à proximité
        if (next==NULL) //si on est à l'ancre
            ancre->fin = prev;
        else
            next->prev = prev;

        if (prev==NULL)
            ancre->debut = next;
        else
            prev->next = next;

        //maintenant aucun maillon ne pointe sur enlev

        //on libère l'éspace mémoire
        if (del)
            free(enlev->batiment);

        free(enlev);
    } //maintenant enlev n'existe plus
}

//libere l'éspace mémoire de tous les maillons
void libere_b(Ancre_b *ancre, int del)
{
    Maillon_b *inter2, *inter1 = ancre->debut;

    while (inter1!=NULL)
    {
        inter2 = inter1->next;
        if (del)
            free(inter1->batiment);
        free(inter1);
        inter1 = inter2;
    }
    ancre->debut = NULL;
    ancre->fin = NULL;
}

//renvoie le nombre de maillons dans la chaine
int taille_b(Ancre_b ancre)
{
    int rep = 0;
    Maillon_b *inter = ancre.debut;

    while (inter!=NULL) //on passe par chaque maillon
    {
        inter = inter->next;
        rep++;
    }
    return rep;
}


//fin du fichier
