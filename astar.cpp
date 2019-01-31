#include "astar.h"

Astar::Astar(double HW, bool BT)
{
    hweight = HW;
    breakingties = BT;
}

double Astar::computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options)
{
    //need to implement

    int dx = abs(i2 - i1), dy = abs(j2 - j1);
    if (options.metrictype == CN_SP_MT_DIAG) {
        return dx + dy + (CN_SQRT_TWO - 2) * std::min(dx, dy);
    } else if (options.metrictype == CN_SP_MT_MANH) {
        return dx + dy;
    } else if (options.metrictype == CN_SP_MT_EUCL) {
        return sqrt(dx * dx + dy * dy);
    } else if (options.metrictype == CN_SP_MT_CHEB) {
        return dx + dy -  std::min(dx, dy);
    } else {
        return 0;
    }
}
