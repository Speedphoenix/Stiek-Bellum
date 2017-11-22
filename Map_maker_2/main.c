#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>

#define SOURCE "map_leo.bmp"
#define DEST1 "sauv1.map"
#define DEST2 "sauv1.trc"
#define MAPNUM 1

///das chaque cas la position désigne la position DU MOT (grass_l_d veut dire que c'est que du sable avec un peu d'herbe en bas en bas à gauche)
#define grass       makecol(0,255,0)
#define grass_l     makecol(0,247,0)
#define grass_u     makecol(0,239,0)
#define grass_r     makecol(0,231,0)
#define grass_d     makecol(0,223,0)
#define grass_l_u   makecol(0,215,0)
#define grass_l_d   makecol(0,207,0)
#define grass_r_u   makecol(0,199,0)
#define grass_r_d   makecol(0,191,0)
#define sand        makecol(255,200,0)
#define sandPg_l_u  makecol(247,192,0)
#define sandPg_l_d  makecol(239,184,0)
#define sandPg_r_u  makecol(231,176,0)
#define sandPg_r_d  makecol(223,168,0)
#define bridge      makecol(255,0,0)
#define bridge_l    makecol(231,0,0)
#define bridge_u    makecol(207,0,0)
#define bridge_r    makecol(183,0,0)
#define bridge_d    makecol(159,0,0)

#define water       makecol(0,0,255)
#define water_l     makecol(0,0,247)
#define water_u     makecol(0,0,239)
#define water_r     makecol(0,0,231)
#define water_d     makecol(0,0,223)
#define water_l_u   makecol(0,0,215)
#define water_l_d   makecol(0,0,207)
#define water_r_u   makecol(0,0,199)
#define water_r_d   makecol(0,0,191)
#define sandPw_l_u  makecol(215,160,0)
#define sandPw_l_d  makecol(207,152,0)
#define sandPw_r_u  makecol(199,144,0)
#define sandPw_r_d  makecol(191,136,0)

#define treePf_down  makecol(0,128,0)
#define treePf_up    makecol(0,120,0)
#define treePf_small makecol(0,112,0)
#define treePp_down  makecol(0,104,0)
#define treePp_up    makecol(0,96,0)

#define rock         makecol(192,192,192)

#define mairie_l_u  makecol(125,95,0) ///pour les batiments la position designe quel coin du batiment c'est
#define mairie_l_d  makecol(117,87,0)
#define mairie_r_u  makecol(109,79,0)
#define mairie_r_d  makecol(101,71,0)
#define camp_l_u    makecol(77,47,0)
#define camp_l_d    makecol(69,39,0)
#define camp_r_u    makecol(61,31,0)
#define camp_r_d    makecol(53,23,0)

#define mountain    makecol(0,0,0)
#define mountain_l  makecol(16,16,16)
#define mountain_u  makecol(24,24,24)
#define mountain_r  makecol(32,32,32)
#define mountain_d  makecol(40,40,40)
#define mountain_l_u    makecol(48,48,48)
#define mountain_l_d    makecol(56,56,56)
#define mountain_r_u    makecol(64,64,64)
#define mountain_r_d    makecol(72,72,72)
#define sandPm_l_u  makecol(183,128,0)
#define sandPm_l_d  makecol(175,120,0)
#define sandPm_r_u  makecol(167,112,0)
#define sandPm_r_d  makecol(159,105,0)


#define MAPSIZEX 300
#define MAPSIZEY 225

///Une tuile
typedef struct Tile{
    ///le type de tuile
    char type;
    ///Le "type" de ce type de tuile (partie en haut à gauche de l'arbre?)
    int position;
///une tuile
}Tile;

void save_builds(Tile carte[MAPSIZEX][MAPSIZEY]);


int main()
{
    BITMAP *source;
    FILE *dest;
    int col;
    Tile carte[MAPSIZEX][MAPSIZEY];
    char a = ',', b = 'A';

    allegro_init();
    set_color_depth(desktop_color_depth());

    dest = fopen(DEST1, "w");
    source = load_bitmap(SOURCE, NULL);

    if (source==NULL)
        exit(1);

    for (int j=0;j<MAPSIZEY;j++)
    {
        for (int i=0;i<MAPSIZEX;i++)
        {
            col = getpixel(source, i, j);

            //printf("%d\n", col);

            if (col==grass || col==grass_l || col==grass_u || col==grass_r || col==grass_d || col==grass_l_u || col==grass_l_d ||
                col==grass_r_u || col==grass_r_d || col==sand || col==sandPg_l_u || col==sandPg_l_d || col==sandPg_r_u || col==sandPg_r_d ||
                col==bridge || col==bridge_d || col==bridge_l || col==bridge_r || col==bridge_u)
                a = ',';
            else if (col==water || col==water_l || col==water_u || col==water_r || col==water_d || col==water_l_u || col==water_l_d ||
                col==water_r_u || col==water_r_d || col==sandPw_l_u || col==sandPw_l_d || col==sandPw_r_u || col==sandPw_r_d)
                a = '_';
            else if (col==treePf_down || col==treePf_up || col==treePf_small || col==treePp_down || col==treePp_up)
                a = '?';
            else if (col==rock)
                a = '@';
            else if (col==mairie_l_u || col==mairie_l_d || col==mairie_r_u || col==mairie_r_d)
                a = '#';
            else if (col==camp_l_u || col==camp_l_d || col==camp_r_u || col==camp_r_d)
                a = '/';
            else if (col==mountain || col==mountain_l || col==mountain_u || col==mountain_r || col==mountain_d || col==mountain_l_u || col==mountain_l_d ||
                col==mountain_r_u || col==mountain_r_d || col==sandPm_l_u || col==sandPm_l_d || col==sandPm_r_u || col==sandPm_r_d)
                a = '%';
            else
                a = ',';


            if      (col==grass ||      col==water ||       col==mountain ||    col==treePf_down || col==mairie_l_u || col==camp_l_u || col==rock)
                b = 'A';
            else if (col==sand ||       col==water_r_d ||   col==mountain_d ||  col==treePf_up ||   col==mairie_r_u || col==camp_r_u)
                b = 'B';
            else if (col==bridge ||     col==water_d ||     col==mountain_l ||  col==treePp_down || col==mairie_l_d || col==camp_l_d)
                b = 'C';
            else if (col==grass_r_d ||  col==water_l_d ||   col==sandPm_r_u ||  col==treePp_up ||   col==mairie_r_d || col==camp_r_d)
                b = 'D';
            else if (col==grass_d ||    col==water_l ||     col==sandPm_r_d ||  col==treePf_small)
                b = 'E';
            else if (col==grass_l_d ||  col==water_l_u ||   col==mountain_r)
                b = 'F';
            else if (col==grass_l ||    col==water_u ||     col==sandPm_l_u)
                b = 'G';
            else if(col==grass_l_u ||   col==water_r_u ||   col==sandPm_l_d)
                b = 'H';
            else if (col==grass_u ||    col==water_r ||     col==mountain_u)
                b = 'I';
            else if (col==grass_r_u ||  col==sandPw_r_d ||  col==mountain_l_d)
                b = 'J';
            else if (col==grass_r ||    col==sandPw_l_d ||  col==mountain_l_u)
                b = 'K';
            else if (col==sandPg_r_d || col==sandPw_l_u ||  col==mountain_r_d)
                b = 'L';
            else if (col==sandPg_l_d || col==sandPw_r_u ||  col==mountain_r_u)
                b = 'M';
            else if (col==sandPg_l_u)
                b = 'N';
            else if (col==sandPg_r_u)
                b = 'O';
            else if (col==bridge_d)
                b = 'P';
            else if (col==bridge_l)
                b = 'Q';
            else if (col==bridge_u)
                b = 'R';
            else if (col==bridge_r)
                b = 'S';
            else
                b = 'A';

            carte[i][j].type = a;
            carte[i][j].position = b - 'A';
            fprintf(dest, "%c%c", a, b);
        }
        fprintf(dest, "\n");
    }
    fprintf(dest, "\n");

    fclose(dest);

    save_builds(carte);

    return 0;
}
END_OF_MAIN();


void save_builds(Tile carte[MAPSIZEX][MAPSIZEY])
{
    FILE *fic;
    int a=0;

    fic = fopen(DEST2, "w");

    for (int i=0;i<MAPSIZEX;i++)
    {
        for (int j=0;j<MAPSIZEY;j++)
        {
            if ((carte[i][j].type=='#' || carte[i][j].type=='/') && carte[i][j].position==0)
            {
                a++;
            }
        }
    }

    fprintf(fic, "%d\n", a);

    for (int i=0;i<MAPSIZEX;i++)
    {
        for (int j=0;j<MAPSIZEY;j++)
        {
            if ((carte[i][j].type=='#' || carte[i][j].type=='/') && carte[i][j].position==0)
            {
                fprintf(fic, "%d %d 2 2\n", i, j);
                fprintf(fic, "0 %d %d %d %d %d\n", (carte[i][j].type=='/')?0:1, 1000, 1000, 150, 40);
                fprintf(fic, "%d 0\n", (carte[i][j].type=='/')?3:0);
                fprintf(fic, "\n");

                fprintf(fic, "0\n\n");
            }
        }
    }


    fprintf(fic, "\n\n\n0\n\n"); //la taille de l'ancre pour les unités, c'est à dire, 0

    fprintf(fic, "\n\n\n");

    //les valeurs "globales" (on ecrit la structure joueur)
    fprintf(fic, "0 0 20 10\n");
    fprintf(fic, "160 80 0 1\n0 %d\n", MAPNUM);
    fprintf(fic, "0\n");

    fprintf(fic, "\n");

    fclose(fic);
}

