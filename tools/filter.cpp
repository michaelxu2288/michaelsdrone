/**
 * @file filter.cpp
 * @author andrew
 * @brief filter.cpp testing
 * @version 0.1
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <filter.h>
#include <cstdio>

int main(){
    filter::low_pass a(1000, 5);
    printf("a1: %f, a2: %f, b0: %f, b1: %f, b2: %f\n", a.a1, a.a2, a.b0, a.b1, a.b2);
}