#include <filter.h>

int main(){
    filter::low_pass a(1000, 5);
    printf("a1: %f, a2: %f, b0: %f, b1: %f, b2: %f\n", a.a1, a.a2, a.b0, a.b1, a.b2);
}