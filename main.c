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





///renvoie la valeur de victoire
int victory_check(Joueur joueur);

///Remplace le rest: fait attendre en prenant en compte le temps écoulé depuis le dernier tour de boucle
void attente(TIMESTRUCT *prev);


int main()
{
    //les structures qui resteront jusqu'au bout:

    Tile carte[MAPSIZEX][MAPSIZEY]; //la map: un tableau de tuiles géant
    Ancre ancre = NANCRE;           //La liste chainée contenant toutes les unités (alliées ET ennemies)
    Ancre_b ancre_b = NANCRE;       //La liste chainée contenant tous les batiments (alliés ET ennemis)
    Joueur joueur;                  //la structure avec toutes les valeurs "globales" (position de la camera, ressources...)
    Sprites sprites;                //La structure contenant toutes les bitmaps
    BITMAP *buffer;                 //Le buffer de l'écran

    TIMESTRUCT prev;                //on garde le temps à chaque tour de boucle (pour la temporisation)
    int choix, map, diff = -1;      //valeurs de retours du menu
    int end;                        //la valeur de vicoire (0 si la partie est en cour, 1 si gagne, -1 si perdu)

    init_alleg(XSCREEN, YSCREEN);

    buffer = create_bitmap(XSCREEN, YSCREEN);

    //on load les bitmaps depuis les fichiers
    load_sprites(&sprites);
    joueur.langue = ENGLISH;


    do //la boucle générale, revient au début quand la partie est terminé
    {
        map = 0;
        do
        {
            DEB("0")

            choix = menu(sprites, &joueur, MAIN_MENU);
            DEB("0-0")

            if (choix!=EXIT)
            {
                DEB("0-1")

                switch (choix)
                {
                    default:
                    case NEWGAME:
                    do
                    {
                        map = menu(sprites, &joueur, MAP_MENU);
                        if (map)
                        {
                            diff = menu(sprites, &joueur, DIFF_MENU);
                        }
                        DEB("0-2")
                    }while (map!=0 && diff==0);
                    DEB("0-3")

                break;

                    case LOAD:
                    map = menu(sprites, &joueur, LOAD_MENU);
                    if (map)
                        map+=3;

                break;
                }
            }

        }while (choix!=EXIT && map==0);

        DEB("0-4")

        //on remet les valeurs aux valeurs initiales (discutablement inutile)
        reset(&ancre, &ancre_b, &joueur, carte);

        DEB("0-5")

        if (choix!=EXIT)
        {
            //on charge la map
            load_game(carte, &ancre, &ancre_b, &joueur, map);

            DEB("0-6")

            if (choix==NEWGAME)
                joueur.level = diff;

            end = 0;

            while (!end && !joueur.quit) //la boucle de jeu
            {
                DEB("1")
                //check des action du joueur (de la souris)
                action(&ancre, &ancre_b, &joueur, carte);
                DEB("2")
                //mise à jour du terrain de jeu (faire avancer toutes les unités, attaque etc)
                update(carte, &ancre, &ancre_b, &joueur);
                DEB("3")
                //affichage de tout l'écran sur le buffer
                draw_screen(buffer, ancre, ancre_b, carte, &sprites, &joueur);
                blit(buffer, screen, 0, 0, 0, 0, XSCREEN, YSCREEN);
                DEB("4")
                if (joueur.pause)
                    pause_game(&joueur, sprites, carte, ancre, ancre_b);
                DEB("5")
                end = victory_check(joueur);
                DEB("6")
                //temporisation de la boucle de jeu
                attente(&prev);
            }

            if (end==1)
            {
                DEB("7")
                end_game(sprites, &joueur);
            }
            else if (end==-1)
            {
                DEB("8-0")
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

            DEB("8-1")


            if (TEST && key[KEY_ESC]) //on garede la partie dans le deuxième fichier de sauvegarde (pour tester)
                save_game(carte, ancre, ancre_b, joueur, 2);
            DEB("8-2")
            reset(&ancre, &ancre_b, &joueur, carte);
            DEB("8-3")
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
int victory_check(Joueur joueur)
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
void attente(TIMESTRUCT *prev)
{
    TIMESTRUCT nouveau;
    int elapsed; //elapsed est en milisec

    getTime(&nouveau);

    //le temps passé en milisec
    elapsed = getMilisec(prev, &nouveau);

    if (HOW_LONG)
        fprintf(stderr, "total: %d\n\n", elapsed);

    if (elapsed<LAPSE) //si le temps écoulé est inferieur à la temporisation
        rest(LAPSE-elapsed);

    //on prend le nouveau temps maintenant pour ne pas prendre en compte les ptits calcus d'avant
    getTime(prev);
}


//fin du fichier
