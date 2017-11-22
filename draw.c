#include "draw.h"

///AFFICHER LES DONN2ES D'UN BATIMENT APRES LES CASES

//dessine une "barre de chargement" pour l'hp etc
BITMAP *draw_status(double HPmax, double HPcurrent, int type)
{
    float bar_per_w;
    int col;
    BITMAP *rep = create_bitmap(BAR_W, BAR_H);

    rectfill(rep,0,0,BAR_W, BAR_H, MAG);

    bar_per_w = HPcurrent/HPmax;

    if (bar_per_w>1)
        bar_per_w = 1;

    switch (type)
    {
        case BAT:
        col = makecol((1-bar_per_w)*255, 0, bar_per_w * 255);
    break;

        case UNIT:
        col = makecol((1-bar_per_w)*255, bar_per_w * 255,0);
    break;

        case TIME:
        col = makecol((1-bar_per_w)*255, (1-bar_per_w)*255, (1-bar_per_w)*255);
    break;
    }


    rectfill(rep, 1,1,bar_per_w*BAR_W, BAR_H-1, col);
    rect(rep,0,0,BAR_W, BAR_H, NOIR);


    return rep;
}

//dessine l'UI avec les differents trucs dessus
void draw_ui(BITMAP *dest, Ancre ancre, Tile carte[MAPSIZEX][MAPSIZEY], Sprites *sprites, Joueur joueur)
{
    int i, j;
    BITMAP *temp;
    Maillon *inter;
    int p=0, a=0;
    struct timespec prev;
    clock_gettime(CLOCK_MONOTONIC, &prev);

    if (joueur.act==SELECTED || joueur.act==PLACE_BUILD)
    {
        inter = joueur.selection.debut;
        while (inter!=NULL)
        {
            if (inter->unite->type==PEASANT)
            {
                a = 1;
                p = PEASANT;
            }

            inter = inter->next;
        }
    }
    else if(joueur.act==SELECT_BUILD)
    {
        a = 1;
        p = joueur.type;
    }


    masked_blit(sprites->ign_l, dest, 0, 0, 0, 0, MOVELIMIT, UI_HEIGHT);

    rectfill(dest, MOVELIMIT, 0, MAPWIDTH + MOVELIMIT, UI_HEIGHT, COL_UI_ACC); //là où y'aura la minimap

    draw_sprite(dest, sprites->c_bar, MAPWIDTH + MOVELIMIT, 0);//l'info d'un personage

    if (joueur.act==SELECTED || joueur.act==PLACE_BUILD)
    {
        inter = joueur.selection.debut;
        j = 0;
        while (j<5 && inter!=NULL)
        {
            i = 0;
            while (i<15 && inter!=NULL)
            {
                switch (inter->unite->type)
                {
                    case SOLDIER:
                    draw_sprite(dest, sprites->epee_i, MAPWIDTH + MOVELIMIT + 25 + i*ICON_S, j*ICON_S);
                break;

                    case PEASANT:
                    draw_sprite(dest, sprites->paysant_i, MAPWIDTH + MOVELIMIT + 25 + i*ICON_S, j*ICON_S);
                break;
                }

                inter = inter->next;
                i++;
            }
            j++;
        }
    }
    else if (joueur.act==SELECT_BUILD)
    {
        switch (joueur.type)
        {
            case MAIRIE:
                draw_sprite(dest, sprites->mairie_i, MAPWIDTH + MOVELIMIT + 25, 0);
        break;

            case CASERNE:
                draw_sprite(dest, sprites->caserne_i, MAPWIDTH + MOVELIMIT + 25, 0);
        break;
        }
    }

    //si on est en train de cliquer sur le bouton pause
    if (mouse_b & 1 && mouse_x>=(SEPARE-PAUSE_S) && mouse_x<SEPARE && mouse_y<(ECRANY*COTE + PAUSE_S) && mouse_y>=(ECRANY*COTE))
        draw_sprite(dest, sprites->pause_i[1], SEPARE-PAUSE_S, 0);
    else
        draw_sprite(dest, sprites->pause_i[0], SEPARE-PAUSE_S, 0);

    if (a)
        draw_sprite(dest, sprites->small_menu[p], SEPARE, 0);//le menu unité
    else
        draw_sprite(dest, sprites->r_bar, SEPARE, 0);



    temp = minimap(ancre, carte, joueur, 0);
    blit(temp, dest, 0, 0, MOVELIMIT, 0, MAPSIZEX, MAPSIZEY);
    destroy_bitmap(temp);


}

//dessine la minimap
BITMAP *minimap(Ancre ancre, Tile carte[MAPSIZEX][MAPSIZEY], Joueur joueur, int vis)
{
    int i, j;
    BITMAP *rep = create_bitmap(MAPWIDTH, UI_HEIGHT);
    Maillon *inter = ancre.debut;
    int col, x, y;

    for (i=0;i<MAPSIZEX;i++)
    {
        for (j=0;j<MAPSIZEY;j++)
        {
            switch (carte[i][j].type)
            {
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
                    case 2:
                    case 3:
                    col = COL_TREEP;
                break;
                    default:
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

    while (inter!=NULL)
    {
        x = DIV(inter->unite->x);
        y = DIV(inter->unite->y);
        switch (inter->unite->type)
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
            case PEASANT:
            col = COL_PEASANT;
        break;
        }
        rectfill(rep, x, y, x+1, y+1, col);

        inter = inter->next;
    }

    rect(rep, DIV(joueur.xcamera), DIV(joueur.ycamera), (DIV(joueur.xcamera) + joueur.xecran), (DIV(joueur.ycamera) + joueur.yecran), BLANC);

    return rep;
}

//la boite en haut à droite de l'écran avec les ressources...
BITMAP *game_info(Joueur joueur, Sprites *sprites)
{
    BITMAP *rep;
    struct timespec nouveau;
    int sec, min;

    clock_gettime(CLOCK_MONOTONIC, &nouveau);

    sec = nouveau.tv_sec - joueur.debut.tv_sec;
    min = (int) (sec/60);
    sec %= 60;

    rep = create_bitmap(ISIZEX, ISIZEY);

    draw_sprite(rep, sprites->info_bar, 0, 0);

    draw_sprite(rep, sprites->bois_i, XBOISI, YICON);
    textprintf_ex(rep, font, XBOIST, YTEXT, COL_UI_ACC, -1, ": %d", joueur.bois);
    draw_sprite(rep, sprites->pierre_i, XPIERREI, YICON);
    textprintf_ex(rep, font, XPIERRET, YTEXT, COL_UI_ACC, -1, ": %d", joueur.marbre);
    draw_sprite(rep, sprites->viande_i, XVIANDEI, YICON);
    textprintf_ex(rep, font, XVIANDET, YTEXT, COL_UI_ACC, -1, ": %d", joueur.viande);

    if (joueur.nend_b)
    {
        draw_sprite(rep, sprites->camp_i, XREMAINI, YICON);
        textprintf_ex(rep, font, XREMAINT, YTEXT, COL_UI_ACC, -1, ": %d", joueur.nend_b);
    }
    else
    {
        draw_sprite(rep, sprites->enemy_i, XREMAINI, YICON);
        textprintf_ex(rep, font, XREMAINT, YTEXT, COL_UI_ACC, -1, ": %d", joueur.nend_e);
    }

    textprintf_ex(rep, font, XTEMPS, YTEXT, COL_UI_ACC, -1, "%d%s%d", min, (sec<10)?":0":":",sec);


    return rep;
}

//dessine tout sur la bitmap dest
void draw_screen(BITMAP *dest, Ancre ancre, Ancre_b ancre_b, Tile carte[MAPSIZEX][MAPSIZEY], Sprites *sprites, Joueur *joueur)
{
    int i, j;
    Unit *inter = NULL;
    Build *bat = NULL;
    Maillon *maill;
    Maillon_b *maill_b;
    Tile chaque;
    BITMAP *raw, *temp, *fog;
    double time_max, time_current;
    struct timespec now;

    int xval, yval;

    int getim;
    int getdest;
    int getbuf;

    int rim, gim, bim, rdest, gdest, bdest;

    int xfog, yfog;

    //min est la borne des cases à déssiner
    int xmin, ymin, xtaille, ytaille, xo, yo; //xo et yo sont l'orignine, là à partir d'où on va déssiner sur l'écran

    //raw est une bitmaps qui peut etre plus ou moins grande et qui sera stretched blit sur le buffer
    raw = create_bitmap(joueur->xecran*COTE, joueur->yecran*COTE);

    xmin = DIV(joueur->xcamera);
    ymin = DIV(joueur->ycamera);

    xtaille = joueur->xecran; //x/y taille sont les "tailles de l'écran" en prenant en compte que l'éran ne tombe pas forcement pile sur une case
    if (xmin!=(float)joueur->xcamera/COTE)
        xtaille++;

    ytaille = joueur->yecran;
    if (ymin!=(float)joueur->ycamera/COTE)
        ytaille++;

    xo = -(joueur->xcamera%COTE);
    yo = -(joueur->ycamera%COTE);


    DEB("3-0")
    if (joueur->change)
    {
        DEB("3-0-0")
        if (joueur->chang_taill)
        {
            DEB("3-0-1")
            destroy_bitmap(sprites->prev);
            sprites->prev = create_bitmap(joueur->xecran*COTE, joueur->yecran*COTE);
            joueur->chang_taill = 0;

            DEB("3-0-2")
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
                    draw_sprite(sprites->prev, sprites->arbre[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case ROCK:
                    draw_sprite(sprites->prev, sprites->pierre[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case WATER:
                    draw_sprite(sprites->prev, sprites->eau[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case MOUNTAIN:
                    draw_sprite(sprites->prev, sprites->montagne[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case GRASS:
                    default:
                    draw_sprite(sprites->prev, sprites->herbe[chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case BUILDING:
                    draw_sprite(sprites->prev, sprites->batiment[0][chaque.position], xo + i*COTE, yo + j*COTE);
                break;

                    case CAMP:
                    draw_sprite(sprites->prev, sprites->camp[0][chaque.position], xo + i*COTE, yo + j*COTE);
                break;
                }

                if (chaque.fleur)
                    draw_sprite(sprites->prev, sprites->flower[chaque.fleur], xo + i*COTE, yo + j*COTE);

            }
        }
        joueur->change = 0;
    }

    blit(sprites->prev, raw, 0, 0, 0, 0, joueur->xecran*COTE, joueur->yecran*COTE);

    DEB("3-01")

    //affichage du nombre de ressources
    for (i=0;i<xtaille;i++)
    {
        for (j=0;j<ytaille;j++)
        {
            if (carte[i+xmin][j+ymin].res)
                textprintf_ex(raw, font, xo + i*COTE+10, yo + j*COTE+60, VERT, -1, "%d", carte[i+xmin][j+ymin].res);
        }
    }

    DEB("3-1")

    if (joueur->act==PLACE_BUILD) //on montre l'aperçu du batiment à construire
    {
        xval = (mouse_x * ((float)joueur->xecran/ECRANX)) + joueur->xcamera;
        xval = DIV(xval);
        xval-= xmin;

        yval = (mouse_y * ((float)joueur->yecran/ECRANY)) + joueur->ycamera;
        yval = DIV(yval);
        yval-= ymin;

        for (i=0;i<2;i++)
        {
            for (j=0;j<2;j++)
            {
                draw_sprite(raw, sprites->batiment[0][joueur->type*4+(i+2*j)], xo + (xval+i)*COTE, yo + (yval+j)*COTE);
            }
        }
    }

    DEB("3-2")

    maill_b = ancre_b.debut;
    //affichage des differentes infos des batiments
    while (maill_b!=NULL)
    {
        bat = maill_b->batiment;
        if ((bat->x+bat->w)>=xmin && (bat->y+bat->h)>=ymin && bat->x<=(xmin+xtaille) && bat->y<=(ymin+ytaille))
        {
            temp = draw_status(bat->hp_max, bat->hp, BAT);
            draw_sprite(raw, temp, xo + (bat->x-xmin)*COTE + 37, yo + (bat->y-ymin)*COTE);
            destroy_bitmap(temp);

            if (bat->curr_queue)
            {
                time_max = (bat->unit_queue[0]==PEASANT)?TEMPS_PAYS:TEMPS_SOLD;
                clock_gettime(CLOCK_MONOTONIC, &now);
                time_current = (now.tv_sec - bat->start.tv_sec) + 1e-9 * (now.tv_nsec - bat->start.tv_nsec);

                temp = draw_status(time_max, time_current, TIME);
                draw_sprite(raw, temp, xo + (bat->x-xmin)*COTE + 37, yo + (bat->y-ymin)*COTE - 20);
                destroy_bitmap(temp);

                textprintf_ex(raw, font, xo + (bat->x-xmin)*COTE +30, yo + (bat->y-ymin)*COTE - 15, ROUGE, -1, "%d", bat->curr_queue);
            }
        }
        maill_b = maill_b->next;
    }

    DEB("3-3")

    maill = ancre.debut;
    //affichage des unités
    while (maill!=NULL)
    {
        inter = maill->unite;
        if (inter->state!=DEAD)
        {
            if ((inter->x/COTE)>=xmin && (inter->y/COTE)>=ymin && (inter->x/COTE)<=(xmin+xtaille) && (inter->y/COTE)<=(ymin+ytaille))
            {
                switch (inter->type)
                {
                    case PEASANT:
                    temp = sprites->peasant[inter->prec][inter->frame];
                break;

                    case SOLDIER:
                    temp = sprites->sword[inter->prec][inter->frame];
                break;

                    case ENEMY:
                    temp = sprites->ennemi[inter->prec][inter->frame];
                break;
                }

                switch (inter->direction)
                {
                    case RIGHT:
                    default:
                    draw_sprite(raw, temp, inter->x-joueur->xcamera, inter->y-joueur->ycamera);
                break;

                    case LEFT:
                    draw_sprite_h_flip(raw, temp, inter->x-joueur->xcamera, inter->y-joueur->ycamera);
                break;
                }

                temp = draw_status((double)inter->hp_max, (double)inter->hp, UNIT);
                draw_sprite(raw, temp, inter->x - joueur->xcamera, inter->y-10-joueur->ycamera);
                destroy_bitmap(temp);
            }
        }
        maill = maill->next;
    }

    DEB("3-4")

    maill = joueur->selection.debut;
    //affichage différent pour les unités selectionnées
    while (maill!=NULL)
    {
        inter = maill->unite;
        if (inter->state!=DEAD)
        {
            if ((inter->x/COTE)>=xmin && (inter->y/COTE)>=ymin && (inter->x/COTE)<=(xmin+xtaille) && (inter->y/COTE)<=(ymin+ytaille))
            {
                rect(raw, inter->x - joueur->xcamera, inter->y-joueur->ycamera, inter->x+inter->cote-joueur->xcamera, inter->y+inter->cote-joueur->ycamera, NOIR);
            }

        }

        maill = maill->next;
    }
    DEB("3-5")

    //affichage du brouillard de guerre (partie invisible non explorée)
    if (!TEST)
    {
        if (!LAG)
        {
            fog = create_bitmap(joueur->xecran*COTE, joueur->yecran*COTE);

            maill = ancre.debut;

            rectfill(fog, 0,0,COTE*joueur->xecran, COTE*joueur->yecran, NOIR);
            while(maill!=NULL)
            {
                inter = maill->unite;
                if (inter->side==ALLY)
                {
                    if ((inter->x/COTE)>=xmin-(sprites->fog->w)/2 && (inter->y/COTE)>=ymin -(sprites->fog->w)/2 && (inter->x/COTE)<=(xmin+xtaille)+(sprites->fog->w)/2 && (inter->y/COTE)<=(ymin+ytaille)+(sprites->fog->w)/2)
                    {
                        xfog = inter->x-joueur->xcamera;
                        yfog = inter->y-joueur->ycamera;

                        //blit(sprites->fog, fog, 0,0, inter->x-joueur->xcamera-(sprites->fog->w)/2, inter->y-joueur->ycamera-(sprites->fog->w)/2, 600, 600);

                        for (i=0; i<600; i++)
                        {
                            for(j=0; j<600;j++)
                            {
                                getim=getpixel(sprites->fog,j,i);
                                getdest=getpixel(raw,xfog-(sprites->fog->w)/2+j, yfog-(sprites->fog->w)/2+i);
                                getbuf=getpixel(fog,xfog-(sprites->fog->w)/2+j, yfog-(sprites->fog->w)/2+i);


                                if ((getim!=NOIR) && getim!=MAG && getbuf!=MAG )
                                {
                                    rim=getr(getim);
                                    gim=getg(getim);
                                    bim=getb(getim);

                                    rdest=getr(getdest);
                                    gdest=getg(getdest);
                                    bdest=getb(getdest);

                                    putpixel(fog,xfog-(sprites->fog->w)/2+j, yfog-(sprites->fog->w)/2+i,makecol((CLRBUF-CLRF)*rim+CLRF*rdest,(CLRBUF-CLRF)*gim+CLRF*gdest,(CLRBUF-CLRF)*bim+CLRF*bdest));

                                    ///if (getbuf!=makecol(0,0,0)&&(getbuf!=makecol(255,0,0)))
                                    ///{
                                    ///    putpixel(fog, inter->x-joueur->xcamera-(sprites->fog->w)/2+j, inter->y-joueur->ycamera-(sprites->fog->w)/2+i,makecol(255,0,255));
                                    ///}
                                }
                                if (getim==MAG)
                                {
                                    putpixel(fog, xfog-(sprites->fog->w)/2+j, yfog-(sprites->fog->w)/2+i,getim);
                                }

                            }
                        }
                    }
                }

                maill = maill->next;
            }


//            for (i=0; i<joueur->yecran*COTE; i++)
//            {
//                for(j=0; j<joueur->xecran*COTE;j++)
//                {
//
//                    getdest=getpixel(raw,j, i);
//                    getbuf=getpixel(fog,j, i);
//                    if(getbuf==makecol(75,75,75))
//                    {
//
//                        rbuf=getr(getbuf);
//                        gbuf=getg(getbuf);
//                        bbuf=getb(getbuf);
//                        rdest=getr(getdest);
//                        gdest=getg(getdest);
//                        bdest=getb(getdest);
//                        putpixel(fog,j,i,makecol((CLRBUF-CLRF)*rbuf+CLRF*rdest,(CLRBUF-CLRF)*gbuf+CLRF*gdest,(CLRBUF-CLRF)*bbuf+CLRF*bdest));
//                    }
//                }
//            }

            masked_blit(fog, raw, 0,0,0,0, joueur->xecran*COTE, joueur->yecran*COTE);
            destroy_bitmap(fog);
        }
        else
        {
            for (i=0;i<xtaille;i++)
            {
                for (j=0;j<ytaille;j++)
                {
                    if (!carte[i+xmin][j+ymin].visible) //si on ne peut pas voir la tuile
                    {
                        blit(sprites->brouillard, raw, 0, 0, xo + i*COTE, yo + j*COTE, COTE, COTE);
                    }
                }
            }
        }
    }
    DEB("3-6")
    if (joueur->act==SELECTING)
    {
        rect(raw, joueur->xprev-joueur->xcamera, joueur->yprev-joueur->ycamera, mouse_x*((float)joueur->xecran/ECRANX), mouse_y*((float)joueur->yecran/ECRANY), VERT);
    }
    DEB("3-7")

    /// À TRANSFORMER EN STRETCH BLIT
    stretch_blit(raw, dest, 0, 0, joueur->xecran*COTE, joueur->yecran*COTE,0, 0, ECRANX*COTE, ECRANY*COTE);
    destroy_bitmap(raw);

    DEB("3-8")
    temp = create_bitmap(ECRANX*COTE, UI_HEIGHT);
    draw_ui(temp, ancre, carte, sprites, *joueur);
    blit(temp, dest, 0, 0, 0, ECRANY*COTE, ECRANX*COTE, UI_HEIGHT);
    destroy_bitmap(temp);
    DEB("3-9")

    temp = game_info(*joueur, sprites);
    blit(temp, dest, 0, 0, ECRANX*COTE - ISIZEX, 0, ISIZEX, ISIZEY);
    destroy_bitmap(temp);

    DEB("3-10")

    //le rectangle en bas sur l'ecran pour pouvoir "scroll"
    masked_blit(sprites->ign_d, dest, 0, 0, 0, YSCREEN - MOVELIMIT, XSCREEN, MOVELIMIT);

    //on dessine la souris
    draw_sprite(dest, sprites->souris, mouse_x, mouse_y);
}


//fin du fichier
