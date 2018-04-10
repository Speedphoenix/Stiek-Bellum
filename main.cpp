#include "menu.h"


/**     Trucs à faire:

  * Artem   - des macros pour le brouillard

  *        !- Optimiser le code
  *        !- tourner autours d'une ressource/batiment pour optimiser le nombre de paysans
  *        µ- faire apparaitre de plus en plus d'ennemis en de plus en plus proches du joueur au fur et à mesure de la partie
  *         - limite du nombre d'unités (par taille)
  *         - faire disparaitre les ennemis de la minimap (et de l'écran) quand ils sont loin d'unités alliées
  *         - faire pour mettre des batiments sur du sable
  *        !- enlever les allegro_message pour la defaite et quitter le jeu
  *         - mode plein écran
  *         - faire l'attaque des batiments

  *         - pathfinding : faire s'arreter les unités quand elles sont bloquées, résoudre les bugs
  *         - pathfinding : prendre en compte le creep block


  *         - Faire une fonction isElapsed...

  *         - dans load.c ligne 256 Joueur par addresse svp...
  */


/**     trucs dont il manque le dessin
  *         - Message de victoire defaite, aurevoir
  *         - Des petits trucs en plus (d'autres fleurs aléatoires, caillous, nenuphars, etc)
  *         - Le menu principal - faire le fond en moins "cheap" (couleurs plus accentuées, peut etre pas sur du papier à careaux?)
  *         - une unité en plus?
  */



using namespace std;


///renvoie la valeur de victoire
int victory_check(const Joueur& joueur);

///Remplace le rest: fait attendre en prenant en compte le temps écoulé depuis le dernier tour de boucle
void attente(TIMESTRUCT& prev);


int main()
{
    //les structures qui resteront jusqu'au bout:

    Tile carte[MAPSIZEX][MAPSIZEY]; //la map: un tableau de tuiles géant
    list<Unit *> ancre;             //La liste chainée contenant toutes les unités (alliées ET ennemies)
    list<Build *> ancre_b;          //La liste chainée contenant tous les batiments (alliés ET ennemis)
    Joueur joueur;                  //la structure avec toutes les valeurs "globales" (position de la camera, ressources...)
    Sprites sprites;                //La structure contenant toutes les bitmap
    BITMAP *buffer;                 //Le buffer de l'écran

    TIMESTRUCT prev;                //on garde le temps à chaque tour de boucle (pour la temporisation)
    int choix, mapchoice, diff = -1;      //valeurs de retours du menu
    int finished;                   //la valeur de vicoire (0 si la partie est en cour, 1 si gagne, -1 si perdu)

    init_alleg(XSCREEN, YSCREEN);

    buffer = create_bitmap(XSCREEN, YSCREEN);

    //on load les bitmaps depuis les fichiers
    load_sprites(sprites);
    joueur.langue = ENGLISH;


    do //la boucle générale, revient au début quand la partie est terminé
    {
        mapchoice = 0;
        do
        {

            choix = menu(sprites, joueur, MAIN_MENU);

            if (choix!=EXIT)
            {

                switch (choix)
                {
                    default:
                    case NEWGAME:
                    do
                    {
                        mapchoice = menu(sprites, joueur, MAP_MENU);
                        if (mapchoice)
                        {
                            diff = menu(sprites, joueur, DIFF_MENU);
                        }

                    }while (mapchoice!=0 && diff==0);

                break;

                    case LOAD:
                    mapchoice = menu(sprites, joueur, LOAD_MENU);
                    if (mapchoice)
                        mapchoice+=3;

                break;
                }
            }

        }while (choix!=EXIT && mapchoice==0);


        //on remet les valeurs aux valeurs initiales (discutablement inutile)
        reset(ancre, ancre_b, joueur, carte);


        if (choix!=EXIT)
        {
            //on charge la map
            load_game(carte, ancre, ancre_b, joueur, mapchoice);


            if (choix==NEWGAME)
                joueur.level = diff;

            finished = 0;

            while (!finished && !joueur.quit) //la boucle de jeu
            {
                //check des action du joueur (de la souris)
                action(ancre, ancre_b, joueur, carte);

                //mise à jour du terrain de jeu (faire avancer toutes les unités, attaque etc)
                update(carte, ancre, ancre_b, joueur);

                //affichage de tout l'écran sur le buffer
                draw_screen(buffer, ancre, ancre_b, carte, sprites, joueur);
                blit(buffer, screen, 0, 0, 0, 0, XSCREEN, YSCREEN);

                if (joueur.pause)
                    pause_game(joueur, sprites, carte, ancre, ancre_b);

                finished = victory_check(joueur);

                //temporisation de la boucle de jeu
                attente(prev);
            }

            if (finished==1)
            {
                end_game(sprites, joueur);
            }
            else if (finished==-1)
            {
                switch (joueur.langue) ///À CHANGER!!!
                {
                    default:
                    case ENGLISH:
                    allegro_message("You lost!!"); ///À CHANGER
                break;

                    case FRANCAIS:
                    allegro_message("Tu as perdu!!"); ///À CHANGER
                break;

                    case ITALIANO:
                    allegro_message("Hai perso!!"); ///À CHANGER
                break;
                }
            }



            if (TEST && key[KEY_ESC]) //on garede la partie dans le deuxième fichier de sauvegarde (pour tester)
                save_game(carte, ancre, ancre_b, joueur, 2);

            reset(ancre, ancre_b, joueur, carte);
        }
        else
        {
            //exit message
            //allegro_message((joueur.langue==ENGLISH)?"Good bye":(joueur.langue==FRANCAIS)?"Au revoir":"Arrivederci"); ///ÀCHANGER
        }

    //on revient au menu
    } while (choix!=EXIT);

    return 0;
}
END_OF_MAIN();

//renvoie la valeur de victoire
int victory_check(const Joueur& joueur)
{
    if (joueur.nend_b>0)
        return 0;
    else if (joueur.nend_b==-1)
        return -1;
    else if (joueur.level==EASY)
        return 1;
    else if (joueur.level==HARD || joueur.level==HARDCORE)
    {
        if (joueur.nend_e)
            return 0;
        else
            return 1;
    }
    return 1;
}

//pour remplacer le rest, on veut compenser pour d'eventuels gros calculs
void attente(TIMESTRUCT& prev)
{
    TIMESTRUCT nouveau;
    int elapsed; //elapsed est en milisec

    getTime(nouveau);

    //le temps passé en milisec
    elapsed = getMilisec(prev, nouveau);

    if (HOW_LONG)
        fprintf(stderr, "total: %d\n\n", elapsed);

    if (elapsed<LAPSE) //si le temps écoulé est inferieur à la temporisation
        rest(LAPSE-elapsed);

    //on prend le nouveau temps maintenant pour ne pas prendre en compte les ptits calcus d'avant
    getTime(prev);
}


//fin du fichier
