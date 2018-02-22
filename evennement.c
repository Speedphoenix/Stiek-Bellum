#include "evennement.h"

/*
    Ce fichier contient les fonctions montrant un evenement spécifique
    Une action du joueur par exemple
*/

///changer la vitesse de déplacement de la camera en fonction du zoom
///faire le zoom vers la souris et non pas au centre de l'ecran


//crée une nouvelle unité et l'ajoute à la liste chainée
void add_unit(Ancre *ancre, int type, int priority, int x, int y, Build *bat)
{
    Unit *rep = malloc(sizeof(Unit));

    rep->x = x;
    rep->y = y;
    rep->xdest = x; //l'unité est immobile à la création
    rep->ydest = y;
    rep->xpath = DIV(x);
    rep->ypath = DIV(y);
    rep->changepath = 1;
    rep->step = NULL;

    rep->direction = RIGHT; //par défaud une unité regarde à droite
    rep->cote = 75;         //pour l'instant toutes les unités ont la même taille
    rep->unitsize = 1;

    rep->frame = 0;         //la première image de l'animation (immobile)
    rep->state = MOVING;    //immobile
    rep->prec = MOVING;     //la serie d'image d'où predre la bitmap
    rep->priority = priority;  //priorité la plus basse
    rep->bat = bat;         //le batiment à pointer (si aplicable)

    switch (type)
    {
        case SOLDIER:
        rep->hp = HP_SOLDIER;
        rep->hp_max = HP_SOLDIER;
        rep->damage = 20;   //les degats de cette unité
        rep->side = ALLY;
        rep->type = SOLDIER;


        rep->prod = 0;      //ne peut pas produire de ressources
        rep->speed = 8*SCALE;     //vitesse de deplacement (pixels par tour de boucle)
        rep->range = 80;    //portée d'attaque
        rep->delay = 2.0;   //temps d'attente entre chaque attaque
        rep->vision = 4;    //zone éclairée autours de cette unité
    break;

        case PEASANT:
        rep->hp = HP_PEASANT;
        rep->hp_max = HP_PEASANT;
        rep->damage = 10;
        rep->side = ALLY;
        rep->type = PEASANT;


        rep->prod = 1;      // peut produire des ressources
        rep->speed = 6*SCALE;
        rep->range = 80;
        rep->delay = 2.0;
        rep->vision = 4;
    break;

        case ENEMY:
        rep->hp = HP_ENEMY;
        rep->hp_max = HP_ENEMY;
        rep->damage = 20;
        rep->side = ENEMY;
        rep->type = ENEMY;


        rep->prod = 0;
        rep->speed = 10*SCALE;
        rep->range = 80;
        rep->delay = 2.0;
        rep->vision = 4;
    break;

        default:
    break;
    }

    //on initialise les temps pour l'attaque et l'animation
    clock_gettime(CLOCK_MONOTONIC, &rep->since_a);
    clock_gettime(CLOCK_MONOTONIC, &rep->since_w);

    ajout_debut(ancre, rep); //on ajoute l'unité à la liste chainée
}

//crée un nouveau batiment et renvoie son pointeur
Build *nouv_batiment(int x, int y, int w, int h, int side, int state)
{
    Build *rep = malloc(sizeof(Build));

    rep->x = x;
    rep->y = y;
    rep->w = w; //width height
    rep->h = h;

    rep->side = side;
    rep->state = state; //l'état du batiment (normal, en construction..)
    rep->hp_max = HP_BUILD_S;
    rep->hp = state?10:rep->hp_max; //si le batiment commence à être construit il recevra les pv au fur et à mesure de la construction
    rep->range = 150; //la portée d'attaque du batiment (ne sert pour l'instant à rien)
    rep->damage = 40; //les dégats du batiment (ne sert pour l'instant à rien)
    rep->cap = side?0:5;
    rep->statione = 0;

    rep->curr_queue = 0; //il n'y a aucune unité en cours de formation dans ce batiment

    return rep;
}

//verifie si on peut ajouter le batiment à ces coordonnées et le place et l'ajoute à la liste chainée (renvoie 0 sinon)
int add_bat(Ancre_b *ancre_b, Ancre ancre, Tile carte[MAPSIZEX][MAPSIZEY], int x, int y, int state, int type)
{
    int i, j;
    int rep = 0;
    Build *nouv;

    if (!carte[x][y].block && !trouve(ancre, x*COTE + COTE/2, y*COTE + COTE/2, NULL, NEUTR) &&         // si ce n'est pas une case pleine ou s'il n'y a pas d'unité là...
        !carte[x+1][y].block && !trouve(ancre, (x+1)*COTE + COTE/2, y*COTE + COTE/2, NULL, NEUTR) &&   //s'il y a de la place dans les quatres cases du batiment
        !carte[x][y+1].block && !trouve(ancre, x*COTE + COTE/2, (y+1)*COTE + COTE/2, NULL, NEUTR) &&   //pourrait être fait en mode DRY mais bon
        !carte[x+1][y+1].block && !trouve(ancre, (x+1)*COTE + COTE/2, (y+1)*COTE + COTE/2, NULL, NEUTR))
    {
        rep = 1; //y'a de la place/ça a marché
        nouv = nouv_batiment(x, y, 2, 2, ALLY, state);

        for (i=0;i<2;i++)
        {
            for (j=0;j<2;j++) //on met des nouvelles valeurs sur les cases de la map
            {
                carte[x+i][y+j].type = BUILDING;
                carte[x+i][y+j].position = 4*type + (i+ 2*j);   //partie hau-gauche, haut droit etc du batiment
                carte[x+i][y+j].res = 0;                        //pas de ressources à extraire
                carte[x+i][y+j].block = SOLID;                  //les unités ne peuvent pas passer par là
                carte[x+i][y+j].erige = nouv;                   //on y met le pointeur du batiment
                carte[x+i][y+j].visible = 1;                    //cette fonction n'est appelée que pour des batiments alliés (pour l'instant)
                carte[x+i][y+j].fleur = 0;
            }
        }
        ajout_debut_b(ancre_b, nouv); //on ajoute le batiment à la liste chainée

        //on éclaire la zone
        eclaire(carte, (x+1)*COTE, (y+1)*COTE, 4);
    }

    return rep;
}

//detruit le batiment
void destroy_build(Ancre_b *ancre, Maillon_b *del, Tile carte[MAPSIZEX][MAPSIZEY])
{
    int i, j;
    Build *build = del->batiment;
    int x = build->x;
    int y = build->y;

    for (i=0;i<build->w;i++) //on enleve le batiment de la carte
    {
        for (j=0;j<build->h;j++)
        {
            carte[x+i][y+j].res = 0;
            carte[x+i][y+j].block = 0;
            carte[x+i][y+j].type = GRASS;
            carte[x+i][y+j].position = P_HERBE;
            carte[x+i][y+j].erige = NULL;
        }
    }
    //on l'enleve de la liste chainée
    supprimer_b(ancre, del, 1);
}

//prend les actions du joueur (action avec la souris)
void action(Ancre *ancre, Ancre_b *ancre_b, Joueur *joueur, Tile carte[MAPSIZEX][MAPSIZEY])
{
    Maillon *inter1 = joueur->selection.debut, *inter2;

    int valx = 0, valy = 0, val = 0;

    int x = mouse_x; //on prend les valeurs de la souris
    int y = mouse_y; //cette valeur ne doit pas changer


    if (key[KEY_P]) //s'il faut mettre le menu pause
        joueur->pause = 1;

    DEB("1-0-0")
    //on enleve toutes les unités qui auraient pu mourrir de la selection
    if (joueur->act==SELECTED || joueur->act==PLACE_BUILD)
    {
        while(inter1!=NULL) //on regarde chaque unité de la selection
        {
            inter2 = inter1->next; //on prend maintenant le maillon suivant (si jamais on a suprimé le courant après)

            if (inter1->unite->state == DEAD)
                supprimer(&joueur->selection, inter1, 0); //on l'enlève de la selection
            else if (inter1->unite->prod) //si il reste une unité qui peut construire un batiment
                val++;

            inter1 = inter2;
        }

        if (joueur->selection.debut==NULL) //si la séléction s'est vidée entre temps
        {
            joueur->act = RIEN;
            joueur->type = 0;
        }
        else if (joueur->act==PLACE_BUILD && !val) //si la selection ne s'est pas vidée mais que les constructeurs sont morts, on ne peut plus placer le batiment)
        {
            joueur->act = SELECTED;
            joueur->type = 0;
        }

    }
    DEB("1-0-1")
    val = mouse_z -joueur->prevwheel; //on prend combien le joueur a tourné la roulette de la souris entre temps

    if (abs(val)>=3) //si ça dépasse la limite pour zoom/dezoom
    {
        if (val>=3) //s'il faut zoomer (positif)
        {
            if (joueur->xecran>MINECRANX) //si on est pas aux limites de (dé)zoom
                joueur->xecran-=2;  //de combien on incrémente le zoom
            if (joueur->yecran>MINECRANY)
                joueur->yecran--;
        }
        else //s'il faut dézoomer (négatif)
        {
            if (joueur->xecran<MAXECRANX)
                joueur->xecran+=2;
            if (joueur->yecran<MAXECRANY)
                joueur->yecran++;
        }

        joueur->change = 1;
        joueur->chang_taill = 1;
        joueur->prevwheel = mouse_z; //on garde la nouvelle valeur de la roulette de la souris
    }
    DEB("1-0-2")
    //s'il faut déplacer la caméra: on check si la souris est aux bords de l'écran et que la caméra n'est pas au bord de la map
    if (((x<=MOVELIMIT && joueur->xcamera>0) || (x>=(XSCREEN - MOVELIMIT) && (joueur->xcamera<(MAPSIZEX - joueur->xecran)*COTE)) ||
        (y<=MOVELIMIT && joueur->ycamera>0) || (y>=(YSCREEN - MOVELIMIT) && (joueur->ycamera<(MAPSIZEY - joueur->yecran)*COTE))))
    {
        if (x<=MOVELIMIT && joueur->xcamera>0) //en abscisse
            valx = x - MOVELIMIT;               //negatif
        else if (x>=(XSCREEN - MOVELIMIT))
            valx = MOVELIMIT - (XSCREEN-x);     //positif

        if (y<=MOVELIMIT && joueur->ycamera>0) //en ordonnée
            valy = y - MOVELIMIT;               //negatif
        else if (y>=(YSCREEN - MOVELIMIT))
            valy = MOVELIMIT - (YSCREEN-y);     //positif

        joueur->xcamera+=valx; //on bouge la caméra
        joueur->ycamera+=valy;

        joueur->change = 1;

        if (joueur->xcamera<0)
            joueur->xcamera = 0;
        else if (joueur->xcamera>(MAPSIZEX - joueur->xecran)*COTE)
            joueur->xcamera = (MAPSIZEX-joueur->xecran)*COTE;
                                                    //si on a dépassé l'ecran avec la caméra
        if (joueur->ycamera<0)
            joueur->ycamera = 0;
        else if (joueur->ycamera>(MAPSIZEY - joueur->yecran)*COTE)
            joueur->ycamera = (MAPSIZEY-joueur->yecran)*COTE;
    }
    DEB("1-0-3")
    //on appelle les programmes d'actions "actifs"
    if (y<=(ECRANY*COTE) || joueur->act==SELECTING) //on ne veut pas interrompre la selection si on va sur l'ui
        action_ecran(*ancre, ancre_b, joueur, carte, x, y); //action avec la souris sur l'écran de jeu
    else if (y<(YSCREEN-MOVELIMIT) && x>MOVELIMIT)
        action_ui(ancre, joueur, carte, x, y);      //action avec la souris en bas de l'écran, dans l'UI
}

//prend les actions du joueur sur le terrain de jeu (la map)
void action_ecran(Ancre ancre, Ancre_b *ancre_b, Joueur *joueur, Tile carte[MAPSIZEX][MAPSIZEY], int x, int y)
{
    Maillon *inter = NULL;
    Build *bat;
    struct timespec now;
    double elapsed;

    x = x*((float)joueur->xecran/ECRANX);
    y = y*((float)joueur->yecran/ECRANY);

    x += joueur->xcamera;
    y += joueur->ycamera; //pour avoir les coordonnées dans le referrentiel de la map

    if (TEST)
        printf("%d %d\n", DIV(x), DIV(y)); ///POUR TESTER, pour voir sur quelle tuile est la souris
    DEB("1-1-0")
    if (mouse_b & 2) //dans le cas click droit
    {
        switch (joueur->act) //qu'elle action le joueur etait en train de faire
        {
            default:
            case SELECT_BUILD:  //il se passe rien...
            case RIEN:          //il se pass rien...

        break;

            case SELECTING:     //annule l'action en cours
            joueur->act = RIEN;
            joueur->type = 0;
        break;

            case SELECTED:      //on donne l'ordre de se déplacer
            DEB("1-1-1")
            if (joueur->clic_prec!=2) //que si le joueur n'avait pas déjà clique droit (les unités ne suivent pas la souris)
            {
                inter = joueur->selection.debut;

                while (inter!=NULL) //on prend chaque unité de la selection
                {
                    if (inter->unite->priority<=ORDRE) //si la priorité est inferieure
                    {
                        inter->unite->xdest = x; //on met les nouvelles coordonnées dans le GPS
                        inter->unite->ydest = y;
                        inter->unite->state = MOVING;
                        inter->unite->priority = ORDRE;
                        inter->unite->prec = MOVING;
                        inter->unite->changepath = 1;
                    }

                    inter = inter->next;
                }
            }

        break;

            case PLACE_BUILD:   //revenir à la selection des unités
            joueur->type = 0;
            joueur->act = SELECTED;
        break;
        }

        //on enregistre le dernier click
        joueur->clic_prec = 2;
    }
    else if (mouse_b & 1) //clique gauche
    {
        switch (joueur->act)
        {
            case SELECTED:
            if (joueur->clic_prec) //si c'est c'est pas que le joueur a appuyé trop longtemps
                break;
            else
                libere(&joueur->selection, 0); //on libere la selection sans detruire les unités

            default:            //on prend la selection
            case SELECT_BUILD:
            case RIEN:
            if (joueur->clic_prec!=1) //si par example on a commencé à cliquer sur l'ui...
            {
                joueur->xprev = x; //on garde les coordonnées initiales de la selection
                joueur->yprev = y;

                joueur->act = SELECTING; //on passe en mode selection
            }

        break;

            case SELECTING:     //il se passe rien...
        break;

            //on voit si on peut valider l'action ou non + place le batiment
            case PLACE_BUILD:
            if (!joueur->clic_prec) //que si le joueur n'était pas déjà en train de cliquer
            {
                DEB("1-1-2")
                if (add_bat(ancre_b, ancre, carte, DIV(x), DIV(y), PROGRESS, joueur->type)) //si le batiment peut être placé
                {
                    joueur->bois -= joueur->xprev;
                    joueur->marbre -= joueur->yprev; //on fait la transaction
                    joueur->act = SELECTED;          //on revient à l'état de selection
                    joueur->change = 1;

                    inter = joueur->selection.debut; //on deplace tous les paysants dans cette direction
                    while (inter!=NULL)
                    {
                        if (inter->unite->priority<=ORDRE && inter->unite->prod)
                        {
                            inter->unite->xdest = x;
                            inter->unite->ydest = y;
                            inter->unite->state = MOVING;
                            inter->unite->prec = MOVING;
                            inter->unite->priority = ORDRE;
                            inter->unite->changepath = 1;
                        }

                        inter = inter->next;
                    }
                }
            }
        break;
        }

        // on enregistre le cick
        joueur->clic_prec = 1;
    }
    else //si aucun bouton de la souris n'est appuyé
    {
        switch (joueur->act)
        {
            default:
            case SELECTED:      //il se passe rien...
            case SELECT_BUILD:  //il se passe rien...
            case RIEN:          //il se passe rien...

        break;

            case SELECTING:     //on arrete la selection et on valide/selectionne
            DEB("1-1-3")
                //s'il faut prendre un batiment
            bat = carte[DIV(x)][DIV(y)].erige;
            if (DIV(x)==DIV(joueur->xprev) && DIV(y)==DIV(joueur->yprev) && //si la souris n'a pas changé de case
                (bat && (bat->hp==bat->hp_max && bat->side==ALLY)))         //s'il y a un batiment allié à utiliser
            {                                                               //et si ce batiment est utilisable
                joueur->xprev = bat->x; //on prend les coordonnées du batiment
                joueur->yprev = bat->y;
                joueur->act = SELECT_BUILD; //on passe en mode batiment selectionné
                joueur->type = (int)(carte[DIV(x)][DIV(y)].position/4); //on garde le type du batiment (mairie ou caserne)
            }
            else
            {
                clock_gettime(CLOCK_MONOTONIC, &now);
                elapsed = (now.tv_sec - joueur->last_clic.tv_sec) + 1e-9 * (now.tv_nsec - joueur->last_clic.tv_nsec);

                if (elapsed<=0.5) //si y'a double clic
                {
                    selec(&joueur->selection, ancre, joueur->xcamera, joueur->ycamera, joueur->xcamera + COTE*joueur->xecran, joueur->ycamera + COTE*joueur->yecran);
                }
                else
                {
                    //on selectione les unités presentes dans le carré de selection
                    selec(&joueur->selection, ancre, joueur->xprev, joueur->yprev, x, y);
                }

                if (joueur->selection.debut!=NULL) //si la selection n'est pas vide
                {
                    joueur->act = SELECTED;
                }
                else //si elle est vide
                {
                    joueur->act = RIEN;
                }
            }

        break;

            case PLACE_BUILD:   //on maintient le truc à déplacer
        break;
        }

        if (joueur->clic_prec==1)
            clock_gettime(CLOCK_MONOTONIC, &joueur->last_clic);

        joueur->clic_prec = 0; //on garde le fait que le joueur n'a pas cliqué
    }
    DEB("1-1-4")
}

//prend les actions du joueur sur l'UI
void action_ui(Ancre *ancre, Joueur *joueur, Tile carte[MAPSIZEX][MAPSIZEY], int x, int y)
{
    Maillon *inter = NULL;
    Build *bat;
    int xval, yval, p, clk, bois = 0, pierre = 0, uni = 0; //quand besoin est de variables

    //on se place dans le refferentiel de l'UI
    y -= ECRANY*COTE; //ce sous-programme n'est appelé que si la souris est sur l'UI (la partie basse de l'écran)
    DEB("1-2-0")
    if (x<(MAPWIDTH+MOVELIMIT)) //si la souris est sur la minimap
    {
        DEB("1-2-1")
        x -= MOVELIMIT; //on ignore la partie à gauche de la minimap

        if (mouse_b & 1) //si clique gauche
        {
            DEB("1-2-2")
            xval = x - (joueur->xecran/2); //on fait que la souris soit au centre du recltangle de l'écran sur la minimap
            yval = y - (joueur->yecran/2);

            if (xval<0)
                xval = 0;
            else if (xval>(MAPSIZEX - joueur->xecran))
                xval = MAPSIZEX - joueur->xecran;
                                    //si l'écran sortirait de la map
            if (yval<0)
                yval = 0;
            else if (yval>(MAPSIZEY - joueur->yecran))
                yval = MAPSIZEY - joueur->yecran;

            joueur->xcamera = xval * COTE; //on prend les nouvelles valeurs pour les coordonnées de la caméra
            joueur->ycamera = yval * COTE;
            joueur->change = 1;

        }
        if (mouse_b & 2) //clique gauche
        {
            DEB("1-2-3")
            switch (joueur->act)
            {
                case SELECTED: //on deplace les unitées sur la minimap
                inter = joueur->selection.debut;
                while (inter!=NULL)
                {
                    if (inter->unite->priority<=ORDRE) //si on a la priorité
                    {
                        inter->unite->xdest      = x * COTE;
                        inter->unite->ydest      = y * COTE; //on met les nouvelles coordonnées dans le GPS
                        inter->unite->state      = MOVING;
                        inter->unite->priority   = ORDRE;
                        inter->unite->prec       = MOVING;
                        inter->unite->changepath = 1;
                    }

                    inter = inter->next;
                }
            break; //il n'y a pas d'autre cas pour l'instant
            }

        }
    }
    else if (x>=SEPARE) //si on est dans le menu unité
    {
        DEB("1-2-4")
        if (mouse_b & 1) //clique gauche
        {
            DEB("1-2-5")
            x-=SEPARE; //on se base dans le rectangle du menu

            if (joueur->clic_prec!=1) //si on est bien sur front descendant
            {
                switch (joueur->act)
                {
                    case SELECTED: //on a déjà selectionné des unité
                    p = 0;

                    inter = joueur->selection.debut;
                    while (inter!=NULL)
                    {
                        if (inter->unite->type==PEASANT) //s'il y a des constructeurs dans la selection
                            p = 1;

                        inter = inter->next;
                    }

                    if (p) //s'il y a des constructeurs dans la selection
                    {
                        clk = unit_menu_click(x, y);

                        if (clk) //si la souris est sur l'icone à cliquer
                        {
                            switch (clk)
                            {
                                case 1: //construire une mairie
                                if (joueur->bois>=WOOD_MAINB && joueur->marbre>=ROCK_MAINB)
                                {
                                    joueur->type = MAIRIE;
                                    joueur->xprev = WOOD_MAINB; //on garde les valeurs de ressource à utiliser
                                    joueur->yprev = ROCK_MAINB; //on fera la transaction quand on place le batiment
                                    joueur->act = PLACE_BUILD;
                                }
                            break;

                                case 2: //construire une caserne
                                if (joueur->bois>=WOOD_BARACKS && joueur->marbre>=ROCK_BARACKS)
                                {
                                    joueur->type = CASERNE;
                                    joueur->xprev = WOOD_BARACKS; //on garde les valeurs de ressource à utiliser
                                    joueur->yprev = ROCK_BARACKS; //on fera la transaction quand on place le batiment
                                    joueur->act = PLACE_BUILD;
                                }
                            break;
                            }
                        }
                    }
                break;

                    case SELECT_BUILD: //si un batiment est selectionné

                    clk = unit_menu_click(x, y);

                    if (clk) //si la souris est sur une icone à cliquer
                    {
                        p = 0;
                        bat = carte[joueur->xprev][joueur->yprev].erige; //on prend le pointeur du batiment selectionné

                        if (bat->curr_queue<QUEUE_MAX) //s'il y a de la place dans la queue du batiment
                        {
                            switch (joueur->type)
                            {                  ///Pourrait être fait de manière plus propre mais bon....
                                case MAIRIE:
                                if (clk==1)
                                {
                                    bois = WOOD_PEAS;
                                    pierre = ROCK_PEAS; //on prend les valeur des ressources à payer
                                    uni = PEASANT;
                                    p = 1;
                                }
                            break;

                                case CASERNE:
                                if (clk==1)
                                {
                                    bois = WOOD_SOLD;
                                    pierre = ROCK_SOLD;

                                    uni = SOLDIER;
                                    p = 1;
                                }
                            break;
                            }

                            if (p && (joueur->bois>=bois && joueur->marbre>=pierre)) //si on a assez des ressources necessaires
                            {
                                joueur->bois-=bois;
                                joueur->marbre-=pierre; //on paye

                                if (bat->curr_queue==0) //si aucune unité n'est en cours de formation
                                {
                                    clock_gettime(CLOCK_MONOTONIC, &bat->start); //on initialise le temps de formation
                                }

                                bat->unit_queue[bat->curr_queue] = uni; //on forme l'unité
                                bat->curr_queue++;
                            }
                        }
                    }
                break;
                }
            }
        }
    }
    else if (joueur->clic_prec==1 && !mouse_b&1 && x>=(SEPARE-PAUSE_S) && x<SEPARE && y<PAUSE_S && y>=0) //si on lache le click souris sur le bouton pause
    {
        joueur->pause = 1;
    }

    //on enregistre les clicks
    if (mouse_b & 1)
        joueur->clic_prec = 1;
    else if (mouse_b & 2)
        joueur->clic_prec = 2;
    else
        joueur->clic_prec = 0;

    if (PLACE_BUILD && (mouse_b & 2))
    {
        joueur->act = SELECTED;
        joueur->type = 0;
    }
    DEB("1-2-6")
}

//renvoie quel bouton a été cliqué sur le menu unité (0 si rien)
int unit_menu_click(int x, int y)
{
    int rep = 0;

    if (x%BOX_W<=ICON_S && x<=BOX_W && y%BOX_H>=(BOX_H-ICON_S) && y<(2*BOX_H)) //si la souris est sur une icone à cliquer
    {
        if (y<BOX_H) //si c'est celle du haut
        {
            rep = 1;
        }
        else //celle du bas
        {
            rep = 2;
        }
    }
    return rep;
}


//renvoie l'unité présente à ces coordonnées (NULL s'il n'y en a pas)
Unit *trouve(Ancre ancre, int x, int y, Unit *exclu, int side_excl)
{
    Maillon *maill = ancre.debut;
    Unit *rep = NULL;
    int ix, iy, siz;

    while (maill!=NULL) //on regarde chaque unité
    {
        ix = maill->unite->x;
        iy = maill->unite->y;
        siz = maill->unite->cote;
        if (x<=(ix+siz) && x>=ix && y<=(iy+siz) && y>=iy) //s'il x et y sont à l'interieur de l'unité
        {
            if (maill->unite != exclu && maill->unite->side != side_excl)// si c'est pas l'unité à ignorer
            {
                rep = maill->unite; //on garde l'unité
                break;
            }
        }
        maill = maill->next;
    }
    return rep;
}

//met dans l'ancre dest toutes les unités à l'interieur du rectangle de selection
void selec(Ancre *dest, Ancre ancre, int x1, int y1, int x2, int y2)
{
    Maillon *maill = ancre.debut;
    int ix, iy, siz;

    //on s'arrange pour que x1<x2 et y1<y2
    if (x1>x2)
    {
        siz=x1;
        x1=x2;
        x2=siz;
    }

    if (y1>y2)
    {
        siz=y1;
        y1=y2;
        y2=siz;
    }

    while (maill!=NULL) //on regarde toutes les unités
    {
        if (maill->unite->side==ALLY) //on ne prend que les alliés
        {
            ix = maill->unite->x;
            iy = maill->unite->y;
            siz = maill->unite->cote;

            if (((ix<=x1 && (ix+siz)>=x1) || (ix>=x1 && ix<=x2)) && //on teste si il a un bout du rectangle de selection qui touche un bout de la zone autours de l'unité
                ((iy<=y1 && (iy+siz)>=y1) || (iy>=y1 && iy<=y2)))
            {
                ajout_debut(dest, maill->unite); //on l'ajoute à la selection
            }
        }

        maill = maill->next;
    }
}

//découvre les tuiles autours des coordonnées dans la portée
void eclaire(Tile carte[MAPSIZEX][MAPSIZEY], int x, int y, int porte)
{
    int i, j;
    int nx = DIV(x), ny = DIV(y); //on fait la convertion x et y en tuiles

    for (i=(nx-porte);i<=(nx+porte);i++)
    {
        for (j=(ny-porte);j<=(ny+porte);j++) //pour chaque tuile autours
        {
            if (i>=0 && i<MAPSIZEX && j>=0 && j<MAPSIZEY && !carte[i][j].visible) //si la tuile n'est pas déjà visible
            {
                if (if_dist(nx, ny, i, j, porte)) //si la distance est inferieure à la portée
                    carte[i][j].visible = 1;
            }
        }
    }
}

//renvoie une booleenne: si la distance est inferieure (ou égale) ou non
int if_dist(int x1, int y1, int x2, int y2, int dist)
{
    return !((pow(x1-x2, 2) + pow(y1-y2, 2)) > pow(dist, 2));
}

//pour voir l'optimisation, motif debug
void temps_passe(struct timespec *prev)
{
    if (HOW_LONG)
    {
        struct timespec nouveau;
        int sec, usec, elapsed; //elapsed est en milisec
        clock_gettime(CLOCK_MONOTONIC, &nouveau);

        //le nombre de secondes passées (est généralement 0)
        sec = nouveau.tv_sec - prev->tv_sec;

        //le nombre de microsec passées (est négatif quand sec = 1)
        usec = (int)(nouveau.tv_nsec/1000) - (int)(prev->tv_nsec/1000);

        //le temps passé en milisec
        elapsed = 1000*sec + (int)(usec/1000);

        printf("  : %d\n\n", elapsed);
        *prev = nouveau;
    }

}

//libere une liste chainée de steps
void free_path(Step **ancre)
{
    Step *inter, *supr = *ancre;

    while (supr!=NULL)
    {
        inter = supr->next;

        free(supr);

        supr = inter;
    }

    *ancre = NULL;
}

//passe l'unité au pas suivant
void next_step(Step **ancre)
{
    Step *inter = *ancre;
    *ancre = (*ancre)->next;

    free(inter);

    if (*ancre!=NULL)
        (*ancre)->prev = NULL;
}

//fin du fichier
