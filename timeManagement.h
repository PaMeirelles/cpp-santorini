#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

struct TMInfo {
    int remainingTime;
    int ply;

    TMInfo(int remainingTime, int ply);
};

int estimatedTurnsLeft(int remainingTime, int a);
int et_s(TMInfo info);
int et_p(TMInfo info);
int et_f(TMInfo info);
int eg_c(TMInfo info);
#endif