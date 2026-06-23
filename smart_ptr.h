//This implementation doesn't ensure uniqueness 
//smart int* t=&m
//smart int* t1=t;
//will cause undefined behaviour
#pragma once 
#include<stdlib.h>
#include<assert.h>
static inline void cleaner(void* ptrs){  
    free(*(void **)ptrs); 
}
static void move(void** dst,void** src){
    assert(t1!=t2); 
    *dst = *src;
    *src = NULL;
}
#define smart __attribute__((cleanup(cleaner)))

