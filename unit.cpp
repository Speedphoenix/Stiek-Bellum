#include "unit.h"

/*
    UNE GRANDE PARTIE DES SOUS-PROGRAMMES DE CE FICHIER ONT ÉTÉ RECICLÉS DU MINI PROJET TP COLLECTION SUR DREAM THEATER (par Leonardo Jeanteur)

    ce fichier contient les sous programmes necessaires à la manipulation des maillons
*/

#include "evennement.h"

using namespace std;

Unit::Unit()
{

}

Unit::Unit(int _type, int _priority, int _x, int _y, Build *_bat)
    :m_pos(_x, _y), m_dest(_x, _y), step(NULL), xpath(DIV(_x)),
    ypath(DIV(_y)), changepath(1), unitsize(1), frame(0), state(MOVING),
    priority(_priority), prec(MOVING), direction(RIGHT), cote(COTE), bat(_bat)
{
    //l'unité est immobile à la création
    //par défaud une unité regarde à droite
    //pour l'instant toutes les unités ont la même taille
    //la première image de l'animation (immobile)
    //immobile
    //la serie d'image d'où predre la bitmap
    //priorité la plus basse
    //le batiment à pointer (si aplicable)

    switch (_type)
    {
        case SOLDIER:
        hp = HP_SOLDIER;
        hp_max = HP_SOLDIER;
        damage = 20;   //les degats de cette unité
        side = ALLY;
        type = SOLDIER;


        prod = 0;      //ne peut pas produire de ressources
        speed = 8*SCALE;     //vitesse de deplacement (pixels par tour de boucle)
        range = 80;    //portée d'attaque
        delay = 2.0;   //temps d'attente entre chaque attaque
        vision = 4;    //zone éclairée autours de cette unité
    break;

        case PEASANT:
        hp = HP_PEASANT;
        hp_max = HP_PEASANT;
        damage = 10;
        side = ALLY;
        type = PEASANT;


        prod = 1;      // peut produire des ressources
        speed = 6*SCALE;
        range = 80;
        delay = 2.0;
        vision = 4;
    break;

        case ENEMY:
        hp = HP_ENEMY;
        hp_max = HP_ENEMY;
        damage = 20;
        side = ENEMY;
        type = ENEMY;


        prod = 0;
        speed = 10*SCALE;
        range = 80;
        delay = 2.0;
        vision = 4;
    break;

        default:
    break;
    }

    //on initialise les temps pour l'attaque et l'animation
    getTime(since_a);
    getTime(since_w);
}

Unit::~Unit()
{

}


void Unit::sendStream(ostream& theStream, int version)
{
    Coord someCoords(-1, -1);

    theStream << m_pos << m_dest << endl;

    theStream << hp << " " << hp_max << " " << damage << " " << side << " " << type << " " << unitsize << endl;

    theStream << frame << " " << state << " " << priority << " " << prec << " " << direction << endl;

    theStream << cote << " " << prod << " ";
    theStream << (int)(speed/SCALE);
    theStream << " " << range << " " << delay << " " << vision << endl;


    //on se permet de ne pas mettre les temps depuis la dernière animation/attaque


    if (!bat || bat->hp<=0)
        theStream << someCoords << endl;
    else
        theStream << bat->m_pos << endl;

    theStream << endl;
}


void Unit::receiveStream(istream& theStream, Tile carte[MAPSIZEX][MAPSIZEY], int version)
{
    Coord taken;

    theStream >> m_pos >> m_dest;

    theStream >> hp >> hp_max >> damage >> side >> type >> unitsize;

    theStream >> frame >> state >> priority >> prec >> direction;

    theStream >> cote >> prod;

    theStream >> speed;
    speed*=SCALE;

    theStream >> range >> delay >> vision;
    E(vision)

    //on se permet de ne pas mettre les temps depuis la dernière animation/attaque

    theStream >> taken;

    if (taken.x==-1 || taken.y==-1)
        bat = nullptr;
    else if (carte[taken.x][taken.y].erige)
        bat = carte[taken.x][taken.y].erige;
    else
        bat = nullptr;


    getTime(since_a); //les temps ne sont pas enregistrés
    getTime(since_w);


    changepath = 1;
    step = nullptr;
    xpath = DIV(m_dest.x);
    ypath = DIV(m_dest.y);
}


//fin du fichier
