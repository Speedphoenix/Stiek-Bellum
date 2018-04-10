#include "auto.h"

/*
    ce fichier contient les sous programmes appellés à chaque tour de boucle de jeu pour l'actualisation
*/

using namespace std;

///L'interieur du commentaire... argh
////déplace l'unité en fonction de la destination
//void move_call(Tile carte[MAPSIZEX][MAPSIZEY], Ancre& ancre, Unit *unite)
//{
//    Unit *enemi=NULL;
//    Tile tuile;
//
//    int *x = &unite->x, *y = &unite->y; //pour ne pas à réécrire trop
//    int *xdest = &unite->xdest, *ydest = &unite->ydest; //pareil
//    int xtile, ytile, xpath, ypath; //pareil, la tuile sur laquelle est l'unité
//
//    int speed = unite->speed; //pour ne pas faire des lignes trop longues
//    int cote = unite->cote;
//
//    int dx=0, dy=0; //la direction dans laquelle aller (vecteur déplacement)
//    int xposs=0, yposs=0; //les poss sont des booleens pour la possibilité de dféplacement selon un axe
//
//    int inter = 0;
//
//    //ptit blindage
//    if (*xdest>=MAPSIZEX*COTE)
//        *xdest = MAPSIZEX*COTE - 1;
//    else if (*xdest<0)
//        *xdest = 0;
//    //on sait jamais
//    if (*ydest>=MAPSIZEY*COTE)
//        *ydest = MAPSIZEY*COTE - 1;
//    else if (*ydest<0)
//        *ydest = 0;
//
//
////    if (unite->changepath)
////        free_path(&unite->step);
//
//    enemi = trouve(ancre, *xdest, *ydest, NULL, unite->side); //pour voir s'il y a un ennemi sur la tuile d'arrivée
//
//    tuile = carte[DIV(*xdest)][DIV(*ydest)]; //la tuile à destination
//
//    //s'il faut arreter le déplacementpour faire autre chose
//    if (if_dist(*x + cote/2, *y + cote/2, *xdest, *ydest, unite->range) &&      //si la destination est dans la portée d'attaque/extraction
//       ((enemi!=NULL && enemi->side!=unite->side) ||                            //si il y a un ennemi à la destination
//       (tuile.erige && tuile.erige->side!=unite->side) ||                       //si il y a un batiment ennemi
//       (tuile.erige && (tuile.erige->side==unite->side && tuile.erige->hp!=tuile.erige->hp_max)) || //si il y a un batiment allié à réparer/construire
//       (tuile.res && unite->prod)))                                             //si il y a de la ressource à extraire
//    {
//        unite->frame = 0;
//
//
//        if (unite->prod && (tuile.res || (tuile.erige && tuile.erige->hp<tuile.erige->hp_max && tuile.erige->side==unite->side)))//production de res ou construction
//        {
//            unite->state = MINE;
//
//            switch (tuile.type) //on met la variable de précision (pour l'animation)
//            {
//                default:
//                case TREE:
//                unite->prec = CHOP;
//            break;
//
//                case BUILDING:
//                case ROCK:
//                unite->prec = MINE;
//            break;
//            }
//        }
//        else
//        {
//            unite->state = ATTACK;
//            unite->prec = ATTACK;
//        }
//    }
//    else //s'il faut se déplacer
//    {
//        if (unite->changepath)
//        {
//            path(carte, ancre, unite);
//            unite->changepath = 0;
//        }
//
//        if (unite->step)
//        {
//            xpath = unite->xpath; //unite->step->x;
//            ypath = unite->ypath; //unite->step->y;
//
//            xtile = DIV(unite->x + cote/2);
//            ytile = DIV(unite->y + cote/2);
//
//            //les valeurs de déplacement
//            if (xtile!=xpath) //s'il faut se déplacer en abscisse
//            {
//                dx = speed * (xtile<xpath?1:-1);
//            }
//            else if (DIV(*xdest)==xtile && *x+cote/2!=*xdest)
//            {
//                inter = *xdest - (*x+cote/2);
//                if (abs(inter)>speed)
//                    dx = speed * ((inter>0)?1:-1);
//                else
//                    dx = inter;
//            }
//
//
//            if (ytile!=ypath)
//            {
//                dy = speed * (ytile<ypath?1:-1);
//            }
//            else if (DIV(*ydest)==ytile && *y+cote/2!=*ydest) //s'il faut se déplacer en ordonnée
//            {
//                inter = *ydest - (*y+cote/2);
//                if (abs(inter)>speed)
//                    dy = speed * ((inter>0)?1:-1);
//                else
//                    dy = inter;
//            }
//
//
//            if (dx &&
//               !(carte[DIV(*x+dx+(cote/2))][DIV(*y+(cote/2))].block) && //si le déplacement en abscisse est possible
//               !trouve(ancre, *x+dx+(cote/2), *y+(cote/2), unite, NEUTR))
//                xposs = 1;
//
//
//            if (dy &&
//               !(carte[DIV(*x+(cote/2))][DIV(*y+dy+(cote/2))].block) && //si le déplacement en ordonnée est possible
//               !trouve(ancre, *x+(cote/2), *y+dy+(cote/2), unite, NEUTR))
//                yposs = 1;
//
//
//
//            if ((xposs || yposs) &&
//               !(carte[DIV(*x+dx+cote/2)][DIV(*y+dy+cote/2)].block) && //si le déplacement en diagonale est possible
//               !trouve(ancre, *x+dx+(cote/2), *y+dy+(cote/2), unite, NEUTR))
//            {
//                if ( (pow(dx, 2)+pow(dy, 2))>pow(speed, 2) )
//                {
//                    dx *= M_SQRT1_2;
//                    dy *= M_SQRT1_2; //on divise par sqrt(2);
//
//                    if (abs(dx)<10) //pour ne pas avoir des valeurs sures (pas qui tendent vers qcc)
//                        dx+=(dx>0)?1:-1;
//                    if (abs(dy)<10)
//                        dy+=(dy>0)?1:-1;
//                }
//                *x += dx;
//                *y += dy;
//
//                if (dx>0) //avec le pathfind la direction du mouvement n'est pas toujours décidée par la destination
//                    unite->direction = RIGHT;
//                else
//                    unite->direction = LEFT;
//            }
//            else if (xposs)
//            {
//                *x += dx;
//
//                if (dx>0) //avec le pathfind la direction du mouvement n'est pas toujours décidée par la destination
//                    unite->direction = RIGHT;
//                else
//                    unite->direction = LEFT;
//            }
//            else if (yposs)
//            {
//                *y += dy;
//
//                //avec le pathfind la direction du mouvement n'est pas toujours décidée par la destination
//                unite->direction = RIGHT;
//            }
//            else //on arrete le mouvement
//            {
//                if (unite->side==ENEMY && unite->bat) //si c'est un garde on ne lui donne pas de distractions
//                    unite->priority = GUARD;
//                else
//                    unite->priority = STAND;
//
//                *xdest = *x;
//                *ydest = *y;
//                unite->frame = 0;
//            }
//
//            if (DIV(*x)!=xtile || DIV(*y)!=ytile)
//            {
//                unite->changepath = 1;
//            }
//
////            if (DIV(*x)==xpath && DIV(*y)==ypath)
////            {
////                next_step(&unite->step);
////            }
//        }
//        else //on arrete le mouvement
//        {
//            if (unite->side==ENEMY && unite->bat) //si c'est un garde on ne lui donne pas de distractions
//                unite->priority = GUARD;
//            else
//                unite->priority = STAND;
//
//            *xdest = *x;
//            *ydest = *y;
//            unite->frame = 0;
//        }
//
//    }
//}
//
////décide du chemin que l'unité doit prendre
//void path(Tile carte[MAPSIZEX][MAPSIZEY], Ancre ancre, Unit *unite)
//{
//    int i, j, k;
//
//    Step *step, *nouv, *inter;
//    Step *open_l = NULL, *close_l = NULL;
//
//    int a = 0, b, g, h;
//    int xf, yf, xs, ys, x, y;
//
//    xf = DIV(unite->xdest);
//    yf = DIV(unite->ydest);
//    xs = DIV(unite->x + unite->cote/2);
//    ys = DIV(unite->y + unite->cote/2);
//
//    //printf("%d %d %d %d", xs, ys, xf, yf);
//
//    if (ABS(xs-xf)<=1 && ABS(ys-yf)<=1)
//    {
//        unite->xpath = xf;
//        unite->ypath = yf;
//
//        unite->step = (Step *) malloc(sizeof(Step));
//        unite->step->x = xf;
//        unite->step->y = yf;
//
//        unite->step->next = NULL;
//        unite->step->prev = NULL;
//    }
//    else
//    {
//        step = (Step *) malloc(sizeof(Step));
//
//        step->x = DIV(unite->x + unite->cote/2);
//        step->y = DIV(unite->y + unite->cote/2);
//        step->g = 0;
//        step->h = 1.4*MIN(ABS(xs-xf), ABS(ys-yf)) + ABS((xs-xf) - ABS(ys-yf));
//        step->total = step->g + step->h;
//
//        step->next = NULL;
//        step->prev = NULL;
//        //on le met dans la liste ouverte
//        open_l = step;
//
//        step->from = NULL;
//        step->ending = 0;
//
//        while (!a)
//        {//printf("a\n");
//            x = step->x;
//            y = step->y;
//            //on voit toutes les cases autours de la courante pour check+trouver un chemin
//            for (i=-1;i<=1;i++)
//            {
//                for (j=-1;j<=1;j++)
//                {
//                    b = 1; //b est à 1 s'il faut créer un nouveau pas pour cette case
//                    if ((i || j) && x+i<MAPSIZEX && x+i>=0 && y+j<MAPSIZEY && y+j>=0 && //si la case est accessible
//                    !carte[x+i][y+j].block/* && !trouve(ancre, (x+i)*COTE+COTE/2, (y+j)*COTE+COTE/2, unite, NEUTR)*/)
//                    {
//                                                            //si on est en diagonale il faut check les adjacents
//                        if (!((i && j) && ((carte[x+i][y].block/* || trouve(ancre, (x+i)*COTE+COTE/2, y*COTE+COTE/2, unite, NEUTR)*/) ||
//                            (carte[x][y+j].block/* || trouve(ancre, x*COTE+COTE/2, (y+j)*COTE+COTE/2, unite, NEUTR)*/))))
//                        {//printf("0\n");
//
//                            g = step->g + (i && j)?1.4:1;
//                            h = 1.4*MIN(ABS((x+i)-xf), ABS((y+j)-yf)) + ABS(ABS((x+i)-xf) - ABS((y+j)-yf));
//
//                            if ((1.4*MIN(ABS((x+i)-xs), ABS((y+j)-ys)) + ABS(ABS((x+i)-xs) - ABS((y+j)-ys)))>2*unite->vision)
//                            {//printf("1\n");
//                                step->ending = 1;
//                                a = 1;
//                            }
//
//                            k = 0;
//                            inter = close_l;
//                            //printf("2-0\n");
//                            while (inter!=NULL && b)
//                            {////printf("2\n");
//                                k++;
//                                if (inter->x==x+i && inter->y==y+j)
//                                    b = 0;
//
//                                inter = inter->next;
//                            }
//                            //printf("2-1  %d\n", k);
//
//                            if (b)
//                            {
//                                inter = open_l;
//                                while(inter!=NULL)
//                                {
//                                    nouv = inter->next;
//                                    if (inter->x==x+i && inter->y==y+j)
//                                    {//printf("3\n");
//                                        if (inter->total>(g+h))
//                                        {//printf("4\n");
//                                            if(inter->next!=NULL)
//                                                inter->next->prev = inter->prev;
//
//                                            if (inter->prev!=NULL)
//                                                inter->prev->next = inter->next;
//                                            else
//                                                open_l = inter->next;
//
//                                            free(inter);
//
//                                            break;
//                                        }
//                                        else
//                                            b = 0;
//                                    }
//
//                                    inter = nouv;
//                                }
//
//                                if (b)
//                                {
//                                    nouv = (Step *) malloc(sizeof(Step));
//
//                                    nouv->g = g;
//                                    nouv->h = h;
//                                    nouv->x = x + i;
//                                    nouv->y = y + j;
//                                    nouv->total = g + h;
//
//                                    nouv->next = open_l;
//                                    nouv->prev = NULL;
//                                    //on le met dans la liste ouverte
//
//                                    if (open_l!=NULL)
//                                        open_l->prev = nouv;
//
//                                    open_l = nouv;
//
//                                    nouv->from = step;
//
//                                    if (nouv->x==xf && nouv->y==yf)
//                                    {//printf("5\n");
//                                        nouv->ending = 1;
//                                        a = 1;
//                                    }
//                                    else
//                                        nouv->ending = 0;
//                                }
//                            }
//                        }
//                    }
//                    if (a)
//                        break;
//                }
//                if (a)
//                    break;
//            }
//            //printf("a: %d\n", a);
//            //si on a trouvé un pas final (dans la liste ouverte)
//            if (a)
//            {//printf("9\n");
//                step = open_l;
//                while (!step->ending && step!=NULL)
//                    step = step->next;
//
//                if (step==NULL)
//                    a = 0;
//            }
//            else
//            {//printf("10\n");
//                //on enlève le courant pour le mettre dans la fermée
//                if(step->next!=NULL)
//                    step->next->prev = step->prev;
//
//                if (step->prev!=NULL)
//                    step->prev->next = step->next;
//                else
//                    open_l = step->next;
//                //printf("11\n");
//                //on le met dans la fermée
//                step->next = close_l;
//                step->prev = NULL;
//
//                if (close_l!=NULL)
//                    close_l->prev = step;
//
//                close_l = step;
//                //printf("12\n");
//                if (open_l==NULL) /// arreter le mouvemnt s'il n'y a plus de déplacements à faire (on est bloqué)
//                {//printf("13\n");
//                    a = 1;
//                    step = close_l;
//                    inter = close_l;
//                    while (inter!=NULL)
//                    {//printf("14\n");
//                        if (inter->total<step->total)
//                        {//printf("15\n");
//                            step = inter;
//                        }
//
//                        inter = inter->next;
//                    }
//                }
//                else //on trouve le meilleur dans la ouverte
//                {//printf("16\n");
//                    step = open_l;
//                    inter = open_l;
//                    while (inter!=NULL)
//                    {//printf("17\n");
//                        if (inter->total<step->total)
//                        {//printf("18\n");
//                            step = inter;
//                        }
//
//                        inter = inter->next;
//                    }
//                }
//            }
//        }
//
////        nouv = (Step *)malloc(sizeof(Step));
////        inter = NULL;
////E
////        while (step->from!=NULL)
////        {E
////            *nouv = *step;
////            nouv->from = NULL;
////            nouv->next = inter;
////
////            if (inter!=NULL)
////                inter->prev = nouv;
////
////            step = step->from;
////
////            inter = nouv;
////            if (step->from)
////                nouv = (Step *)malloc(sizeof(Step));
////        }
////printf("\n");E
////        unite->step = nouv;
//
//        nouv = step;
//
//        if (step->from!=NULL)
//        {
//            step = step->from;
//            while (step->from!=NULL)
//            {
//                nouv = step;
//                step = step->from;
//            }
//        }
//E
//        unite->xpath = nouv->x;
//        unite->ypath = nouv->y;
//
//
//        //libere les deux listes
//        free_path(&open_l);
//        free_path(&close_l);
//    }
//}


//fonction d'actualisation, apellée à chaque tour de boucle
void update(Tile carte[MAPSIZEX][MAPSIZEY], list<Unit *>& ancre, list<Build *>& ancre_b, Joueur& joueur)
{
    int bois, pierre, nend, if_lose = 1; //if_lose sert pour verifier les conditions de défaite
    Build *build;

    list<Build *>::iterator iter_b;
    list<Unit *>::iterator iter;

    joueur.nend_e = 0;

    nend = 0;

    DEB("2-0")

    for (iter_b = ancre_b.begin();iter_b!=ancre_b.end();iter_b++)
    {
        build = *iter_b;

        DEB("2-1")

        if (build->hp<=0)
        {
            DEB("2-2")
            destroy_build(ancre_b, iter_b, carte);
            joueur.change = 1;
        }
        else
        {
            DEB("2-3")
            if (build->curr_queue) //s'il y a des unités en formation dans ce batiment
            {
                DEB("2-4")
                formation(ancre, carte, *build);
            }
            DEB("2-5")
            if (build->side==ENEMY)
            {
                DEB("2-6")
                nend++; //le jeu n'est pas fini

                if (build->statione<build->cap) //si il y a de la place pour spawn des ennemis autours
                    spawn_camp(*build, ancre, carte);
            }
            else //si c'est un batiment allié
            {
                DEB("2-7")
                if (if_lose) //si les conditions de défaites pourraient être vraies
                {
                    DEB("2-8")
                    switch (carte[build->x][build->y].position/4)
                    {
                        default:
                        case MAIRIE:
                        bois = WOOD_PEAS;
                        pierre = ROCK_PEAS;
                    break;

                        case CASERNE:
                        bois = WOOD_SOLD;
                        pierre = ROCK_SOLD;
                    break;
                    }

                    if (build->curr_queue)
                        if_lose = 0;

                    if (bois<=joueur.bois && pierre<=joueur.marbre) //si le joueur est en position pour créer des unités
                        if_lose = 0;
                }
            }
            DEB("2-8")
        }

    }

    joueur.nend_b = nend;

    DEB("2-10")


    for (iter = ancre.begin();iter!=ancre.end();iter++)
    {
        DEB("2-11")

        //on regarde une unité
        Unit& unite = *(*iter);


        if (unite.side==ALLY)
            if_lose = 0;

        //on met l'unité dans la bonne direction
        if (unite.x==unite.xdest)
            unite.direction = RIGHT;
        else if (unite.xdest<(unite.x + unite.cote/2))
            unite.direction = LEFT;
        else
            unite.direction = RIGHT;

        //on tue les unités qui ont décédé
        if(unite.hp<=0)
            unite.state = DEAD;

        if (unite.state==DEAD)
        {
            DEB("2-12")
            if (unite.side==ENEMY)
            {
                if (unite.predator==ALLY)
                    joueur.viande++;

                if (unite.bat) //on libere de l'espace de stationnement si l'unité est associée à un batiment
                {
                    if (unite.bat->hp>0)
                    {
                        unite.bat->statione--;
                        //on remet à zero le compeur pour ne pas que l'unite suivante arrive tout de suite, et pour que le camp ne spaw pas s'il est en combat
                        getTime(unite.bat->start);
                    }
                }
            }
            DEB("2-13")

            delete *iter;
            ancre.erase(iter);
        }
        else
        {
            DEB("2-14")
            act_unit(unite, ancre, carte, joueur);

            if (unite.side==ENEMY)
            {
                joueur.nend_e++;
                DEB("2-51")
                automat(ancre, ancre_b, carte, joueur, unite);
                DEB("2-52")
            }
        }

    }

    DEB("2-60")

    //on fait spawn (si besoin) des ennemis aléatoirement sur la map
    if (nend)
        spawn_map(joueur, ancre, carte);

    DEB("2-61")

    if (if_lose)
        joueur.nend_b = -1;
}

//l'actualisation d'une seule unité
void act_unit(Unit& unite, list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Joueur& joueur)
{
    switch (unite.state)
    {
        case MOVING: //si l'unité est en mouvement OU inactive
        DEB("2-21")
        if (unite.side==ALLY) //on eclaire la zone autours
            eclaire(carte, unite.x+unite.cote/2, unite.y+unite.cote/2, unite.vision);

        unite.prec = MOVING; //on met la bonne animation

        if (unite.x!=unite.xdest || unite.y!=unite.ydest) //si l'unité est en mouvement
        {
            DEB("2-22")
            //sous-prog de déplacement
            move_call(carte, ancre, unite);

            //s'il faut passer à l'image suivante en termes d'animation
            if (if_elapsed(unite, ANIMATION) && unite.state==MOVING) //si on vient de changer le type d'action on veut pas passer à l'image suivante
            {
                unite.frame++;
                if (unite.frame==NUMFRAMES)
                    unite.frame = 0;
            }

        }
        else //si l'unité est immobile
        {
            DEB("2-23")
            unite.frame = 0;

            if (unite.side==ENEMY && unite.bat) //si c'est un garde on ne lui donne pas de distractions
                unite.priority = GUARD;
            else
                unite.priority = STAND;

            unite.prec = MOVING;

        }
    break;

        case ATTACK: //si l'unité est en train d'attaquer
        DEB("2-30")
        if (unite.frame==0) //si l'unité n'est pas au milieu d'une attaque
        {
            DEB("2-31")
            if (if_elapsed(unite, WORK)) //si il est temps d'attaquer à nouveau
            {
                DEB("2-32")
                attack(ancre, carte, unite); //on attaque

                unite.frame++; //on passe à l'image suivante
            }
        }
        else
        {
            DEB("2-3")
            if (if_elapsed(unite, ANIMATION)) //s'il faut passer à la prochaine image de l'animation
            {
                DEB("2-34")
                unite.frame++;
                if (unite.frame==NUMFRAMES)
                    unite.frame = 0;
            }
        }
    break;

        case MINE:
        DEB("2-40")
        if (unite.prod) //si c'est bien une unité qui peut produire
        {
            DEB("2-41")
            if (unite.frame==0)
            {
                DEB("2-42")
                if (if_elapsed(unite, WORK)) //s'il faut extraire à nouveau
                {
                    DEB("2-43")
                    mine(carte, unite, joueur);

                    unite.frame++;
                }
            }
            else
            {
                DEB("2-44")
                if (if_elapsed(unite, ANIMATION))
                {
                    DEB("2-45")
                    unite.frame++;
                    if (unite.frame==NUMFRAMES)
                        unite.frame = 0;
                }
            }
        }
        else
            unite.state = MOVING;
    break;

        default:
    break;
    }

    unite.predator = 0;
    DEB("2-50")
}

//déplace l'unité en fonction de la destination
void move_call(Tile carte[MAPSIZEX][MAPSIZEY], list<Unit *>& ancre, Unit& unite)
{
    Unit *enemi=NULL;
    Tile tuile;

    ///FAIRE DES REFERENCES
    int *x = &unite.x, *y = &unite.y; //pour ne pas à réécrire trop
    int *xdest = &unite.xdest, *ydest = &unite.ydest; //pareil
    int xtile, ytile; //pareil, la tuile sur laquelle est l'unité

    int speed = unite.speed; //pour ne pas faire des lignes trop longues
    int cote = unite.cote;

    int dx=0, dy=0; //la direction dans laquelle aller (vecteur déplacement)
    int xposs=0, yposs=0; //les poss sont des booleens pour la possibilité de dféplacement selon un axe

    int inter = 0;

    //ptit blindage
    if (*xdest>=MAPSIZEX*COTE)
        *xdest = MAPSIZEX*COTE - 1;
    else if (*xdest<0)
        *xdest = 0;
    //on sait jamais
    if (*ydest>=MAPSIZEY*COTE)
        *ydest = MAPSIZEY*COTE - 1;
    else if (*ydest<0)
        *ydest = 0;


    enemi = trouve(ancre, *xdest, *ydest, NULL, unite.side); //pour voir s'il y a un ennemi sur la tuile d'arrivée

    tuile = carte[DIV(*xdest)][DIV(*ydest)]; //la tuile à destination

    //s'il faut arreter le déplacementpour faire autre chose
    if (if_dist(*x + cote/2, *y + cote/2, *xdest, *ydest, unite.range) &&      //si la destination est dans la portée d'attaque/extraction
       ((enemi!=NULL && enemi->side!=unite.side) ||                            //si il y a un ennemi à la destination
       (tuile.erige && tuile.erige->side!=unite.side) ||                       //si il y a un batiment ennemi
       (tuile.erige && (tuile.erige->side==unite.side && tuile.erige->hp!=tuile.erige->hp_max)) || //si il y a un batiment allié à réparer/construire
       (tuile.res && unite.prod)))                                             //si il y a de la ressource à extraire
    {
        unite.frame = 0;


        if (unite.prod && (tuile.res || (tuile.erige && tuile.erige->hp<tuile.erige->hp_max && tuile.erige->side==unite.side)))//production de res ou construction
        {
            unite.state = MINE;

            switch (tuile.type) //on met la variable de précision (pour l'animation)
            {
                default:
                case TREE:
                unite.prec = CHOP;
            break;

                case BUILDING:
                case ROCK:
                unite.prec = MINE;
            break;
            }
        }
        else
        {
            unite.state = ATTACK;
            unite.prec = ATTACK;
        }
    }
    else //s'il faut se déplacer
    {
        if (unite.changepath)
        {
            path(carte, ancre, unite);
        }

        xtile = DIV(unite.x + cote/2);
        ytile = DIV(unite.y + cote/2);

        //les valeurs de déplacement
        if (xtile!=unite.xpath) //s'il faut se déplacer en abscisse
        {
            dx = speed * (xtile<unite.xpath?1:-1);
        }
        else if (DIV(*xdest)==xtile && *x+cote/2!=*xdest)
        {
            inter = *xdest - (*x+cote/2);
            if (abs(inter)>speed)
                dx = speed * ((inter>0)?1:-1);
            else
                dx = inter;
        }


        if (ytile!=unite.ypath)
        {
            dy = speed * (ytile<unite.ypath?1:-1);
        }
        else if (DIV(*ydest)==ytile && *y+cote/2!=*ydest) //s'il faut se déplacer en ordonnée
        {
            inter = *ydest - (*y+cote/2);
            if (abs(inter)>speed)
                dy = speed * ((inter>0)?1:-1);
            else
                dy = inter;
        }


        if (dx &&
           !(carte[DIV(*x+dx+(cote/2))][DIV(*y+(cote/2))].block) && //si le déplacement en abscisse est possible
           !trouve(ancre, *x+dx+(cote/2), *y+(cote/2), &unite, NEUTR))
            xposs = 1;


        if (dy &&
           !(carte[DIV(*x+(cote/2))][DIV(*y+dy+(cote/2))].block) && //si le déplacement en ordonnée est possible
           !trouve(ancre, *x+(cote/2), *y+dy+(cote/2), &unite, NEUTR))
            yposs = 1;



        if ((xposs || yposs) &&
           !(carte[DIV(*x+dx+cote/2)][DIV(*y+dy+cote/2)].block) && //si le déplacement en diagonale est possible
           !trouve(ancre, *x+dx+(cote/2), *y+dy+(cote/2), &unite, NEUTR))
        {
            if ((pow(dx,2)+pow(dy,2)) > pow(speed, 2))
            {
                dx *= M_SQRT1_2;
                dy *= M_SQRT1_2; //on divise par sqrt(2);

                if (abs(dx)<10) //pour ne pas avoir des valeurs sures (pas qui tendent vers qcc)
                    dx+=(dx>0)?1:-1;
                if (abs(dy)<10)
                    dy+=(dy>0)?1:-1;
            }
            *x += dx;
            *y += dy;

            if (dx>0) //avec le pathfind la direction du mouvement n'est pas toujours décidée par la destination
                unite.direction = RIGHT;
            else
                unite.direction = LEFT;
        }
        else if (xposs)
        {
            *x += dx;

            if (dx>0) //avec le pathfind la direction du mouvement n'est pas toujours décidée par la destination
                unite.direction = RIGHT;
            else
                unite.direction = LEFT;
        }
        else if (yposs)
        {
            *y += dy;

            //avec le pathfind la direction du mouvement n'est pas toujours décidée par la destination
            unite.direction = RIGHT;
        }
        else //on arrete le mouvement
        {
            if (unite.side==ENEMY && unite.bat) //si c'est un garde on ne lui donne pas de distractions
                unite.priority = GUARD;
            else
                unite.priority = STAND;

            *xdest = *x;
            *ydest = *y;
            unite.frame = 0;
        }

        if (DIV(*x)!=xtile || DIV(*y)!=ytile)
        {
            unite.changepath = 1;
        }
    }
}

//décide du chemin que l'unité doit prendre
void path(Tile carte[MAPSIZEX][MAPSIZEY], list<Unit *>& ancre, Unit& unite)
{
    int i, j, k;

    Step *step, *nouv, *inter;
    Step *open_l = NULL, *close_l = NULL;

    int a = 0, b, g, h;
    int xf, yf, xs, ys, x, y;

    xf = DIV(unite.xdest);
    yf = DIV(unite.ydest);
    xs = DIV(unite.x + unite.cote/2);
    ys = DIV(unite.y + unite.cote/2);

    //printf("%d %d %d %d", xs, ys, xf, yf);

    if (ABS(xs-xf)<=1 && ABS(ys-yf)<=1)
    {
        unite.xpath = xf;
        unite.ypath = yf;
    }
    else
    {
        step = (Step *) malloc(sizeof(Step));

        step->x = DIV(unite.x + unite.cote/2);
        step->y = DIV(unite.y + unite.cote/2);
        step->g = 0;
        step->h = 1.4*MIN(ABS(xs-xf), ABS(ys-yf)) + ABS((xs-xf) - ABS(ys-yf));
        step->total = step->g + step->h;

        step->next = NULL;
        step->prev = NULL;
        //on le met dans la liste ouverte
        open_l = step;

        step->from = NULL;
        step->ending = 0;

        while (!a)
        {//printf("a\n");
            x = step->x;
            y = step->y;
            //on voit toutes les cases autours de la courante pour check+trouver un chemin
            for (i=-1;i<=1;i++)
            {
                for (j=-1;j<=1;j++)
                {
                    b = 1; //b est à 1 s'il faut créer un nouveau pas pour cette case
                    if ((i || j) && x+i<MAPSIZEX && x+i>=0 && y+j<MAPSIZEY && y+j>=0 && //si la case est accessible
                    !carte[x+i][y+j].block/* && !trouve(ancre, (x+i)*COTE+COTE/2, (y+j)*COTE+COTE/2, &unite, NEUTR)*/)
                    {
                                                            //si on est en diagonale il faut check les adjacents
                        if (!((i && j) && ((carte[x+i][y].block/* || trouve(ancre, (x+i)*COTE+COTE/2, y*COTE+COTE/2, &unite, NEUTR)*/) ||
                            (carte[x][y+j].block/* || trouve(ancre, x*COTE+COTE/2, (y+j)*COTE+COTE/2, &unite, NEUTR)*/))))
                        {//printf("0\n");

                            g = step->g + ((i && j)?1.4:1.0);
                            h = 1.4*MIN(ABS((x+i)-xf), ABS((y+j)-yf)) + ABS(ABS((x+i)-xf) - ABS((y+j)-yf));

                            if ((1.4*MIN(ABS((x+i)-xs), ABS((y+j)-ys)) + ABS(ABS((x+i)-xs) - ABS((y+j)-ys)))>2*unite.vision)
                            {//printf("1\n");
                                step->ending = 1;
                                a = 1;
                            }

                            k = 0;
                            inter = close_l;
                            //printf("2-0\n");
                            while (inter!=NULL && b)
                            {////printf("2\n");
                                k++;
                                if (inter->x==x+i && inter->y==y+j)
                                    b = 0;

                                inter = inter->next;
                            }
                            //printf("2-1  %d\n", k);

                            if (b)
                            {
                                inter = open_l;
                                while(inter!=NULL)
                                {
                                    nouv = inter->next;
                                    if (inter->x==x+i && inter->y==y+j)
                                    {//printf("3\n");
                                        if (inter->total>(g+h))
                                        {//printf("4\n");
                                            if(inter->next!=NULL)
                                                inter->next->prev = inter->prev;

                                            if (inter->prev!=NULL)
                                                inter->prev->next = inter->next;
                                            else
                                                open_l = inter->next;

                                            free(inter);

                                            break;
                                        }
                                        else
                                            b = 0;
                                    }

                                    inter = nouv;
                                }

                                if (b)
                                {
                                    nouv = (Step *) malloc(sizeof(Step));

                                    nouv->g = g;
                                    nouv->h = h;
                                    nouv->x = x + i;
                                    nouv->y = y + j;
                                    nouv->total = g + h;

                                    nouv->next = open_l;
                                    nouv->prev = NULL;
                                    //on le met dans la liste ouverte

                                    if (open_l!=NULL)
                                        open_l->prev = nouv;

                                    open_l = nouv;

                                    nouv->from = step;

                                    if (nouv->x==xf && nouv->y==yf)
                                    {//printf("5\n");
                                        nouv->ending = 1;
                                        a = 1;
                                    }
                                    else
                                        nouv->ending = 0;
                                }
                            }
                        }
                    }
                    if (a)
                        break;
                }
                if (a)
                    break;
            }
            //printf("a: %d\n", a);
            //si on a trouvé un pas final (dans la liste ouverte)
            if (a)
            {//printf("9\n");
                step = open_l;
                while (!step->ending && step!=NULL)
                    step = step->next;

                if (step==NULL)
                    a = 0;
            }
            else
            {//printf("10\n");
                //on enlève le courant pour le mettre dans la fermée
                if(step->next!=NULL)
                    step->next->prev = step->prev;

                if (step->prev!=NULL)
                    step->prev->next = step->next;
                else
                    open_l = step->next;
                //printf("11\n");
                //on le met dans la fermée
                step->next = close_l;
                step->prev = NULL;

                if (close_l!=NULL)
                    close_l->prev = step;

                close_l = step;
                //printf("12\n");
                if (open_l==NULL) /// arreter le mouvemnt s'il n'y a plus de déplacements à faire (on est bloqué)
                {//printf("13\n");
                    a = 1;
                    step = close_l;
                    inter = close_l;
                    while (inter!=NULL)
                    {//printf("14\n");
                        if (inter->total<step->total)
                        {//printf("15\n");
                            step = inter;
                        }

                        inter = inter->next;
                    }
                }
                else //on trouve le meilleur dans la ouverte
                {//printf("16\n");
                    step = open_l;
                    inter = open_l;
                    while (inter!=NULL)
                    {//printf("17\n");
                        if (inter->total<step->total)
                        {//printf("18\n");
                            step = inter;
                        }

                        inter = inter->next;
                    }
                }
            }
        }

        nouv = step;

        if (step->from!=NULL)
        {
            step = step->from;
            while (step->from!=NULL)
            {
                nouv = step;
                step = step->from;
            }
        }

        unite.xpath = nouv->x;
        unite.ypath = nouv->y;


        //liberer les deux listes

        inter = open_l;
        while (inter!=NULL)
        {
            nouv = inter->next;
            free(inter);
            inter = nouv;
        }

        inter = close_l;
        while (inter!=NULL)
        {
            nouv = inter->next;
            free(inter);
            inter = nouv;
        }
    }
}

//fait attaquer l'unité
void attack(list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Unit& unite)
{
    Unit *victime;
    Tile tuile = carte[DIV(unite.xdest)][DIV(unite.ydest)];


    //si on est dans la portée d'attaque
    if (if_dist(unite.x + unite.cote/2, unite.y + unite.cote/2, unite.xdest, unite.ydest, unite.range))
    {
        victime = trouve(ancre, unite.xdest, unite.ydest, NULL, unite.side);

        if (victime!=NULL) //s'il y a à attaquer
        {
            if (victime->priority<TAUNT) //si la victime doit se sentir agressée
            {
                victime->xdest = unite.x + unite.cote/2; //on dit à l'unité d'ataquer aussi
                victime->ydest = unite.y + unite.cote/2;
                victime->state = MOVING;
                victime->prec = MOVING;
                victime->priority = TAUNT;
                victime->changepath = 1;
            }

            victime->hp -= unite.damage;
            victime->predator = unite.side;
        }
        else if(tuile.erige!=NULL && tuile.erige->side!=unite.side) //s'om y a un batiment ennemi
        {
            tuile.erige->hp -= unite.damage;
        }
        else
        {
            unite.state = MOVING;
            unite.prec = MOVING;

            if (unite.side==ENEMY && unite.bat) //si c'est un garde on ne lui donne pas de distractions
            {
                unite.xdest = unite.x;
                unite.ydest = unite.y;
                unite.priority = GUARD;
            }
            else
            {
                unite.priority = AUTO;
                unite.changepath = 1;
            }

        }
    }
    else //si on est pas dans la portée d'attaque
    {
        unite.state = MOVING;
        unite.prec = MOVING;

        if (unite.side==ENEMY && unite.bat) //si c'est un garde on ne lui donne pas de distractions
        {
            unite.xdest = unite.x;
            unite.ydest = unite.y;
            unite.priority = GUARD;
        }
        else
        {
            unite.priority = AUTO;
            unite.changepath = 1;
        }
    }
}

//fait extraire des ressources par l'unité
void mine(Tile carte[MAPSIZEX][MAPSIZEY], Unit& unite, Joueur& joueur)
{
    int x, y; //les coordonnées de la tuile à extraire

    //si on est dans la portée d'extraction
    if (if_dist(unite.x + unite.cote/2, unite.y + unite.cote/2, unite.xdest, unite.ydest, unite.range))
    {

        x = DIV(unite.xdest);
        y = DIV(unite.ydest);

        if (carte[x][y].res) //s'il y a un nombre non nul de ressources à cet endroit
        {
            if (((carte[x][y].type==TREE)?joueur.bois:joueur.marbre)<MAXRES)
            {
                unite.prec = (carte[x][y].type==TREE)?CHOP:MINE; //le type d'animation
                carte[x][y].res -= unite.damage; //on diminue la quantité de ressources à destination

                switch (carte[x][y].type)
                {
                    case TREE:
                    joueur.bois += unite.damage;

                    if (carte[x][y].res<0)              //si on a pris plus que la quantité de ressources restantes
                        joueur.bois+=carte[x][y].res;  //on enleve le surplus
                break;

                    case ROCK:
                    joueur.marbre += unite.damage;

                    if (carte[x][y].res<0)              //si on a pris plus que la quantité de ressources restantes
                        joueur.marbre+=carte[x][y].res;//on enleve le surplus
                break;

                    default:
                break;
                }

                if (carte[x][y].res<=0) //on enleve l'arbre / rocher s'il n'y a pllus de ressourcces
                {
                    enlev(carte, x, y);
                    joueur.change = 1;
                    if (!next_res(unite, carte)) //si on a pas trouvé de prochaine ressource à extraire
                    {
                        unite.xdest = unite.x;
                        unite.ydest = unite.y;
                        unite.state = MOVING;
                        unite.prec = MOVING;
                        unite.priority = STAND;
                    }
                }
            }
            else
            {
                unite.xdest = unite.x;
                unite.ydest = unite.y;
                unite.state = MOVING;
                unite.prec = MOVING;
                unite.priority = STAND;
            }

        }
        else if (carte[x][y].erige && (carte[x][y].erige->side==unite.side && carte[x][y].erige->hp<carte[x][y].erige->hp_max)) //s'il faut réparer/construire un batiment
        {
            carte[x][y].erige->hp += unite.damage; //on ajoute les points de vie au batiment

            if (carte[x][y].erige->hp>carte[x][y].erige->hp_max) //on finalise la construction du batiment
            {
                carte[x][y].erige->hp = carte[x][y].erige->hp_max;
            }
        }
        else //s'il n'y a rien à faire
        {
            if (!next_res(unite, carte)) //si on a pas trouvé de prochaine ressource à extraire
            {
                unite.xdest = unite.x;
                unite.ydest = unite.y;
                unite.state = MOVING;
                unite.prec = MOVING;
                unite.priority = STAND;
            }
        }
    }
    else //si y'a pas la portée
    {
        unite.state = MOVING;
        unite.prec = MOVING;
        unite.priority = AUTO;
        unite.changepath = 1;
    }
}

//Supprime une tuile en faisant attention à enlever les feuilles d'un arbre....
void enlev(Tile carte[MAPSIZEX][MAPSIZEY], int x, int y)
{
    switch (carte[x][y].type)
    {
        case TREE:
        switch (carte[x][y].position)
        {
            case 0: //si c'est la partie basse d'un grand arbre
            case 2:
            if (y!=0)
            {       //si c'est un arbre sur deux cases et qu'il faut detruire aussi les feuilles
                if (carte[x][y-1].type==carte[x][y].type && carte[x][y-1].position==(carte[x][y].position+1))
                {
                    carte[x][y-1].type = GRASS;
                    carte[x][y-1].position = P_HERBE;
                    carte[x][y-1].block = 0;
                }
            }
            case 4: //petit arbre
            carte[x][y].type = GRASS;
            carte[x][y].position = P_HERBE;
            carte[x][y].res = 0;
            carte[x][y].block = 0;
        break;

            default:
        break;
        }
    break;

        case ROCK:
        carte[x][y].type = GRASS;
        carte[x][y].position = P_SABLE;
        carte[x][y].res = 0;
        carte[x][y].block = 0;
    break;

        default:
    break;
    }
}

//renvoie une booléenne: si il faut passer ou non à l'image suivante et faire avancer l'unité
int if_elapsed(Unit& unite, int type)
{
    TIMESTRUCT inter;
    double pas, elapsed;

    getTime(inter);
    //la précision en nanosecodes est inutile, des microsecondes auraient suffi (avec struct timeval au lieu de timespec)

    switch (type)
    {
        default:
        case ANIMATION:
        pas = DELAY;
        elapsed = getSec(unite.since_a, inter);
    break;
        case WORK:
        pas = unite.delay;
        elapsed = getSec(unite.since_w, inter);
    break;
    }


    if (elapsed >= pas)
    {
        switch (type) //on met le nouveau temps
        {
            default:
            case ANIMATION:
            unite.since_a = inter;
        break;
            case WORK:
            unite.since_w = inter;
        break;
        }
        return 1;
    }
    else //discutablement inutile
        return 0;
}

//forme regarde le batiment et forme une unité s'il y en a une à former
void formation(list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Build& build)
{
    int i;
    TIMESTRUCT now;
    double elapsed, needed;
    int x, y;

    if (build.curr_queue) //on sait jamais
    {
        getTime(now);

        switch (build.unit_queue[0]) //on regarde combien de temps on est censé attendre
        {
            default:
            case PEASANT:
            needed = TEMPS_PAYS;
        break;

            case SOLDIER:
            needed = TEMPS_SOLD;
        break;
        }

        elapsed = getSec(build.start, now);

            //si on a passé le temps de formation de l'unite
        if (elapsed>=needed)
        {
            if (reparti(build, ancre, carte, &x, &y))
            {
                ancre.push_back(new Unit(build.unit_queue[0], STAND, x, y, &build));

                build.curr_queue--; //on enleve l'unité de la queue de formation
                for (i=0;i<build.curr_queue;i++)
                {                                       //on décale la queue de un cran
                    build.unit_queue[i] = build.unit_queue[i+1];
                }
                build.start = now; //on remet à zero le temps de formation
            }
        }
    }
}

//repartit l'unité autours du batiment là où y'a de la place
int reparti(Build& bat, list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], int *x, int *y)
{
    int i, j;
    int rep = 0;
    for (i=(bat.x*COTE - FORMRANGE);i<((bat.x+bat.w)*COTE + FORMRANGE);i+=25) //on regarde tout l'espace autours du batiment
    {
        for (j=(bat.y*COTE - FORMRANGE);j<((bat.y+bat.h)*COTE + FORMRANGE);j+=25)
        {
            if (!carte[DIV(i)][DIV(j)].block && !trouve(ancre, i, j, NULL, NEUTR)) //si y'a de la place
            {
                *x = i - COTE/2; //on change les valeurs
                *y = j - COTE/2;
                rep = 1;
                break;
            }
        }
        if (rep)
            break;
    }
    return rep; //on renvoie si ça a marché ou non
}

//spawn des ennemis autours d'un batiment ennemi (et blindage)
void spawn_camp(Build& build, list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY])
{
    TIMESTRUCT now;
    double elapsed;
    int x, y, a=0, i=0;

    getTime(now);


    elapsed = getSec(build.start, now);

    if (elapsed>=TEMPS_ENNE_B)
    {
        while (!a && i<42) //oon regarde max 40 tests random et si ça trouve rien on passe à reparti
        {
            if (i<40)
            {
                x = (rand()%(2*SPAWNRANGE + build.w)) + build.x*COTE - SPAWNRANGE;
                y = (rand()%(2*SPAWNRANGE + build.h)) + build.y*COTE - SPAWNRANGE; //randomize les coordonnées

                if (x<0) //ptit blindage
                    x=0;
                else if (x>=MAPSIZEX*COTE)
                    x=MAPSIZEX*COTE;
                if (y<0)
                    y=0;
                else if (y>=MAPSIZEY*COTE)
                    y=MAPSIZEY*COTE;

                //si l'emplacement est libre
                if (!carte[DIV(x)][DIV(y)].block && !trouve(ancre, x, y, NULL, NEUTR))
                {
                    a = 1;
                    x-=COTE/2; //on prend les bonnes coordonnées
                    y-=COTE/2;
                }
            }
            else //si on a eu aucun succes avec les RNG
                a = reparti(build, ancre, carte, &x, &y);
        }


        if (a) //si ça a marché on ajoute l'unité
        {
            ancre.push_back(new Unit(ENEMY, GUARD, x, y, &build));

            build.statione++; //le batiment a moins de place maintenant

            build.start = now; //on remet à zero le compteur de la prochaine unité
        }
    }

}

//spawn des ennemis aléatoirement sur la map
void spawn_map(Joueur& joueur, list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY])
{
    int i, j;
    TIMESTRUCT now;
    double elapsed;
    int x, y, a=0;

    getTime(now);


    elapsed = getSec(joueur.last_spawn, now);

    if (elapsed>=BALANCE(TEMPS_ENNE_R) && getSecInt(joueur.debut, now)>=BALANCE(START_SPAWN))
    {
                            //on ne teste qu'une fois avec l'aléatoire
        x = rand()%MAPSIZEX;
        y = rand()%MAPSIZEY;

        if (!carte[x][y].block && !trouve(ancre, x*COTE+COTE/2, y*COTE+COTE/2, NULL, NEUTR)) //on regarde dabords si la place est libre
        {
            a = 1;
            for (i=-BALANCE(SAFEZONE);i<=BALANCE(SAFEZONE);i++) //on regarde tout autours si c'est pas trop proche d'un batiment allié
            {
                for (j=-BALANCE(SAFEZONE);j<=BALANCE(SAFEZONE);j++)
                {
                    if (x+i>=0 && x+i<MAPSIZEX && y+j>=0 && y+j<MAPSIZEY)
                    {
                        if (carte[x+i][y+j].type==BUILDING && !if_dist(0, 0, i, j, BALANCE(SAFEZONE)))
                            a = 0;
                    }
                }
            }
            x*=COTE;
            y*=COTE;
        }


        if (a)
        {
            ancre.push_back(new Unit(ENEMY, STAND, x, y, NULL));

            joueur.last_spawn = now; //on remet à zero le timer du spawn
        }
    }

}

//dirige le paysant vers la prochaine ressource. renvoie si ça a marché ou pas
int next_res(Unit& unite, Tile carte[MAPSIZEX][MAPSIZEY])
{
    int i, j, k;
    int rep = 0, x, y;
    char recherche;


    if (unite.prod && unite.state==MINE)
    {
        //si y'a pas déjà des ressources à l'arrivée
        if (!carte[DIV(unite.xdest)][DIV(unite.ydest)].res)
        {
            switch (unite.prec) //on regarde ce qu'on cherche (arbre ou rocher?)*
            {
                case CHOP:
                default:
                recherche = TREE;
            break;

                case MINE:
                recherche = ROCK;
            break;
            }

            x = DIV(unite.x + unite.cote/2); //on pprend la case sur laquelle l'unité est
            y = DIV(unite.y + unite.cote/2);

            for (k=1;k<=unite.vision;k++) //on veut dabords regarder les cases autours puis celles plus eloignées
            {
                for (i=-k;i<=k;i++) //on regarde toutes les cases autours
                {
                    for (j=-k;j<=k;j++)
                    {
                        if (x+i>=0 && x+i<MAPSIZEX && y+j>=0 && y+j<MAPSIZEY) //ptit blindage
                        {
                            if (carte[x+i][y+j].type==recherche && carte[x+i][y+j].res) //si c'est ce qu'on cherche et qu'il y a de la ressource (on veut pas des feuilles nous)
                            {
                                if (if_dist(x, y, x+i, y+j, k))
                                {
                                    unite.xdest = (x+i)*COTE + COTE/2;
                                    unite.ydest = (y+j)*COTE + COTE/2;
                                    unite.state = MOVING;
                                    unite.prec = MOVING;
                                    unite.priority = AUTO;
                                    unite.changepath = 1;
                                    rep = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (rep)
                        break;
                }
                if (rep)
                    break;
            }

        }
        else
        {
            rep = 1;
        }

    }
    else
    {
        rep = 0;
        unite.state = MOVING;
        unite.prec = MOVING;
        unite.changepath = 1;
    }

    return rep;
}

//pour une unité ennemie, regarde quoi faire
void automat(list<Unit *>& ancre, list<Build *>& ancre_b, Tile carte[MAPSIZEX][MAPSIZEY], Joueur& joueur, Unit& unite)
{
    int i, j;
    int a, b, dist, x, y, xbatdest = 0, ybatdest = 0, xtile, ytile;
    Unit *inter;
    Build* bat;
    Tile tuile;
    int dist_prev = 1000000; ///À remplacer par valeur max int
    int dist_cur = 0;
    int xgo, ygo;
    bool batfound;

    list<Build *>::iterator iter_b;

    if (unite.side==ENEMY)
    {
        if (unite.priority<=GUARD)
        {
            xtile = DIV(unite.x+unite.cote/2);
            ytile = DIV(unite.y+unite.cote/2);

            if (unite.bat && !if_dist(xtile, ytile, unite.bat->x+unite.bat->w/2,  unite.bat->y+unite.bat->h/2, GUARD_DIST))
            {
                xbatdest = unite.bat->x+unite.bat->w/2;
                ybatdest = unite.bat->y+unite.bat->h/2;

                if (xtile>(xbatdest+DEFRANGE))
                    x = xbatdest + DEFRANGE;
                else if (xtile<(xbatdest-DEFRANGE))
                    x = xbatdest - DEFRANGE;
                else
                    x = xtile;

                if (ytile>ybatdest+DEFRANGE)
                    y = ybatdest + DEFRANGE;
                else if (ytile<ybatdest-DEFRANGE)
                    y = ybatdest - DEFRANGE;
                else
                    y = ytile;


                unite.xdest = x*COTE + COTE/2;
                unite.ydest = y*COTE + COTE/2;
                unite.changepath = 1;

                unite.priority = GUARD+1;
            }
            else
            {
                a = 1;

                dist = unite.vision*COTE + 1; //la distance prise initialement est superieure à la portée de l'unité

                for (auto& elem : ancre) //check d'unités opposantes à attaquer
                {
                    inter = elem;

                    if (unite.side!=inter->side)
                    {
                        b = sqrt(pow(unite.x-inter->x, 2) + pow(unite.y-inter->y, 2)); //il faut ajouter +unite.cote/2 à chaque fois si on fait des unités de taille differente
                            //faudrait enlever sqrt...
                        if (b<dist)
                        {
                            dist = b;
                            x = inter->x + inter->cote/2;
                            y = inter->y + inter->cote/2;
                            a = 0;
                        }
                    }
                }


                if (a) //check de batiments opposants à attaquer
                {

                    dist = unite.vision + 1;

                    for (i=-unite.vision;i<=unite.vision;i++)
                    {
                        for (j=-unite.vision;j<=unite.vision;j++)
                        {
                            if (xtile+i>=0 && xtile+i<MAPSIZEX && ytile+j>=0 && ytile+j<MAPSIZEY) //ptit blindage pour eviter les segmentaion faults
                            {
                                tuile = carte[xtile+i][ytile+j];
                                if (tuile.erige)
                                {
                                    if (tuile.erige->side!=unite.side)
                                    {
                                        b = sqrt(pow(i, 2) + pow(j, 2));

                                        if (b<dist)
                                        {
                                            dist = b;
                                            x = unite.x+unite.cote/2 + COTE*i;
                                            y = unite.y+unite.cote/2 + COTE*j;
                                            a = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (!a) //si on a trouvé quelquechose à attaquer
                {
                    unite.xdest = x;
                    unite.ydest = y;
                    unite.changepath = 1;

                    if (unite.priority<GUARD)
                        unite.priority = AUTO;
                            //on utilise pas dist parceque ça peut etre faux (dist est pas exact pour les batiments)
                    if ( (pow(x - (unite.x+unite.cote/2), 2) + pow(y - (unite.y+unite.cote/2), 2)) < pow(unite.range, 2))
                    {
                        unite.state = ATTACK;
                        unite.prec = ATTACK;

                        if (unite.priority==GUARD)
                            unite.priority++;
                    }
                    else
                    {
                        unite.state = MOVING;
                        unite.prec = MOVING;
                        unite.changepath = 1;
                    }
                } //si on a rien trouvé à attaquer et que c'est une unité qui roam
                else if (unite.priority<=AUTO)//tests pour le deplacement vers les batiments opposés
                {
                    xgo = unite.x;
                    ygo = unite.y;

                    for (iter_b = ancre_b.begin();iter_b!=ancre_b.end();iter_b++)
                    {
                        batfound = false;

                        bat = *iter_b;

                        if(bat->side!=unite.side)
                        {
                            batfound = true;
                            xbatdest = bat->x*COTE;
                            ybatdest = bat->y*COTE;
                            dist_cur = sqrt(pow(xbatdest-unite.x, 2) + pow(ybatdest-unite.y, 2));
                        }

                        if(batfound && dist_cur<dist_prev)
                        {
                            dist_prev=dist_cur;
                            xgo=xbatdest;
                            ygo=ybatdest;
                        }
                    }

//                    if (!(unite.x>=xgo-10 && unite.x<=xgo+160 && unite.y>=ygo-10 && unite.y<=ygo+160))
//                    //((unite.x>xgo+155 ||unite.x<xgo-5) && (unite.y>ygo+155 ||unite.y<ygo-5))
//                    {
                    if (unite.xdest!=xgo || unite.ydest!=ygo)
                    {
                        unite.xdest = xgo;
                        unite.ydest = ygo;
                        unite.state = MOVING;
                        unite.prec = MOVING;
                        unite.changepath = 1;
                    }

//                    }
                }
            }
        }
    }
}

//fin du fichier

