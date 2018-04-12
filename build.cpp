#include "build.h"

#include "evennement.h"

using namespace std;

Build::Build()
{

}

Build::Build(int _x, int _y, int _w, int _h, int _side, int _state)
    :m_pos(_x, _y), w(_w), h(_h), state(_state), side(_side), hp_max(HP_BUILD_S),
    range(DEFAULT_RANGE), damage(DEFAULT_DMG), statione(0), curr_queue(0)
{
    //width height
    //l'état du batiment (normal, en construction..)
    //la portée d'attaque du batiment (ne sert pour l'instant à rien)
    //les dégats du batiment (ne sert pour l'instant à rien)

    //il n'y a aucune unité en cours de formation dans ce batiment

    hp = state?10:hp_max; //si le batiment commence à être construit il recevra les pv au fur et à mesure de la construction

    cap = side?0:5;
}

Build::~Build()
{

}

void Build::sendStream(ostream& theStream, int version)
{
    theStream << m_pos << " " << w << " " << h << endl;

    theStream << state << " " << side << " " << hp << " " << hp_max << " " << range << " " << damage << endl;

    theStream << cap << " " << statione << endl;

    theStream << endl;

    theStream << curr_queue << endl;
    for (int i=0;i<curr_queue;i++) //la queue de formation d'unités
    {
        theStream << unit_queue[i] << endl;
    }
}

void Build::receiveStream(istream& theStream, Tile carte[MAPSIZEX][MAPSIZEY], int version)
{
    theStream >> m_pos >> w >> h;

    theStream >> state >> side >> hp >> hp_max >> range >> damage;

    theStream >> cap >> statione;

    theStream >> curr_queue;
    for (int i=0;i<curr_queue;i++) //la queue de formation d'unités
    {
        theStream >> unit_queue[i];
    }

    for (int k=0;k<w;k++)
    {
        for (int j=0;j<h;j++) //toutes les cases (tuiles)
        {
            carte[m_pos.x+k][m_pos.y+j].erige = this; //on y met le pointeur sur batiment
        }
    }

    getTime(start);

    //on éclaire la zone
    if (side==ALLY)
    {
        ///REMPLACER ÇA PAR RANGE...
        eclaire(carte, (m_pos.x + w/2) * COTE, (m_pos.y + h/2) * COTE, 4);
    }
}





