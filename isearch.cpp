    #include "isearch.h"

ISearch::ISearch()
{
    hweight = 1;
    breakingties = CN_SP_BT_GMAX;
}

ISearch::~ISearch(void) {}

SearchResult ISearch::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    //need to implement
    std::list<Node> successors;
    Node start , finish;
    bool in_open = 0;
    start.i = map.getStart_i();
    start.j = map.getStart_j();
    finish.i = map.getFinish_i();
    finish.j = map.getFinish_j();
    start.parent = nullptr;
    unsigned int steps = 0;
    double success_curr_cost;
    int height = map.getMapHeight();
    start.g = 0;
    start.H = computeHFromCellToCell(start.i, start.j, finish.i, finish.j, options);
    start.F = hweight * start.H;
    open.push_front(start);
    Node current;
    std::chrono::time_point<std::chrono::system_clock> start_time, end;
    start_time = std::chrono::system_clock::now();
    while (!open.empty()) {
        steps++;
        auto optimum = optimal();
        current = *optimum;
        open.erase(optimum);
        close[hash(current, height)] = current;
        if (current == finish) {
            sresult.pathfound = 1;
            break;
        }
        successors = findSuccessors(current, map, options);
        for (auto success : successors) {
            in_open = 0;
            if (close.find(hash(success, height)) != close.end())
                continue;
            success.H = hweight * computeHFromCellToCell(success.i, success.j, finish.i, finish.j, options);
            if (success.i == current.i || success.j == current.j) {
                success_curr_cost = current.g + 1;
            }
            else {
                success_curr_cost = current.g + sqrt(2);
            }
            for (auto it = open.begin(); it != open.end(); ++it){
                if (*it == success) {
                    in_open = 1;
                    if (success_curr_cost < it->g) {
                        it->g = success_curr_cost;
                        it->F = it->g + it->H;
                        it->parent = &close[hash(current, height)];
                    }
                }
            }
            if (!in_open) {
                success.g = success_curr_cost;
                success.F = success.g + success.H;
                success.parent = &close[hash(current, height)];
                open.push_front(success);
            }
        }
    }
    if (sresult.pathfound == 1)
        makePrimaryPath(current);
    end = std::chrono::system_clock::now();
    sresult.time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_time).count()) / 1000000000;
    sresult.nodescreated = close.size() + open.size();
    sresult.numberofsteps = steps;
    sresult.pathlength = current.g;
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;
    return sresult;
}

std::list<Node> ISearch::findSuccessors(Node curNode, const Map &map, const EnvironmentOptions &options)
{
    std::list<Node> successors;
    //need to implement
    Node child;
    int x = curNode.i, y = curNode.j;
    for (int i = x - 1; i < x + 2; ++i) {
        for (int j = y - 1; j < y + 2; ++j) {
            child.i = i;
            child.j = j;
            if (map.CellOnGrid(i, j) && map.CellIsTraversable(i, j) && !(i == x && j == y)) {
                if (abs(x - i) + abs(y - j) == 1) {
                    successors.push_front(child);
                } else if (options.allowdiagonal) {
                    if (map.CellIsTraversable(i, y) && map.CellIsTraversable(x, j)) {
                        successors.push_front(child);
                    } else if (options.cutcorners) {
                        if (map.CellIsTraversable(i, y) || map.CellIsTraversable(x, j)) {
                            successors.push_front(child);
                        } else if (options.allowsqueeze) {
                            successors.push_front(child);
                        }
                    }
                }
            }
        }
    }
    return successors;
}



void ISearch::makePrimaryPath(Node curNode)
{
    Node current = curNode;
    while (current.parent) {
        lppath.push_front(current);
        current = *(current.parent);
    }
    lppath.push_front(current);
}

int ISearch::hash (Node node, int height) {
    return node.i + node.j * height;
}

std::_List_iterator<Node> ISearch::optimal() {
    auto optimal = open.begin();
    for (auto it = open.begin(); it != open.end(); ++it){
        if ((*it).F < (*optimal).F) {
            optimal = it;
        } else if ((*it) == (*optimal)) {
            if (((*it).g > (*optimal).g && CN_SP_BT_GMAX)
                    || ((*it).g < (*optimal).g && CN_SP_BT_GMIN))
                optimal = it;
            else if ((*it).g == (*optimal).g) {
                if ((*it).H < (*optimal).H)
                    optimal = it;
            }
        }
    }
    return optimal;
}

void ISearch::makeSecondaryPath() {
    auto it = lppath.begin();
    int first_I, first_J, second_I, second_J, diff1, diff2;
    hppath.push_back(*it);
    while (it != --lppath.end()) {
        first_I = it->i;
        first_J = (it++)->j;
        second_I = it->i;
        second_J = (it++)->j;
        diff1 = second_I - first_I;
        diff2 = second_J - first_J;
        if ((it->i - second_I) != diff1 || (it->j - second_J) != diff2)
            hppath.push_back(*(--it));
        else
            --it;
    }
}

