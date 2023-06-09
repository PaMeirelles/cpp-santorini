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
    if((a - (ply / 2)) != 0){
        return remainingTime / (a - (ply / 2));
    }
    else{
        return remainingTime;
    }
}

int eg_c(TMInfo info){
    return estimatedGameLength(info.remainingTime, info.ply, 40);
}

int withEmerg(TMInfo info){
    if(info.ply > 50){
        return estimatedGameLength(info.remainingTime, info.ply, 40);
    }
    return estimatedGameLength(info.remainingTime, info.ply, 28);
}