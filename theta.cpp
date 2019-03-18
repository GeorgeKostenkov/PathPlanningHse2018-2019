#include "theta.h"

Theta::~Theta(){}

bool Theta::lineOfSight(int start_i, int start_j, int finish_i, int finish_j, const Map &map) {
    int i = start_i;
    int j = start_j;
    int i_diff = std::abs(i - finish_i);
    int j_diff = std::abs(j - finish_j);
    int dir_i, dir_j;
    if (i < finish_i)
        dir_i = 1;
    else
        dir_i = -1;
    if (j < finish_j)
        dir_j = 1;
    else
        dir_j = -1;
    int f = 0;
    if (i_diff < j_diff) {
        while (j != finish_j) {
            if (map.CellIsObstacle(i, j))
                return false;
            else
                f += i_diff;
            if (f >= j_diff) {
                if (map.CellIsObstacle(i + (dir_i - 1)/2, j + (dir_j - 1)/2))
                    return false;
                i += dir_i;
                f -= j_diff;
            }
            if (f != 0 && map.CellIsObstacle(i + (dir_i - 1)/2, j + (dir_j - 1)/2))
                return false;
            if (i_diff == 0 && map.CellIsObstacle(i, j + (dir_j - 1)/2)
                    && map.CellIsObstacle(i - 1, j + (dir_j - 1)/2))
                return false;
            j += dir_j;
        }
        return true;
    }
    while (i != finish_i) {
        if (map.CellIsObstacle(i, j))
            return false;
        else
            f += j_diff;
        if (f >= i_diff) {
            if (map.CellIsObstacle(i + (dir_i - 1)/2, j + (dir_j - 1)/2))
                return false;
            j += dir_j;
            f -= i_diff;
        }
        if (f && map.CellIsObstacle(i + (dir_i - 1)/2, j + (dir_j - 1)/2))
            return false;
        if (!j_diff && map.CellIsObstacle(i + (dir_i - 1)/2, j)
                && map.CellIsObstacle(i + (dir_i - 1)/2, j - 1))
            return false;
        i += dir_i;
    }
    return true;
}

Node Theta::resetParent(Node cur, Node par, const Map &map) {
    if (!par.parent)
        return cur;
    if (lineOfSight(par.parent->i, par.parent->j, cur.i, cur.j, map)) {
        int i = par.parent->i;
        int j = par.parent->j;
        cur.g = par.parent->g + sqrt((i - cur.i) * (i - cur.i)  + (j - cur.j) * (j - cur.j));
        cur.parent = par.parent;
        return cur;
    }
    return cur;
}

void Theta::makeSecondaryPath()
{
    Node *first = &*hppath.begin();
    auto it = hppath.begin();
    ++it;
    Node *second = &*it;
    lppath.push_back(*first);
    while(second != &*hppath.end()) {
        int first_i = first->i;
        int first_j = first->j;
        int second_i = second->i;
        int second_j = second->j;
        int i_diff = std::abs(first_i - second_i);
        int j_diff = std::abs(first_j - second_j);
        int dir_i, dir_j, i, j;
        if (first_i < second_i)
            dir_i = 1;
        else
            dir_i = -1;
        if (first_j < second_j)
            dir_j = 1;
        else
            dir_j = -1;
        int f = 0;
        if (i_diff < j_diff) {
            i = first_i;
            j = first_j;
            while (j != second_j) {
                lppath.push_back({i, j, 0, 0, 0, nullptr});
                f += i_diff;
                if (f >= j_diff) {
                    i += dir_i;
                    f -= j_diff;
                }
                j += dir_j;
            }
        }
        else {
            i = first_i;
            j = first_j;
            while (i != second_i) {
                lppath.push_back({i, j, 0, 0, 0, nullptr});
                f += j_diff;
                if (f >= i_diff) {
                    j += dir_j;
                    f -= i_diff;
                }
                i += dir_i;
            }
        }
        first = second;
        ++it;
        second = &*it;
    }
}

void Theta::makePrimaryPath(Node curNode)
{
    Node current = curNode;
    while(current.parent) {
        hppath.push_front(current);
        current = *current.parent;
    }
    hppath.push_front(current);
}
