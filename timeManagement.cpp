int estimatedTurnsLeft(int remainingTime, int a){
    return remainingTime / a;
}

int et_s(int remainingTime){
    return estimatedTurnsLeft(remainingTime, 15);
}

int et_p(int remainingTime){
    return estimatedTurnsLeft(remainingTime, 8);
}

int et_f(int remainingTime){
    return estimatedTurnsLeft(remainingTime, 25);
}