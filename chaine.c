#include "chaine.h"

/*
    UNE GRANDE PARTIE DES SOUS-PROGRAMMES DE CE FICHIER ONT ÉTÉ RECICLÉS DU MINI PROJET TP COLLECTION SUR DREAM THEATER (par Leonardo Jeanteur)

    ce fichier contient les sous programmes necessaires à la manipulation des maillons
*/

//pour ajouter un maillon au début de la chaine
void ajout_debut(Ancre *ancre, Unit *unite)
{
    Maillon *nouveau, *inter = ancre->debut; //on garde l'adresse de l'ancien premier maillon

    ancre->debut = nouveau = (Maillon*)malloc(sizeof(Maillon)); //on cree le nouveau premier maillon

    nouveau->unite = unite; //on y met le pointeur

    nouveau->next = inter; //on y met l'adresse de l'ancien premier maillon
    nouveau->prev = NULL;

    if (inter!=NULL)
        inter->prev = nouveau;
    else
        ancre->fin = nouveau; //si la chaine était vide
}

//pour ajouter un maillon à la fin de la chaine
void ajout_fin(Ancre *ancre, Unit *unite)
{
    Maillon *nouveau, *inter = ancre->fin; //on garde l'adresse de l'ancien premier maillon

    ancre->fin = nouveau = (Maillon*)malloc(sizeof(Maillon)); //on cree le nouveau premier maillon

    nouveau->unite = unite; //on y met les bonnes infos

    nouveau->next = NULL; //on y met l'adresse de l'ancien premier maillon
    nouveau->prev = inter;


    if  (inter!=NULL)
        inter->next = nouveau;
    else
        ancre->debut = nouveau; //si la chaine était vide
}

//pour ajouter un maillon entre position where et where+1. Si la chaine est plus petite ça s'ajoute en fin de chaine
void ajout_entre(Ancre *ancre, int where, Unit *unite)
{
    Maillon *inter1, *inter2, *nouve; //nouv est le nouveau maillon créé, inter1 est le maillon précédent, inter2 est le maillon successif
    int i=1;

    nouve = (Maillon*)malloc(sizeof(Maillon));

    if (ancre->debut==NULL) //si la chaine est vide
    {
        ancre->fin = ancre->debut = nouve; //on cree le maillon à l'ancre

        nouve->unite = unite; //on met les valeurs

        nouve->next = NULL; //le maillon est unique
        nouve->prev = NULL;

    }
    else //si la chaine n'est pas vide
    {
        if (where==0) //si le maillon est à insérer au début
        {
            inter2 = ancre->debut;
            inter1 = NULL;
        }
        else
        {
            inter1 = ancre->debut;

            while (inter1->next!=NULL && i!=where) //on va jusqu'à where
            {
                inter1 = inter1->next;
                i++;
            }

            inter2 = inter1->next;
            inter1->next = nouve; //on cree le maillon
        }



        nouve->unite = unite;
        nouve->prev = inter1;
        nouve->next = inter2;

        if (where==0)
            ancre->debut = nouve;
        if (inter2==NULL)
            ancre->fin = nouve;
        else
            inter2->prev = nouve;

    }
}

//pour supprimer le maillon en paramètre
void supprimer(Ancre *ancre, Maillon *enlev, int del)
{
    Maillon *prev, *next;
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
            free(enlev->unite);

        free(enlev);
    } //maintenant enlev n'existe plus
}

//pour chercher le numero d'une unite à partir de l'ancre
Maillon *recherche_num(Ancre ancre, int num, int type)
{
    Maillon *rep = NULL, *inter = ancre.debut;

    int entier = 0; //l'entier que l'on va tester à chaque fois

    while (inter!=NULL) //tant qu'on est pas au dernier maillon ou qu'on s'arrete
    {
        switch (type) //on prend la valeur à tester
        {
            default:
            entier = inter->unite->x;
        break;
        }

        if (entier==num) //si c'est les mêmes
        {
            rep = inter;
            inter = NULL; //on sort de la boucle
        }
        else
            inter = inter->next; //on passe au maillon suivant
    }

    return rep;
}

//libere l'éspace mémoire de tous les maillons
void libere(Ancre *ancre, int del)
{
    Maillon *inter2, *inter1 = ancre->debut;

    while (inter1!=NULL)
    {
        inter2 = inter1->next;
        if (del)
            free(inter1->unite);
        free(inter1);
        inter1 = inter2;
    }
    ancre->debut = NULL;
    ancre->fin = NULL;

}

//renvoie le nombre de maillons dans la chaine
int taille(Ancre ancre)
{
    int rep = 0;
    Maillon *inter = ancre.debut;

    while (inter!=NULL) //on passe par chaque maillon
    {
        inter = inter->next;
        rep++;
    }
    return rep;
}

//fin du fichier
