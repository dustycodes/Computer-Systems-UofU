
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
    
    union FLOAT f, f1;
    f.s = 0;
    f.exp = 0xff;
    f.frac = 0;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);

    f.s = 1;
    f.exp = 0xff;
    f.frac = 0;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);

    f.s = 0;
    f.exp = 0xff;
    f.frac = 0x1;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f.f,f.s,f.exp,f.frac);

    f1.s = 0;
    f1.exp = 0xff;
    f1.frac = 0x2;
    printf("%f | 0x%x 0x%2.2x 0x%3.3x\n",f1.f,f1.s,f1.exp,f1.frac);
    
    printf("%f == %f ? %i\n",f.f,f1.f,f.f==f1.f);

    return 0;
}

