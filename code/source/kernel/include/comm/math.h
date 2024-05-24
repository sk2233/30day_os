#ifndef MATH_H
#define MATH_H

static inline int clump(int val,int max,int min) {
    if(val<min){
        return min;
    }
    if(val>max){
        return max;
    }
    return val;
}

static inline int abs(int val) {
    if(val<0){
        return -val;
    }
    return val;
}

static inline int max(int val1,int val2) {
    if(val1>val2){
        return val1;
    }
    return val2;
}

static inline int min(int val1,int val2) {
    if(val1<val2){
        return val1;
    }
    return val2;
}

#endif