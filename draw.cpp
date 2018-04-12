#include "draw.h"

///AFFICHER LES DONNÉES D'UN BATIMENT APRES LES CASES

using namespace std;

//dessine une "barre de chargement" pour l'hp etc
BITMAP *draw_status(double HPmax, double HPcurrent, int type)
{
    float bar_per_w;
    int col, colVal;
    BITMAP *rep = create_bitmap(BAR_W, BAR_H);

    rectfill(rep,0,0,BAR_W, BAR_H, MAG);

    bar_per_w = HPcurrent/HPmax;

    if (bar_per_w>1)
        bar_per_w = 1;

    colVal = (1-bar_per_w)*255;

    switch (type)
    {
        case BAT:
        col = makecol(colVal, 0, bar_per_w * 255);
    break;

        case UNIT:
        col = makecol(colVal, bar_per_w * 255, 0);
    break;

        case TIME:
        col = makecol(colVal, colVal, colVal);
    break;

        default:
        col = makecol(0, 0, 0);
    break;
    }


    rectfill(rep, 1,1,bar_per_w*BAR_W, BAR_H-1, col);
    rect(rep,0,0,BAR_W, BAR_H, NOIR);


    return rep;
}

//dessine l'UI avec les differents trucs dessus
void draw_ui(BITMAP *dest, list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Sprites& sprites, Joueur& joueur)
{
    int i, j;
    BITMAP *temp;
    int p = 0, a = 0;

    //TIMESTRUCT prev;

    //getTime(prev); ///useless?

    if (joueur.act==SELECTED || joueur.act==PLACE_BUILD)
    {
        for (auto& elem : joueur.selection)
        {
            if (elem->type==PEASANT)
            {
                a = 1;
                p = PEASANT;
            }
        }
    }
    else if(joueur.act==SELECT_BUILD)
    {
        a = 1;
        p = joueur.type;
    }


    masked_blit(sprites.ign_l, dest, 0, 0, 0, 0, MOVELIMIT, UI_HEIGHT);

    rectfill(dest, MOVELIMIT, 0, MAPWIDTH + MOVELIMIT, UI_HEIGHT, COL_UI_ACC); //là où y'aura la minimap

    draw_sprite(dest, sprites.c_bar, MAPWIDTH + MOVELIMIT, 0);//l'info d'un personage

    //on affiche les icones des unités séléctionnées
    if (joueur.act==SELECTED || joueur.act==PLACE_BUILD)
    {
        j = 0;
        i = 0;

        for (auto& elem : joueur.selection)
        {
            switch (elem->type)
            {
                case SOLDIER:
                draw_sprite(dest, sprites.epee_i, MAPWIDTH + MOVELIMIT + 25 + i*ICON_S, j*ICON_S);
            break;

                default:
                case PEASANT:
                draw_sprite(dest, sprites.paysant_i, MAPWIDTH + MOVELIMIT + 25 + i*ICON_S, j*ICON_S);
            break;
            }

            i++;
            if (i>=15)
            {
                i = 0;
                j++;
            }

            if (j>=5)
                break;
        }
    }
    else if (joueur.act==SELECT_BUILD)
    {
        switch (joueur.type)
        {
            default:
            case MAIRIE:
                draw_sprite(dest, sprites.mairie_i, MAPWIDTH + MOVELIMIT + 25, 0);
        break;

            case CASERNE:
                draw_sprite(dest, sprites.caserne_i, MAPWIDTH + MOVELIMIT + 25, 0);
        break;
        }
    }

    //si on est en train de cliquer sur le bouton pause
    if (mouse_b & 1 && mouse_x>=(SEPARE-PAUSE_S) && mouse_x<SEPARE && mouse_y<(ECRANY*COTE + PAUSE_S) && mouse_y>=(ECRANY*COTE))
        draw_sprite(dest, sprites.pause_i[1], SEPARE-PAUSE_S, 0);
    else
        draw_sprite(dest, sprites.pause_i[0], SEPARE-PAUSE_S, 0);

    if (a)
        draw_sprite(dest, sprites.small_menu[p], SEPARE, 0);//le menu unité
    else
        draw_sprite(dest, sprites.r_bar, SEPARE, 0);



    temp = minimap(ancre, carte, joueur, 0);
    blit(temp, dest, 0, 0, MOVELIMIT, 0, MAPSIZEX, MAPSIZEY);
    destroy_bitmap(temp);
}

//dessine la minimap
BITMAP *minimap(list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Joueur& joueur, int vis)
{
    int i, j;
    BITMAP *rep = create_bitmap(MAPWIDTH, UI_HEIGHT);
    int col, x, y;

    //on dessine en fonction de la case
    for (i=0;i<MAPSIZEX;i++)
    {
        for (j=0;j<MAPSIZEY;j++)
        {
            switch (carte[i][j].type)
            {
                default:
                case GRASS:
                switch (carte[i][j].position)
                {
                    case P_HERBE:
                    col = COL_GRASS;
                break;
                    case P_PONT:
                    col = COL_BRIDGE;
                break;
                    default:
                    case P_SABLE:
                    col = COL_SAND;
                break;
                }
            break;

                case WATER:
                col = COL_WATER;
            break;

                case MOUNTAIN:
                col = COL_MOUNTAIN;
            break;

                case TREE:
                switch (carte[i][j].position)
                {
                    case 2: //conifere partie basse
                    case 3: //conifere partie haute
                    col = COL_TREEP;
                break;
                    default: //le reste. Normalement que des feuillus
                    col = COL_TREEL;
                break;
                }
            break;

                case ROCK:
                col = COL_ROCK;
            break;

                case BUILDING:
                col = COL_CONSTR;
            break;

                case CAMP:
                col = COL_ENEMY;
            break;
            }

            if (!TEST && !carte[i][j].visible && !vis)
                col = NOIR;

            rectfill(rep, i, j, i+1, j+1, col);
        }
    }

    //on dessine les unités qu'il y a dessus
    for (auto& elem : ancre)
    {
        x = DIV(elem->m_pos.x);
        y = DIV(elem->m_pos.y);
        switch (elem->type)
        {
            case ENEMY:
            if (carte[x][y].visible || TEST)
                col = COL_ENEMY;
            else
                col = NOIR;
        break;
            case SOLDIER:
            col = COL_SOLDIER;
        break;
            default:
            case PEASANT:
            col = COL_PEASANT;
        break;
        }
        rectfill(rep, x, y, x+1, y+1, col);
    }

    rect(rep, DIV(joueur.xcamera), DIV(joueur.ycamera), (DIV(joueur.xcamera) + joueur.xecran), (DIV(joueur.ycamera) + joueur.yecran), BLANC);

    return rep;
}

//la boite en haut à droite de l'écran avec les ressources...
BITMAP *game_info(Joueur& joueur, Sprites& sprites)
{
    BITMAP *rep;
    TIMESTRUCT nouveau;
    int sec, minutes;

    getTime(nouveau);

    sec = getSecInt(joueur.debut, nouveau);
    minutes = (int) (sec/60);
    sec %= 60;

    rep = create_bitmap(ISIZEX, ISIZEY);

    draw_sprite(rep, sprites.info_bar, 0, 0);

    draw_sprite(rep, sprites.bois_i, XBOISI, YICON);
    textprintf_ex(rep, font, XBOIST, YTEXT, COL_UI_ACC, -1, ": %d", joueur.bois);
    draw_sprite(rep, sprites.pierre_i, XPIERREI, YICON);
    textprintf_ex(rep, font, XPIERRET, YTEXT, COL_UI_ACC, -1, ": %d", joueur.marbre);
    draw_sprite(rep, sprites.viande_i, XVIANDEI, YICON);
    textprintf_ex(rep, font, XVIANDET, YTEXT, COL_UI_ACC, -1, ": %d", joueur.viande);

    if (joueur.nend_b)
    {
        draw_sprite(rep, sprites.camp_i, XREMAINI, YICON);
        textprintf_ex(rep, font, XREMAINT, YTEXT, COL_UI_ACC, -1, ": %d", joueur.nend_b);
    }
    else
    {
        draw_sprite(rep, sprites.enemy_i, XREMAINI, YICON);
        textprintf_ex(rep, font, XREMAINT, YTEXT, COL_UI_ACC, -1, ": %d", joueur.nend_e);
    }

    textprintf_ex(rep, font, XTEMPS, YTEXT, COL_UI_ACC, -1, "%d%s%d", minutes, (sec<10)?":0":":",sec);


    return rep;
}

//dessine tout sur la bitmap dest
void draw_screen(BITMAP *dest, list<Unit *>& ancre, list<Build *>& ancre_b, Tile carte[MAPSIZEX][MAPSIZEY], Sprites& sprites, Joueur& joueur)
{
    int i, j;
    Unit *inter = NULL;
    Build *bat = NULL;
    Tile chaque;
    BITMAP *raw, *temp;
    double time_max, time_current;
    TIMESTRUCT now;

    int xval, yval;

    //min est la borne des cases à déssiner
    int xmin, ymin, xtaille, ytaille, xo, yo; //xo et yo sont l'orignine, là à partir d'où on va déssiner sur l'écran

    //raw est une bitmaps qui peut etre plus ou moins grande et qui sera stretched blit sur le buffer
    raw = create_bitmap(joueur.xecran*COTE, joueur.yecran*COTE);

    xmin = DIV(joueur.xcamera);
    ymin = DIV(joueur.ycamera);

    xtaille = joueur.xecran; //x/y taille sont les "tailles de l'écran" en prenant en compte que l'éran ne tombe pas forcement pile sur une case
    // si xcamera ne tombe pas sur un multiple de COTE
    if (xmin*COTE!=joueur.xcamera)
        xtaille++;

    ytaille = joueur.yecran;
    if (ymin*COTE!=joueur.ycamera)
        ytaille++;

    xo = -(joueur.xcamera%COTE);
    yo = -(joueur.ycamera%COTE);


    if (joueur.change)
    {
        if (joueur.chang_taill)
        {
            destroy_bitmap(sprites.prev);
            sprites.prev = create_bitmap(joueur.xecran*COTE, joueur.yecran*COTE);
            joueur.chang_taill = 0;
        }

        //affichage des tuiles
        for (i=0;i<xtaille;i++)
        {
            for (j=0;j<ytaille;j++)
            {
                chaque = carte[i+xmin][j+ymin];
                switch (chaque.type)
                {
                    case TREE:
                    draw_sprite(sprites.prev, sprites.arbre[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case ROCK:
                    draw_sprite(sprites.prev, sprites.pierre[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case WATER:
                    draw_sprite(sprites.prev, sprites.eau[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case MOUNTAIN:
                    draw_sprite(sprites.prev, sprites.montagne[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    default:
                    case GRASS:
                    draw_sprite(sprites.prev, sprites.herbe[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case BUILDING:
                    draw_sprite(sprites.prev, sprites.batiment[0][chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case CAMP:
                    draw_sprite(sprites.prev, sprites.camp[0][chaque.position], xo + i*COTE, yo + j*COTE);
                break;
                }

                if (chaque.fleur)
                    draw_sprite(sprites.prev, sprites.flower[chaque.fleur], xo + i*COTE, yo + j*COTE);

            }
        }
        joueur.change = 0;
    }

    blit(sprites.prev, raw, 0, 0, 0, 0, joueur.xecran*COTE, joueur.yecran*COTE);


    //affichage du nombre de ressources
    for (i=0;i<xtaille;i++)
    {
        for (j=0;j<ytaille;j++)
        {
            if (carte[i+xmin][j+ymin].res)
                textprintf_ex(raw, font, xo + i*COTE+10, yo + j*COTE+60, VERT, -1, "%d", carte[i+xmin][j+ymin].res);
        }
    }


    if (joueur.act==PLACE_BUILD) //on montre l'aperçu du batiment à construire
    {
        xval = (mouse_x * ((float)joueur.xecran/ECRANX)) + joueur.xcamera;
        xval = DIV(xval);
        xval-= xmin;

        yval = (mouse_y * ((float)joueur.yecran/ECRANY)) + joueur.ycamera;
        yval = DIV(yval);
        yval-= ymin;

        for (i=0;i<2;i++)
        {
            for (j=0;j<2;j++)
            {
                draw_sprite(raw, sprites.batiment[0][joueur.type*4+(i+2*j)], xo + (xval+i)*COTE, yo + (yval+j)*COTE);
            }
        }
    }


    //affichage des differentes infos des batiments
    for (auto& elem : ancre_b)
    {
        bat = elem;
        if ((bat->m_pos.x+bat->w)>=xmin && (bat->m_pos.y+bat->h)>=ymin && bat->m_pos.x<=(xmin+xtaille) && bat->m_pos.y<=(ymin+ytaille))
        {
            temp = draw_status(bat->hp_max, bat->hp, BAT);
            draw_sprite(raw, temp, xo + (bat->m_pos.x-xmin)*COTE + 37, yo + (bat->m_pos.y-ymin)*COTE);
            destroy_bitmap(temp);

            if (bat->curr_queue)
            {
                time_max = (bat->unit_queue[0]==PEASANT)?TEMPS_PAYS:TEMPS_SOLD;
                getTime(now);
                time_current = getSec(bat->start, now);

                temp = draw_status(time_max, time_current, TIME);
                draw_sprite(raw, temp, xo + (bat->m_pos.x-xmin)*COTE + 37, yo + (bat->m_pos.y-ymin)*COTE - 20);
                destroy_bitmap(temp);

                textprintf_ex(raw, font, xo + (bat->m_pos.x-xmin)*COTE +30, yo + (bat->m_pos.y-ymin)*COTE - 15, ROUGE, -1, "%d", bat->curr_queue);
            }
        }
    }



    //affichage des unités
    for (auto& elem : ancre)
    {
        inter = elem;
        if (inter->state!=DEAD)
        {
            xval = inter->m_pos.x;
            yval = inter->m_pos.y;
            if ((xval/COTE)>=xmin && (yval/COTE)>=ymin && (xval/COTE)<=(xmin+xtaille) && (yval/COTE)<=(ymin+ytaille))
            {
                switch (inter->type)
                {
                    default:
                    case PEASANT:
                    temp = sprites.peasant[inter->prec][inter->frame];
                break;

                    case SOLDIER:
                    temp = sprites.sword[inter->prec][inter->frame];
                break;

                    case ENEMY:
                    temp = sprites.ennemi[inter->prec][inter->frame];
                break;
                }

                switch (inter->direction)
                {
                    case RIGHT:
                    default:
                    draw_sprite(raw, temp, inter->m_pos.x-joueur.xcamera, inter->m_pos.y-joueur.ycamera);
                break;

                    case LEFT:
                    draw_sprite_h_flip(raw, temp, inter->m_pos.x-joueur.xcamera, inter->m_pos.y-joueur.ycamera);
                break;
                }

                temp = draw_status((double)inter->hp_max, (double)inter->hp, UNIT);
                draw_sprite(raw, temp, inter->m_pos.x - joueur.xcamera, inter->m_pos.y-10-joueur.ycamera);
                destroy_bitmap(temp);
            }
        }
    }



    //affichage différent pour les unités selectionnées
    for (auto& elem : joueur.selection)
    {
        inter = elem;
        if (inter->state!=DEAD)
        {
            if ((inter->m_pos.x/COTE)>=xmin && (inter->m_pos.y/COTE)>=ymin && (inter->m_pos.x/COTE)<=(xmin+xtaille) && (inter->m_pos.y/COTE)<=(ymin+ytaille))
            {
                rect(raw, inter->m_pos.x - joueur.xcamera, inter->m_pos.y-joueur.ycamera, inter->m_pos.x+inter->cote-joueur.xcamera, inter->m_pos.y+inter->cote-joueur.ycamera, NOIR);
            }

        }
    }


    //affichage du brouillard de guerre (partie cachée non explorée)
    if (!TEST)
    {
        for (i=0;i<xtaille;i++)
        {
            for (j=0;j<ytaille;j++)
            {
                if (!carte[i+xmin][j+ymin].visible) //si on ne peut pas voir la tuile
                {
                    blit(sprites.brouillard, raw, 0, 0, xo + i*COTE, yo + j*COTE, COTE, COTE);
                }
            }
        }
    }

    if (joueur.act==SELECTING)
    {
        rect(raw, joueur.xprev-joueur.xcamera, joueur.yprev-joueur.ycamera, mouse_x*((float)joueur.xecran/ECRANX), mouse_y*((float)joueur.yecran/ECRANY), VERT);
    }


    stretch_blit(raw, dest, 0, 0, joueur.xecran*COTE, joueur.yecran*COTE,0, 0, ECRANX*COTE, ECRANY*COTE);
    destroy_bitmap(raw);


    temp = create_bitmap(ECRANX*COTE, UI_HEIGHT);
    draw_ui(temp, ancre, carte, sprites, joueur);
    blit(temp, dest, 0, 0, 0, ECRANY*COTE, ECRANX*COTE, UI_HEIGHT);
    destroy_bitmap(temp);


    temp = game_info(joueur, sprites);
    blit(temp, dest, 0, 0, ECRANX*COTE - ISIZEX, 0, ISIZEX, ISIZEY);
    destroy_bitmap(temp);


    //le rectangle en bas sur l'ecran pour pouvoir "scroll"
    masked_blit(sprites.ign_d, dest, 0, 0, 0, YSCREEN - MOVELIMIT, XSCREEN, MOVELIMIT);

    //on dessine la souris
    draw_sprite(dest, sprites.souris, mouse_x, mouse_y);
}


//fin du fichier
