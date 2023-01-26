#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RBKey.h"

Key randKey(int max){
    static int call = 0;
    if(!call){
        srand(time(NULL));
        call++;
    }
    return rand()%max;
}