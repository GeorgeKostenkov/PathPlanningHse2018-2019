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
    start.i = map.getStart_x();
    start.j = map.getStart_y();
    finish.i = map.getFinish_x();
    finish.j = map.getFinish_y();
    start.parent = nullptr;
    unsigned long long steps = 0;
    double success_curr_cost;
    int height = map.getMapHeight();
    start.g = 0;
    start.H = computeHFromCellToCell(start.i, start.j, finish.i, finish.j, options);
    start.F = hweight * start.H;
    open.push_back(start);
    Node current;
    std::chrono::time_point<std::chrono::system_clock> start_time, end;
    start_time = std::chrono::system_clock::now();
    while (!open.empty()) {
        steps++;
        auto optimum = optimal(open);
        current = *optimum;
        open.erase(optimum);
        /*for (auto x : open) {
            std::cout << x.i << ' ' << x.j << ' ' << x.F << ' ' << x.H << ' ' << x.g << '\n';
        }
        std::cout << '\n';*/
        close[hash(current, height)] = current;
        if (current == finish) {
            sresult.pathfound = 1;
            break;
        }
        successors = findSuccessors(current, map, options);
        for (auto success : successors) {
            if (close.find(hash(success, height)) != close.end())
                continue;
            if (current.i == success.i || current.j == success.j)
                success_curr_cost = current.g + 1;
            else
                success_curr_cost = current.g + CN_SQRT_TWO;
            bool not_opened = not_in_open(success, open);
            if (not_opened || success_curr_cost < success.g) {
                success.parent = &close[hash(current, height)];
                success.g = success_curr_cost;
                success.H = computeHFromCellToCell(success.i, success.j, finish.i, finish.j, options);
                success.F = success.g + success.H;
                if (not_opened)
                    open.push_back(success);
            }
        }
    }
    if (sresult.pathfound == 1)
        makePrimaryPath(current);
    end = std::chrono::system_clock::now();
    sresult.time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_time).count()) / 1000000000;
    sresult.nodescreated = close.size() + open.size() + 1;
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
    int height = map.getMapHeight();
    Node child;
    int x = curNode.i, y = curNode.j;
    for (int i = x - 1; i < x + 2; ++i) {
        for (int j = y - 1; j < y + 2; ++j) {
            child.i = i;
            child.j = j;
            if (map.CellOnGrid(i, j) && map.CellIsTraversable(i, j) && close.find(hash(child, height)) == close.end() && !(i == x && j == y)) {
                if (abs(x - i) + abs(y - j) == 1) {
                    successors.push_back(child);
                } else if (options.allowdiagonal) {
                    if (map.CellIsTraversable(i, y) && map.CellIsTraversable(x, j)) {
                        successors.push_back(child);
                    } else if (options.cutcorners) {
                        if (map.CellIsTraversable(i, y) || map.CellIsTraversable(x, j)) {
                            successors.push_back(child);
                        } else if (options.allowsqueeze) {
                            successors.push_back(child);
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

long int ISearch::hash (Node node, int height) {
    return node.i + node.j * height;
}

std::_List_iterator<Node> ISearch::optimal(std::list<Node> &list) {
    auto optimal = list.begin();
    for (auto it = list.begin(); it != list.end(); ++it){
        if ((*optimal).F > (*it).F)
            optimal = it;
    }
    return optimal;
}

bool ISearch::not_in_open(Node node, std::list<Node> &list) {
    for (auto it = list.begin(); it != list.end(); ++it){
        if ((*it) == node) {
            return 0;
        }
    }
    return 1;
}

void ISearch::makeSecondaryPath()
{
    int x1, x2, y1, y2;
    auto first = lppath.begin();
    auto last = lppath.end()--;
    while (first != last) {
        x1 = (*first).i;
        y1 = (*first).j;
        ++first;
        x2 = (*first).i;
        y2 = (*first).j;
    }
}
