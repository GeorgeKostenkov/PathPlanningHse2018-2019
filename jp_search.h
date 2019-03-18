#ifndef JP_SEARCH_H
#define JP_SEARCH_H
#include "astar.h"

class JP_Search:public Astar
{
public:
    JP_Search(float hweight, bool breakingties):Astar(hweight, breakingties){}
    ~JP_Search();

private:
    bool Neighbours(int dir_i, int dir_j, Node current, const Map &map);
    void Jump_Points(int dir_i, int dir_j, Node current, const Map &map, std::list<Node> &successors);
    std::list<Node> findSuccessors(Node current, const Map &map, const EnvironmentOptions &options);
    void makePrimaryPath(Node current);
    int find_dir(int cur, int par);
    void ModifyNode(Node &current, int dir_i, int dir_j);
};

#endif // JP_SEARCH_H
