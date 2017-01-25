
#include <stdio.h>

union FLOAT {
    float f;
    struct {
        unsigned frac:23;
        unsigned exp:8;
        unsigned s:1;
    };
};


int main(){
    
    union FLOAT f;
    f.f = 0.0f;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);
    f.f = 0.5f;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);
    f.f = 1.0f;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);
    f.f = 2.0f;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);
    return 0;
}

