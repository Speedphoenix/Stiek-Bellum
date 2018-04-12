#include "load.h"

/*
    Ce fichier contient les sous programmes appellées uniquement au début du jeu
    merci à Mr. Segado pour la fonction init alleg
*/

/** trucs qui ne sont pas enregistrés dans save_game:
    la quantité de ressources sur une case (soit y'en a pas soit reprend tout)
    les temps (de formation des unités, les animation, attaques, tout est remis à zero)
    La vision sur les cases
 */

using namespace std;

//ce sous programme sert à définir les différents parametres d'une tuile
void set_tile(Tile& tile, char type)
{
    int a;

    tile.position = type - 'A'; //pour passer du caractère à un entier
    tile.erige = NULL;         //par defaud pas de batiment dessus
    tile.fleur = 0;

    switch (tile.type)
    {
        case ROCK:
        case TREE:
        tile.block = SOLID;
        switch (tile.position)
        {
            case 0: //si c'est la partie basse d'un arbre feuillu ou conifere
            case 2:
            tile.res = GRAND; //set les ressources
        break;
            case 4: //si c'est un petit arbre
            tile.res = PETIT;
        break;
            default:
        break;
        }
    break;

        case BUILDING:
        tile.block = SOLID;
        tile.res = 0;
    break;

        case CAMP:
        tile.block = SOLID;
        tile.res = 0;
    break;

        case WATER:
        tile.block = NAGE; //peut pas passer
        tile.res = 0;

        if (tile.position==0)
        {
            //rien pour l'instant, on ajoutera les nenuphars après
        }
    break;

        case MOUNTAIN:
        tile.block = SOLID;
        tile.res = 0;
    break;

        default:
        case GRASS:
        tile.block = VIDE; //il n'y a rien
        tile.res = 0;

        if (tile.position==P_HERBE)
        {
            a = rand()%20;
            if (a<7)
                tile.fleur = a;
            else if (a==8)
                tile.fleur = 3;
        }
        else if (tile.position==P_SABLE) //on met les caillous
        {
            a = rand()%44;
            if (a<3)
                tile.fleur = a+7;
            else if (a==4)
                tile.fleur = 7;
        }
    break;
    }
    tile.visible = 0; //au debut tous les blocks sont invisibles
}

//sauvegarde toute la partie dans un fichier de sauvegarde
void save_game(Tile carte[MAPSIZEX][MAPSIZEY], list<Unit *>& ancre, list<Build *>& ancre_b, Joueur& joueur, int num)
{
    int i, j;
    TIMESTRUCT now;
    Unit *unite;
    Build *bat;
    FILE *fic;
    char filename[50];
    int val, counter;
    ofstream outStream;

    getTime(now);

    sprintf(filename, NAME, SAUV, num, MAP); //on assemble le nom du fichier

    fic = fopen(filename, "w");
    //sauvegarde de la map (ne sauvegarde pas la quantité de ressources. ni la visibilité :'(..)
    for (j=0;j<MAPSIZEY;j++)
    {
        for (i=0;i<MAPSIZEX;i++)
        {
            fprintf(fic, "%c%c", carte[i][j].type, carte[i][j].position+'A');
        }
        fprintf(fic, "\n");
    }
    fprintf(fic, "\n");
    fclose(fic);


    sprintf(filename, NAME, SAUV, num, FOG);

    fic = fopen(filename, "w");
    //sauvegarde du brouillard de guerre
    for (j=0;j<MAPSIZEY;j++)
    {
        counter = 0;
        val = 0;
        for (i=0;i<MAPSIZEX;i++)
        {
            if (val==carte[i][j].visible)
            {
                counter++;
            }
            else
            {
                fprintf(fic, "%d %d ", val, counter);
                val = carte[i][j].visible;
                counter = 1;
            }
        }
        fprintf(fic, "%d %d \n", val, counter);
    }
    fprintf(fic, "\n");

    fclose(fic);


    sprintf(filename, NAME, SAUV, num, VALS); //on assemble le nom du deuxième fichier

    outStream.open(filename, ios::out | ios::trunc); ERR_CHARG(outStream)
    //fic = fopen(filename, "w");
    //sauvegarde des autres valeurs

    outStream << endl << endl << endl << ancre_b.size() << endl << endl;

    for (auto& elem : ancre_b)
    {
        //on sauvegarde les batiments
        elem->sendStream(outStream, 1);
    }

    outStream << endl << endl << endl << ancre.size() << endl << endl;

    for (auto& elem : ancre) //on sauvegarde toutes les unités
    {
        elem->sendStream(outStream, 1);
    }

    outStream << endl << endl << endl;

    //les valeurs "globales" (on ecrit la structure joueur)
    outStream << joueur.xcamera << " " << joueur.ycamera << " " << joueur.xecran << " " << joueur.yecran << endl;
    outStream << joueur.bois << " " << joueur.marbre << " " << joueur.viande << " " << joueur.nend_b << endl;
    outStream << joueur.level << " " << joueur.map_num << endl;

    //on ne met pas prevwheel
    //on ne met pas non plus les valeurs à petite durée d'utilisation
    outStream << getSecInt(joueur.debut, now) << endl;

    outStream << endl;

    outStream.close();
}

//chargement de toute la map et des bitmaps
void load_game(Tile carte[MAPSIZEX][MAPSIZEY], list<Unit *>& ancre, list<Build *>& ancre_b, Joueur& joueur, int num)
{
    int i, j, k;
    FILE *fic;
    char a, filename[50];
    int siz, x, y;
    Unit *unite;
    Build *bat;
    ifstream inStream;

    //on fait le nom du fichier
    sprintf(filename, NAME, SAUV, num, MAP);

    //chargement de la map depuis un fichier
    fic = fopen(filename, "r"); ERR_CHARG(fic)

    for (j=0;j<MAPSIZEY;j++)
    {
        for (i=0;i<MAPSIZEX;i++)
        {
            fscanf(fic, "%c%c", &carte[i][j].type, &a); //chaque tuile
            set_tile(carte[i][j], a);
        }
        fscanf(fic, "\n");
    }
    fclose(fic);
    //fin du chargement de la map


    sprintf(filename, NAME, SAUV, num, FOG);

    fic = fopen(filename, "r");
    //chargement du brouillard de guerre s'il y en a un
    if (fic!=NULL)
    {
        for (j=0;j<MAPSIZEY;j++)
        {
            i = 0;
            while (i<MAPSIZEX)
            {
                fscanf(fic, "%d %d ", &x, &y);
                for (k=0;k<y;k++)
                {
                    carte[i][j].visible = x;
                    i++;
                }
            }
            fscanf(fic, "\n");
        }

        fclose(fic);
    }


    //assemblage de l'autre nom
    sprintf(filename, NAME, SAUV, num, VALS);

    //chargement des autres valeurs
    inStream.open(filename, ios::in); ERR_CHARG(inStream)

    //fic = fopen(filename, "r");ERR_CHARG(fic)

    inStream >> siz;

    for (i=0;i<siz;i++) //chaque batiment
    {
        bat = new Build();

        bat->receiveStream(inStream, carte, 1);

        ancre_b.push_back(bat); //on l'ajoute à la liste chainée
    }

    inStream >> siz;

    for (i=0;i<siz;i++) //pour chaque unite
    {
        unite = new Unit();

        unite->receiveStream(inStream, carte, 1);

        ancre.push_back(unite);
    }

    //les valeurs "globales" - -on remplit la structure joueur

    inStream >> joueur.xcamera >> joueur.ycamera >> joueur.xecran >> joueur.yecran;
    inStream >> joueur.bois >> joueur.marbre >> joueur.viande >> joueur.nend_b;
    inStream >> joueur.level >> joueur.map_num;


    joueur.quit = 0;
    joueur.change = 1;
    joueur.chang_taill = 1;

    joueur.prevwheel = mouse_z;
    joueur.act = RIEN;
    joueur.clic_prec = 0;

    getTime(joueur.last_spawn);
    getTime(joueur.last_clic);

    inStream >> siz;

    getTime(joueur.debut);
    addSec(joueur.debut, (-1 * siz));

    joueur.pause = 0;

    inStream.close();
}

//remet tout à zero
void reset(list<Unit *>& ancre, list<Build *>& ancre_b, Joueur& joueur, Tile carte[MAPSIZEX][MAPSIZEY])
{
    int i, j;
    for (i=0;i<MAPSIZEX;i++)
    {
        for (j=0;j<MAPSIZEY;j++) //chaque tuile de la map
        {
            carte[i][j].type = GRASS;
            carte[i][j].position = P_HERBE;
            carte[i][j].block = 0;
            carte[i][j].res = 0;
            carte[i][j].visible = 0;
            carte[i][j].erige = NULL;
        }
    }

    //les valeurs "globales"
    joueur.xcamera      = 0;
    joueur.ycamera      = 0;
    joueur.xecran       = ECRANX;
    joueur.yecran       = ECRANY;
    joueur.prevwheel    = mouse_z;
    joueur.bois         = 0;
    joueur.marbre       = 0;
    joueur.viande       = 0;
    joueur.nend_b       = 1;
    joueur.nend_e       = 0;
    joueur.quit         = 0;
    joueur.pause        = 0;
    joueur.xprev        = 0;
    joueur.yprev        = 0;
    joueur.act          = RIEN;
    joueur.clic_prec    = 0;
    joueur.level        = EASY;
    joueur.change       = 1;
    joueur.chang_taill  = 1;


    getTime(joueur.debut);
    getTime(joueur.last_clic);
    getTime(joueur.last_spawn);

    joueur.selection.clear();

    for (auto& elem : ancre)
        delete elem;
    ancre.clear();

    for (auto& elem : ancre_b)
        delete elem;
    ancre_b.clear();
}

//pour la création des petits menus dans l'UI
BITMAP *create_menu(Sprites& sprites, int type)
{
    int i;
    BITMAP *rep, *inter;

    rep = create_bitmap(XSCREEN-SEPARE, UI_HEIGHT); //la bonne taille
    blit(sprites.r_bar, rep, 0, 0, 0, 0, XSCREEN-SEPARE, UI_HEIGHT);

    inter = create_bitmap(BOX_W, BOX_H); //les boites individuelles pour chaque truc possible à "acheter"
    rectfill(inter, 0, 0, BOX_W, BOX_H, MAG);

    draw_sprite(inter, sprites.bois_i, MARGE, MARGE);       //les icones des ressources
    draw_sprite(inter, sprites.pierre_i, MARGE + RES_W, MARGE);
    draw_sprite(inter, sprites.viande_i, MARGE + 2*RES_W, MARGE);


    switch (type)
    {
        case MAIRIE: //y'a qu'une option
        masked_blit(inter, rep, 0, 0, 0, 0, BOX_W, BOX_H);

        masked_blit(sprites.paysant_i, rep, 0, 0, 0, BOX_H-ICON_S, ICON_S, ICON_S); //on met l'icone au bon endroit

        textprintf_ex(rep, font, RES_S + (2*MARGE), YTEXT, COL_UI_ACC, -1, "%d", WOOD_PEAS);    //on met les quantités de ressources necessaires
        textprintf_ex(rep, font, RES_W + RES_S + (2*MARGE), YTEXT, COL_UI_ACC, -1, "%d", ROCK_PEAS);
        textprintf_ex(rep, font, 2*RES_W + RES_S + (2*MARGE), YTEXT, COL_UI_ACC, -1, "%d", 0);

    break;

        case CASERNE: //y'a qu'une optionn (pour l'instant)
        masked_blit(inter, rep, 0, 0, 0, 0, BOX_W, BOX_H);

        masked_blit(sprites.epee_i, rep, 0, 0, 0, BOX_H-ICON_S, ICON_S, ICON_S); //on met l'icone au bon endroit

        textprintf_ex(rep, font, RES_S + (2*MARGE), YTEXT, COL_UI_ACC, -1, "%d", WOOD_SOLD);    //on met les quantités de ressources necessaires
        textprintf_ex(rep, font, RES_W + RES_S + (2*MARGE), YTEXT, COL_UI_ACC, -1, "%d", ROCK_SOLD);
        textprintf_ex(rep, font, 2*RES_W + RES_S + (2*MARGE), YTEXT, COL_UI_ACC, -1, "%d", 0);

    break;

        case PEASANT: //y'a deux options (pour l'instant)
        for (i=0;i<2;i++)
        {
            masked_blit(inter, rep, 0, 0, 0, i*BOX_H, BOX_W, BOX_H);

            textprintf_ex(rep, font, RES_S + (2*MARGE), i*BOX_H + YTEXT, COL_UI_ACC, -1, "%d", (!i)?WOOD_MAINB:WOOD_BARACKS);    //on met les quantités de ressources necessaires
            textprintf_ex(rep, font, RES_W + RES_S + (2*MARGE), i*BOX_H + YTEXT, COL_UI_ACC, -1, "%d", (!i)?ROCK_MAINB:ROCK_BARACKS);
            textprintf_ex(rep, font, 2*RES_W + RES_S + (2*MARGE), i*BOX_H + YTEXT, COL_UI_ACC, -1, "%d", 0);
        }

        masked_blit(sprites.mairie_i, rep, 0, 0, 0, BOX_H - ICON_S, ICON_S, ICON_S); //on met l'icone au bon endroit

        masked_blit(sprites.caserne_i, rep, 0, 0, 0, 2*BOX_H - ICON_S, ICON_S, ICON_S); //on met l'icone au bon endroit


    break;

        default:
    break;
    }
    destroy_bitmap(inter);


    return rep;
}

//charge le tableau de boutons
void load_buttons(Sprites& sprites)
{
    int i, j;
    BITMAP *inter;

    sprites.fond[0] = load_bitmap(MENU_FOND, NULL); ERR_CHARG(sprites.fond[0])
    sprites.fond[1] = load_bitmap(PAUSE_FOND, NULL); ERR_CHARG(sprites.fond[1])

    //chargement des boutons du menu
    inter = load_bitmap(MENU_BUTTON, NULL); ERR_CHARG(inter)

    for (i=0;i<2;i++)
    {
        for (j=0;j<3;j++)
        {
            sprites.hover_unit[i][j] = create_bitmap(HOVER_W, BOUTON_H);
            blit(inter, sprites.hover_unit[i][j], i*HOVER_W, j*BOUTON_H, 0, 0, HOVER_W, BOUTON_H);
        }
    }


    for (i=0;i<3;i++)
    {
        sprites.buttons[MAIN_MENU][i][0] = create_bitmap(BOUTON_W, BOUTON_H);
        rectfill(sprites.buttons[MAIN_MENU][i][0], 0, 0, BOUTON_W, BOUTON_H, MAG);

        for (j=0;j<3;j++)
        {
            sprites.buttons[MAIN_MENU][i][j+1] = create_bitmap(BOUTON_W, BOUTON_H);
            blit(inter, sprites.buttons[MAIN_MENU][i][j+1], 2*HOVER_W + i*BOUTON_W, j*BOUTON_H, 0, 0, BOUTON_W, BOUTON_H);
        }
    }
    destroy_bitmap(inter);

    //chargement des boutons du menu pause
    inter = load_bitmap(PAUSE_BUTTON, NULL); ERR_CHARG(inter)
    for (i=0;i<3;i++)
    {
        for (j=0;j<4;j++)
        {
            sprites.buttons[PAUSE_MENU][i][j] = create_bitmap(BOUTON_W, BOUTON_H);
            blit(inter, sprites.buttons[PAUSE_MENU][i][j], i*BOUTON_W, j*BOUTON_H, 0, 0, BOUTON_W, BOUTON_H);
        }
    }
    destroy_bitmap(inter);


    inter = load_bitmap(MAP_BUTTON, NULL); ERR_CHARG(inter)
    for (i=0;i<3;i++)
    {
        for (j=0;j<4;j++)
        {
            sprites.buttons[MAP_MENU][i][j] = create_bitmap(BOUTON_W, BOUTON_H);
            blit(inter, sprites.buttons[MAP_MENU][i][j], i*BOUTON_W, j*BOUTON_H, 0, 0, BOUTON_W, BOUTON_H);
        }
    }
    destroy_bitmap(inter);

    inter = load_bitmap(DIFF_BUTTON, NULL); ERR_CHARG(inter)
    for (i=0;i<3;i++)
    {
        for (j=0;j<4;j++)
        {
            sprites.buttons[DIFF_MENU][i][j] = create_bitmap(BOUTON_W, BOUTON_H);
            blit(inter, sprites.buttons[DIFF_MENU][i][j], i*BOUTON_W, j*BOUTON_H, 0, 0, BOUTON_W, BOUTON_H);
        }
    }
    destroy_bitmap(inter);

    inter = load_bitmap(SAVE_BUTTON, NULL); ERR_CHARG(inter)
    for (i=0;i<3;i++)
    {
        for (j=0;j<4;j++)
        {
            sprites.buttons[SAVE_MENU][i][j] = create_bitmap(BOUTON_W, BOUTON_H);
            blit(inter, sprites.buttons[SAVE_MENU][i][j], i*BOUTON_W, j*BOUTON_H, 0, 0, BOUTON_W, BOUTON_H);
        }
    }
    destroy_bitmap(inter);

    sprites.flags = load_bitmap(FLAGS, NULL); ERR_CHARG(sprites.flags)


    inter = load_bitmap(VICTORY_BUTTON, NULL); ERR_CHARG(inter)
    for (i=0;i<3;i++)
    {
        for (j=0;j<2;j++)
        {
            sprites.v_buttons[i][j] = create_bitmap(BOUTON_W, BOUTON_H);
            blit(inter, sprites.v_buttons[i][j], i*BOUTON_W, j*BOUTON_H, 0, 0, BOUTON_W, BOUTON_H);
        }
    }
    destroy_bitmap(inter);
}

//ce sousprogramme sert à charger les bitmaps et sprites
void load_sprites(Sprites& sprites)
{
    int i, j;
    BITMAP *inter[5]; //des bitmaps intermediaires, quand besoin est
    char nom[50];

    { //les unités
        //on charge les bitmaps du soldat
        inter[0] = load_bitmap(EPEE, NULL);
        for (i=0;i<2;i++)
        {
            for (j=0;j<NUMFRAMES;j++)
            {
                sprites.sword[i][j] = create_bitmap(COTE, COTE);
                blit(inter[0], sprites.sword[i][j], j*COTE, i*COTE, 0, 0, COTE, COTE);

                ERR_CHARG(sprites.sword[i][j])
            }
        }
        destroy_bitmap(inter[0]);

        //on charge les bitmaps du paysant
        inter[0] = load_bitmap(PAYSANT, NULL);
        for (i=0;i<3;i++)
        {
            for (j=0;j<NUMFRAMES;j++)
            {
                sprites.peasant[i][j] = create_bitmap(COTE, COTE);
                blit(inter[0], sprites.peasant[i][j], j*COTE, i*COTE, 0, 0, COTE, COTE);

                ERR_CHARG(sprites.peasant[i][j])
            }
        }
        destroy_bitmap(inter[0]);


        //on charge les bitmaps de l'ennemi
        inter[0] = load_bitmap(MONSTRE, NULL);
        for (i=0;i<2;i++)
        {
            for (j=0;j<NUMFRAMES;j++)
            {
                sprites.ennemi[i][j] = create_bitmap(COTE, COTE);
                blit(inter[0], sprites.ennemi[i][j], j*COTE, i*COTE, 0, 0, COTE, COTE);


                ERR_CHARG(sprites.ennemi[i][j])
            }
        }
        destroy_bitmap(inter[0]);
    }

    { //les cases
        //on charge les bitmaps des tuiles "herbe"
        for (i=0;i<15;i++)
        {
            sprintf(nom, HERBE, RES, i);
            sprites.herbe[i] = load_bitmap(nom, NULL); ERR_CHARG(sprites.herbe[i])
        }

        //les tuile "bridge"
        for (i=0;i<4;i++)
        {
            sprintf(nom, HERBE, RES, i+15);
            inter[0] = load_bitmap(nom, NULL); ERR_CHARG(inter[0])
            sprites.herbe[i+15] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_SABLE], sprites.herbe[i+15], 0, 0, 0, 0, COTE, COTE);
            masked_blit(inter[0], sprites.herbe[i+15], 0, 0, 0, 0, COTE, COTE);

            destroy_bitmap(inter[0]);
        }


        //on charge les bitmaps des tuiles "eau"
        for (i=0;i<13;i++)
        {
            sprintf(nom, EAU, RES, i);
            sprites.eau[i] = load_bitmap(nom, NULL); ERR_CHARG(sprites.eau[i])
        }

        //on charge les bitmaps des tuiles "montagnes"
        for (i=0;i<13;i++)
        {
            sprintf(nom, MONTAGNE, RES, i);
            sprites.montagne[i] = load_bitmap(nom, NULL); ERR_CHARG(sprites.montagne[i])
        }

        //on charge les bitmaps des tuiles "arbre"
        inter[0] = load_bitmap(ARBRE0, NULL); ERR_CHARG(inter[0])
        inter[1] = load_bitmap(ARBRE1, NULL); ERR_CHARG(inter[1])
        inter[2] = load_bitmap(ARBRE2, NULL); ERR_CHARG(inter[2])
        inter[3] = load_bitmap(ARBRE3, NULL); ERR_CHARG(inter[3])
        inter[4] = load_bitmap(ARBRE4, NULL); ERR_CHARG(inter[4])

        for (i=0;i<5;i++)
        {
            sprites.arbre[i] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_HERBE], sprites.arbre[i], 0, 0, 0, 0, COTE, COTE); //on met sur fond d'herbe
            masked_blit(inter[i], sprites.arbre[i], 0, 0, 0, 0, COTE, COTE);
            destroy_bitmap(inter[i]);
        }


        //on charge les bitmaps des tuiles "pierre"
        inter[0] = load_bitmap(PIERRE0, NULL); ERR_CHARG(inter[0])

        for (i=0;i<1;i++)
        {
            sprites.pierre[i] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_SABLE], sprites.pierre[i], 0, 0, 0, 0, COTE, COTE); //on met sur fond de sable
            masked_blit(inter[i], sprites.pierre[i], 0, 0, 0, 0, COTE, COTE);
            destroy_bitmap(inter[i]);
        }

        //on charge les bitmaps des tuiles "batiment"
        for (i=0;i<4;i++)
        {
            sprintf(nom, MAIRIE0, RES, i);
            inter[0] = load_bitmap(nom, NULL); ERR_CHARG(inter[0])

            sprites.batiment[0][i] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_HERBE], sprites.batiment[0][i], 0, 0, 0, 0, COTE, COTE);
            masked_blit(inter[0], sprites.batiment[0][i], 0, 0, 0, 0, COTE, COTE);

            sprites.batiment[1][i] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_HERBE], sprites.batiment[1][i], 0, 0, 0, 0, COTE, COTE);
            masked_blit(inter[0], sprites.batiment[1][i], 0, 0, 0, 0, COTE, COTE);

            destroy_bitmap(inter[0]);
        }


        //la caserne
        for (i=0;i<4;i++)
        {
            sprintf(nom, CASERNE0, RES, i);
            inter[0] = load_bitmap(nom, NULL); ERR_CHARG(inter[0])

            sprites.batiment[0][i+4] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_HERBE], sprites.batiment[0][i+4], 0, 0, 0, 0, COTE, COTE);
            masked_blit(inter[0], sprites.batiment[0][i+4], 0, 0, 0, 0, COTE, COTE);

            sprites.batiment[1][i+4] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_HERBE], sprites.batiment[1][i+4], 0, 0, 0, 0, COTE, COTE);
            masked_blit(inter[0], sprites.batiment[1][i+4], 0, 0, 0, 0, COTE, COTE);

            destroy_bitmap(inter[0]);
        }

        //on charge les bitmaps des campemants ennemis
        for (i=0;i<4;i++)
        {
            sprintf(nom, CAMP0, RES, i);
            inter[0] = load_bitmap(nom, NULL); ERR_CHARG(inter[0])

            sprites.camp[0][i] = create_bitmap(COTE, COTE);
            blit(sprites.herbe[P_HERBE], sprites.camp[0][i], 0, 0, 0, 0, COTE, COTE);
            masked_blit(inter[0], sprites.camp[0][i], 0, 0, 0, 0, COTE, COTE);


            destroy_bitmap(inter[0]);
        }

    }

    { //les autres bitmaps
        //on charge le brouillard de guerre
        sprites.brouillard = load_bitmap(FOG_TILE, NULL); ERR_CHARG(sprites.brouillard)

        sprites.fog = load_bitmap(BROUILLARD, NULL); ERR_CHARG(sprites.fog)

        //on charge la bitmap de la souris
        sprites.souris = load_bitmap(MOUSE, NULL); ERR_CHARG(sprites.souris)

        //on charge les bitmaps des pt'is icones
        sprites.bois_i = load_bitmap(BOIS, NULL); ERR_CHARG(sprites.bois_i)
        sprites.pierre_i = load_bitmap(PIERRE, NULL); ERR_CHARG(sprites.pierre_i)
        sprites.viande_i = load_bitmap(VIANDE, NULL); ERR_CHARG(sprites.viande_i)
        sprites.camp_i = load_bitmap(CAMP_ICON, NULL); ERR_CHARG(sprites.camp_i)
        sprites.enemy_i = load_bitmap(MANQUE, NULL); ERR_CHARG(sprites.enemy_i)
        sprites.pause_i[0] = load_bitmap(PAUSE_ICON, NULL); ERR_CHARG(sprites.pause_i[0])
        sprites.pause_i[1] = load_bitmap(PAUSE_ICON_HOVER, NULL); ERR_CHARG(sprites.pause_i[1])
        sprites.paysant_i = load_bitmap(PAYSANT_ICON, NULL); ERR_CHARG(sprites.paysant_i)
        sprites.epee_i = load_bitmap(EPEE_ICON, NULL); ERR_CHARG(sprites.epee_i)
        sprites.mairie_i = load_bitmap(MAIRIE_ICON, NULL); ERR_CHARG(sprites.mairie_i)
        sprites.caserne_i = load_bitmap(CASERNE_ICON, NULL); ERR_CHARG(sprites.caserne_i)

        //on charge les bitmaps "ignorées" (celles en bas et en bas à gauche pour pouvoir déplacer la map
        sprites.ign_l = load_bitmap(IGN_L, NULL); ERR_CHARG(sprites.ign_l)
        sprites.ign_d = load_bitmap(IGN_D, NULL); ERR_CHARG(sprites.ign_d)
        sprites.c_bar = load_bitmap(C_BAR, NULL); ERR_CHARG(sprites.c_bar)
        sprites.r_bar = load_bitmap(UNIT_MENU, NULL); ERR_CHARG(sprites.r_bar)
        sprites.info_bar = load_bitmap(INFO_BAR, NULL); ERR_CHARG(sprites.info_bar)


        //on charge les petits menus en bas à gauche dans l'ui
        for (i=0;i<3;i++)
        {
            sprites.small_menu[i] = create_menu(sprites, i);
        }

        //on charge les fleurs
        for (i=0;i<10;i++)
        {
            sprintf(nom, FLEUR, RES, i);
            sprites.flower[i] = load_bitmap(nom, NULL);
        }

        //chargement des boutons du menu
        load_buttons(sprites);
    }
}


//fin du fichier
