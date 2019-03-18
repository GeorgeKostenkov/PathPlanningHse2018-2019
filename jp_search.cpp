#include "jp_search.h"

JP_Search::~JP_Search()
{
}

void JP_Search::ModifyNode(Node &current, int dir_i, int dir_j) {
    current.i += dir_i;
    current.j += dir_j;
    if (dir_i == 0 || dir_j == 0)
        current.g += 1;
    else
        current.g += sqrt(2);
}

int JP_Search::find_dir(int cur, int par) {
    if (cur < par)
        return -1;
    else if (cur > par)
        return 1;
    else
        return 0;
}

void JP_Search::Jump_Points(int dir_i, int dir_j, Node current, const Map &map, std::list<Node> &successors)
{
    bool found = false;
    while(!found) {
        if (map.CellOnGrid(current.i + dir_i, current.j + dir_j)) {
            if (map.CellIsTraversable(current.i + dir_i, current.j + dir_j)) {
                ModifyNode(current, dir_i, dir_j);
            }
            else
                return;
        }
        else
            return;
        if (map.getFinish_i() == current.i && map.getFinish_j() == current.j)
            found = true;
        if (!dir_j) {
            if (map.CellOnGrid(current.i + dir_i, current.j + 1))
                if (map.CellIsTraversable(current.i + dir_i, current.j + 1)
                        && map.CellIsObstacle(current.i, current.j + 1))
                    found = true;
            if (map.CellOnGrid(current.i + dir_i, current.j - 1))
                if (map.CellIsTraversable(current.i + dir_i, current.j - 1)
                        && map.CellIsObstacle(current.i, current.j - 1))
                    found = true;
        } else if (!dir_i) {
            if (map.CellOnGrid(current.i + 1, current.j + dir_j))
                if (map.CellIsTraversable(current.i + 1, current.j + dir_j)
                        && map.CellIsObstacle(current.i + 1, current.j))
                    found = true;
            if (map.CellOnGrid(current.i - 1,current.j + dir_j))
                if (map.CellIsTraversable(current.i - 1, current.j + dir_j)
                        && map.CellIsObstacle(current.i - 1, current.j))
                    found = true;
        } else {
            if (map.CellOnGrid(current.i - dir_i, current.j + dir_j))
                if (map.CellIsObstacle(current.i - dir_i, current.j)
                        && map.CellIsTraversable(current.i - dir_i, current.j + dir_j))
                    found = true;
            if (!found && map.CellOnGrid(current.i + dir_i, current.j - dir_j))
                if (map.CellIsObstacle(current.i, current.j - dir_j)
                        && map.CellIsTraversable(current.i + dir_i, current.j - dir_j))
                    found = true;
            if (!found)
                if (Neighbours(dir_i, 0, current, map) || Neighbours(0, dir_j, current, map))
                    found = true;
        }

    }
    if (close.find(current.i * map.getMapWidth() + current.j) == close.end())
        successors.push_front(current);
    return;
}

bool JP_Search::Neighbours(int dir_i, int dir_j, Node current, const Map &map)
{
    while(map.CellOnGrid(current.i, current.j) && map.CellIsTraversable(current.i, current.j)) {
        if (map.getFinish_i() == current.i && map.getFinish_j() == current.j)
            return true;
        if (!dir_i && map.CellOnGrid(current.i, current.j + dir_j)) {
            if (map.CellOnGrid(current.i + 1, current.j))
                if (map.CellIsTraversable(current.i + 1, current.j + dir_j)
                        && map.CellIsObstacle(current.i + 1, current.j))
                    return true;
            if (map.CellOnGrid(current.i - 1,current.j))
                if (map.CellIsTraversable(current.i - 1, current.j + dir_j)
                        && map.CellIsObstacle(current.i - 1, current.j))
                    return true;
        }
        if (!dir_j && map.CellOnGrid(current.i + dir_i, current.j)) {
            if (map.CellOnGrid(current.i, current.j + 1))
                if (map.CellIsTraversable(current.i + dir_i, current.j + 1)
                        && map.CellIsObstacle(current.i, current.j + 1))
                    return true;
            if (map.CellOnGrid(current.i, current.j - 1))
                if (map.CellIsTraversable(current.i + dir_i, current.j - 1)
                        && map.CellIsObstacle(current.i, current.j - 1))
                    return true;
        }

        current.i += dir_i;
        current.j += dir_j;
    }
    return false;
}

std::list<Node> JP_Search::findSuccessors(Node current, const Map &map, const EnvironmentOptions &options)
{
    int dir_i = 0, dir_j = 0;
    std::list<Node> successors;
    if (current.i == map.getStart_i() && current.j == map.getStart_j())
        for(int i = -1; i <= 1; i++)
            for(int j = -1; j <= 1; j++)
                if (i != 0 || j != 0)
                    Jump_Points(i, j, current, map, successors);
    if (current.i != map.getStart_i() || current.j != map.getStart_j()) {
        dir_i = find_dir(current.i, current.parent->i);
        dir_j = find_dir(current.j, current.parent->j);
        Jump_Points(dir_i, dir_j, current, map, successors);
        if (dir_i && dir_j) {
            if (map.CellIsObstacle(current.i - dir_i, current.j))
                Jump_Points(-dir_i, dir_j, current, map, successors);
            if (map.CellIsObstacle(current.i, current.j - dir_j))
                Jump_Points(dir_i, -dir_j, current, map, successors);
            Jump_Points(dir_i, 0, current, map, successors);
            Jump_Points(0, dir_j, current, map, successors);
        }
        if (!dir_i) {
            if (map.CellOnGrid(current.i - dir_j, current.j))
                if (map.CellIsObstacle(current.i - dir_j, current.j))
                    Jump_Points(-dir_j, dir_j, current, map, successors);
            if (map.CellOnGrid(current.i + dir_j, current.j))
                if (map.CellIsObstacle(current.i + dir_j, current.j))
                    Jump_Points(dir_j, dir_j, current, map, successors);
        } else if (!dir_j) {
            if (map.CellOnGrid(current.i, current.j - dir_i))
                if (map.CellIsObstacle(current.i, current.j - dir_i))
                    Jump_Points(dir_i, -dir_i, current, map, successors);
            if (map.CellOnGrid(current.i, current.j + dir_i))
                if (map.CellIsObstacle(current.i, current.j + dir_i))
                    Jump_Points(dir_i, dir_i, current, map, successors);
        }
    }
    return successors;
}

void JP_Search::makePrimaryPath(Node curNode)
{
    Node current = curNode;
    while(current.parent) {
        hppath.push_front(current);
        current = *current.parent;
    }
    hppath.push_front(current);
}
