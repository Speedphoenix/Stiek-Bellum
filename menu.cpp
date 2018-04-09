#include "menu.h"

/*
    ce fichier contient les fonctions de menu
*/

using namespace std;

//les menus
int menu(Sprites& sprites, Joueur& joueur, int type)
{
    int i;
    //buffer est le buffer, fond est le fond, avant est ce qui contient les boutons mais ne change pas, dessus est ce qui change (les hovers)
    BITMAP *buffer, *fond, *avant, *dessus;
    int rep = -1, choix;
    int boutonx, xboard, yboard, ecart, w, h; //w et h pour le board
    int xflag, yflag;

    buffer = create_bitmap(XSCREEN, YSCREEN);
    fond = create_bitmap(XSCREEN, YSCREEN);

    switch (type)
    {
        case LOAD_MENU:
        type--; //les boutons sont les même que pour SAVE_MENU
        default:
        case MAIN_MENU:
        case MAP_MENU:
        case DIFF_MENU:
        blit(sprites.fond[0], fond, 0, 0, 0, 0, XSCREEN, YSCREEN);

        ecart = ECART_M;
        w = BOUTON_W + 2*ECART_H + HOVER_W;
        h = 4*ecart;
        yboard = 250-ecart;

        xflag = 1200;
        yflag = YSCREEN - 60;

    break;

        case SAVE_MENU:
        case PAUSE_MENU:
        blit(screen, fond, 0, 0, 0, 0, XSCREEN, YSCREEN);

        w = PAUSE_W;
        h = PAUSE_H;
        ecart = ECART_P;
        yboard = PAUSE_Y;

        blit(sprites.fond[1], fond, 0, 0, XSCREEN/2 - w/2, yboard, w, h);

        xflag = XSCREEN/2 + w/2 - 3*60 - 100;
        yflag = yboard + h - 60 - 25;

    break;
    }

    avant = create_bitmap(w, h);
    rectfill(avant, 0, 0, w, h, MAG);

    xboard = XSCREEN/2 - w/2;
    boutonx = w/2 - BOUTON_W/2;

    dessus = create_bitmap(w, h);



    for (i=0;i<4;i++)
    {
        draw_sprite(avant, sprites.buttons[type][joueur.langue][i], boutonx, i*ecart);
    }

    while (rep==-1)
    {
        blit(fond, buffer, 0, 0, 0, 0, XSCREEN, YSCREEN);

        masked_blit(avant, buffer, 0, 0, xboard, yboard, w, h);

        rectfill(dessus, 0, 0, w, h, MAG);

        for (i=0;i<3;i++)
        {
            draw_sprite_h_flip(dessus, sprites.hover_unit[1][i], boutonx - ECART_H - HOVER_W/2,     (i+1)*ecart + BOUTON_H/2 - HOVER_W/2);
            draw_sprite(dessus, sprites.hover_unit[1][i], boutonx + BOUTON_W + ECART_H - HOVER_W/2, (i+1)*ecart + BOUTON_H/2 - HOVER_W/2);
        }

        if ((mouse_x-xboard)>=boutonx && (mouse_x-xboard)<(boutonx+BOUTON_W) && //en abscisse si la souris est sur la bande des boutons
            (mouse_y>yboard && mouse_y<(yboard+(4*ecart))) &&                   //en ordonnée si la souris est sur la zone où y'a des boutons
            ((mouse_y - yboard)%ecart)<BOUTON_H)                                //en ordonnée si la souris est sur un bouton individuel
        {
            choix = (int)((mouse_y - yboard)/(ecart));


            if (choix>0)
            {
                //on efface le précédent
                rectfill(dessus, boutonx - ECART_H - HOVER_W/2,            choix*ecart, boutonx - ECART_H + HOVER_W/2,            choix*ecart + BOUTON_H, MAG);
                rectfill(dessus, boutonx + BOUTON_W + ECART_H - HOVER_W/2, choix*ecart, boutonx + BOUTON_W + ECART_H + HOVER_W/2, choix*ecart + BOUTON_H, MAG);

                draw_sprite_h_flip(dessus, sprites.hover_unit[0][choix-1], boutonx - ECART_H - HOVER_W/2,     choix*ecart);
                draw_sprite(dessus, sprites.hover_unit[0][choix-1], boutonx + BOUTON_W + ECART_H - HOVER_W/2, choix*ecart);
            }

            if (!(mouse_b & 1) && joueur.clic_prec==1)
            {
                if (choix>0 || type==MAP_MENU || type==DIFF_MENU || type==SAVE_MENU)
                    rep = choix;
            }
        }
        else if (mouse_x>xflag && mouse_x<(xflag+3*60) && mouse_y>yflag && mouse_y<(yflag+60))
        {
            if (!(mouse_b & 1) && joueur.clic_prec==1)
            {
                joueur.langue = (int)((mouse_x-xflag)/60);

                rectfill(avant, 0, 0, w, h, MAG);
                for (i=0;i<4;i++)
                {
                    draw_sprite(avant, sprites.buttons[type][joueur.langue][i], boutonx, i*ecart);
                }
            }

        }

        if (mouse_b & 8) //le bouton lateral de la souris (pour revenir en arrière)
        {
            if ((type==MAP_MENU || type==DIFF_MENU || type==SAVE_MENU) && joueur.clic_prec!=8)
                rep = 0;

            joueur.clic_prec = 8;
        }
        else if (mouse_b & 1)
            joueur.clic_prec = 1;
        else if (mouse_b & 2)
            joueur.clic_prec = 2;
        else
            joueur.clic_prec = 0;

        masked_blit(dessus, buffer, 0, 0, xboard, yboard, w, h);
        draw_sprite(buffer, sprites.flags, xflag, yflag);
        draw_sprite(buffer, sprites.souris, mouse_x, mouse_y);
        blit(buffer, screen, 0, 0, 0, 0, XSCREEN, YSCREEN);

        rest(20);
    }

    destroy_bitmap(fond);
    destroy_bitmap(avant);
    destroy_bitmap(buffer);
    destroy_bitmap(dessus);

    return rep;
}

//met le jeu en ause, reprend quand le joueur l'a décidé
void pause_game(Joueur& joueur, Sprites& sprites, Tile carte[MAPSIZEX][MAPSIZEY], Ancre& ancre, list<Build *>& ancre_b)
{
    int choix = 1;

    while (joueur.pause)
    {
        choix = menu(sprites, joueur, PAUSE_MENU);

        switch (choix)
        {
            case RESUME:

            joueur.pause = 0;
        break;

            case SAVE:
            do
            {
                choix = menu(sprites, joueur, SAVE_MENU);

                if (choix)
                {
                    choix+=3;
                    save_game(carte, ancre, ancre_b, joueur, choix);
                }
            }while (choix);

        break;

            case EXIT:
            joueur.quit = 1;
            joueur.pause = 0;
        break;

            default:
        break;
        }
    }
}

//à appeller à la fin d'une partie pour les meilleurs scores
void end_game(Sprites& sprites, Joueur& joueur)
{
    int i, j, a, xbouton;
    BITMAP *buffer, *fond, *avant;
    FILE *fic;
    char nom[20];
    int quant, nouveau, tab[11];
    TIMESTRUCT now;

    sprintf(nom, "%s%d-%d.txt", SAUV, joueur.map_num, joueur.level);

    DEB("7-0")

    fic = fopen(nom, "r");
    if (fic==NULL)
    {
        DEB("7-1")
        quant = 0;
    }
    else
    {
        DEB("7-2")
        fscanf(fic, "%d\n", &quant);

        if (quant>10)
            quant = 10;

        for (i=0;i<quant;i++)
        {
            fscanf(fic, "%d\n", tab+i);
        }
        fclose(fic);
    }
    DEB("7-3")
    getTime(now);

    nouveau = getSecInt(joueur.debut, now);

    j = 0;
    a = 1;
    for (i=0;i<quant;i++)
    {
        j = i;
        a = 0;
        if (tab[i]>nouveau)
        {
            a = 1;
            break;
        }
    }
    DEB("7-4")
    if (a)
    {
        DEB("7-5")
        for (i=quant;i>j;i--)
        {
            tab[i] = tab[i-1];
        }
    }
    else
    {
        j = quant;
    }
    quant++;
    tab[j] = nouveau;

    DEB("7-6")

    buffer = create_bitmap(XSCREEN, YSCREEN);
    fond = create_bitmap(XSCREEN, YSCREEN);
    blit(screen, fond, 0, 0, 0, 0, XSCREEN, YSCREEN);
    blit(sprites.fond[1], fond, 0, 0, XSCREEN/2 - PAUSE_W/2, PAUSE_Y, PAUSE_W, PAUSE_H);
    avant = create_bitmap(PAUSE_W, PAUSE_H);
    rectfill(avant, 0, 0, PAUSE_W, PAUSE_H, MAG);

    xbouton = XSCREEN/2 - BOUTON_W/2;
    draw_sprite(avant, sprites.v_buttons[joueur.langue][0], PAUSE_W/2 - BOUTON_W/2, 0);

    DEB("7-7")
    switch (joueur.langue)
    {
        default:
        case ENGLISH:
        textprintf_ex(avant, font, 40, ECART_P+18 , COL_UI_ACC, -1, "HIGH SCORES");
    break;

        case FRANCAIS:
        textprintf_ex(avant, font, 40, ECART_P+18 , COL_UI_ACC, -1, "MEILLEURS SCORES");
    break;

        case ITALIANO:
        textprintf_ex(avant, font, 40, ECART_P+18 , COL_UI_ACC, -1, "HIGH SCORES");
    break;
    }

    for (i=0;i<quant;i++)
    {
        textprintf_ex(avant, font, 40, ECART_P+40+i*20, COL_UI_ACC, -1, " N°%d%s:  %d:%s%d", i+1, i<9?" ":"", (int)(tab[i]/60), (tab[i]%60)<10?"0":"", tab[i]%60);
    }
    textprintf_ex(avant, font, 170, ECART_P + 40 + j*20, COL_UI_ACC, -1, "%s", "YOU");
    DEB("7-8")
    draw_sprite(avant, sprites.v_buttons[joueur.langue][1], PAUSE_W/2 - BOUTON_W/2, PAUSE_H - ECART_P);

    a = 0;
    while (!a)
    {
        blit(fond, buffer, 0, 0, 0, 0, XSCREEN, YSCREEN);
        masked_blit(avant, buffer, 0, 0, XSCREEN/2 - PAUSE_W/2, PAUSE_Y, PAUSE_W, PAUSE_H);
        draw_sprite(buffer, sprites.souris, mouse_x, mouse_y);
        blit(buffer, screen, 0, 0, 0, 0, XSCREEN, YSCREEN);
        DEB("7-9")
        if (mouse_x>xbouton && mouse_x<(xbouton + BOUTON_W) && (mouse_y-PAUSE_Y)>(PAUSE_H-ECART_P) && (mouse_y-PAUSE_Y)<(PAUSE_H-(ECART_P-BOUTON_H)))
        {
            DEB("7-10")
            if (joueur.clic_prec==1 && !(mouse_b & 1))
            {
                DEB("7-11")
                a = 1;
            }
        }

        if (mouse_b & 1)
            joueur.clic_prec = 1;
        else if (mouse_b & 2)
            joueur.clic_prec = 2;
        else
            joueur.clic_prec = 0;

        rest(20);
    }
    destroy_bitmap(avant);
    destroy_bitmap(buffer);
    destroy_bitmap(fond);

    DEB("7-12")
    if (quant>10)
    {
        DEB("7-13")
        quant = 10;
    }

    fic = fopen(nom, "w");

    DEB("7-14")

    fprintf(fic, "%d\n", quant);

    DEB("7-15")

    for (i=0;i<quant;i++)
    {
        DEB("7-16")
        fprintf(fic, "%d\n", tab[i]);
    }
    DEB("7-17")
    fclose(fic);
    DEB("7-18")
}

//initialisation de allegro
void init_alleg(int sizex, int sizey)
{
    allegro_init();

    install_keyboard();
    install_mouse();

    install_timer();
    srand(time(NULL));

    set_color_depth(desktop_color_depth());
    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, sizex, sizey, 0, 0))
    {
        allegro_message("erreur gfx");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}



//fin du fichier
