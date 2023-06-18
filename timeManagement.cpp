#include "timeManagement.h"

TMInfo::TMInfo(int r, int p){
    remainingTime = r;
    ply = p;
}

int estimatedTurnsLeft(int remainingTime, int a){
    return remainingTime / a;
}

int et_s(TMInfo info){
    return estimatedTurnsLeft(info.remainingTime, 15);
}

int et_p(TMInfo info){
    return estimatedTurnsLeft(info.remainingTime, 8);
}

int et_f(TMInfo info){
    return estimatedTurnsLeft(info.remainingTime, 25);
}

int estimatedGameLength(int remainingTime, int ply, int a){
    return remainingTime / (a - ply);
}

int eg_c(TMInfo info){
    return estimatedGameLength(info.remainingTime, info.ply, 40);
}