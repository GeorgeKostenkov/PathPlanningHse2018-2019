#ifndef THETA_H
#define THETA_H
#include "astar.h"

class Theta: public Astar
{
    public:
        Theta(double hweight, bool breakingties) : Astar(hweight, breakingties){}
        ~Theta(void);

    protected:
        Node resetParent(Node current, Node parent, const Map &map);
        bool lineOfSight(int start_i, int start_j, int finish_i, int finish_j, const Map &map);
        void makePrimaryPath(Node curNode);
        void makeSecondaryPath();
};

#endif // THETA_H
