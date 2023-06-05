int estimatedTurnsLeft(int remainingTime, int a){
    return remainingTime / a;
}

int et_s(int remainingTime){
    return estimatedTurnsLeft(remainingTime, 15);
}