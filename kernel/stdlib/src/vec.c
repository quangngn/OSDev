#include "vec.h"

ivec2_t ivec2_add(ivec2_t v1, ivec2_t v2){
    ivec2_t res = {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };

    return res;
}

ivec2_t ivec2_sub(ivec2_t v1, ivec2_t v2){
    ivec2_t res = {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y
    };

    return res;
}

ivec2_t ivec2_mult(ivec2_t v1, ivec2_t v2){
    ivec2_t res = {
        .x = v1.x * v2.x,
        .y = v1.y * v2.y
    };

    return res;
}

ivec2_t ivec2_scale(ivec2_t v1, int scale){
    ivec2_t res = {
        .x = v1.x * scale,
        .y = v1.y * scale
    };

    return res;
}

// Not sure if this code is correct. Dot product should return an integer (?)
ivec2_t ivec2_dot_prod(ivec2_t v1, ivec2_t v2){
    ivec2_t res = {
        .x = v1.x * v2.x + v1.y * v2.y,
        .y = 0
    };

    return res;
}

// We do not have sqrt() just yet. Should we write one?
// // Square of the vector's magnitude
// int ivec2_mag_sq(ivec2_t v1);

// Check if two vectors are equal (x1 == x2 && y1 == y2)
bool ivec2_equal(ivec2_t v1, ivec2_t v2){
    if (v1.x == v2.x && v1.y == v2.y){
        return true;
    }
    else{
        return false;
    }
}

fvec2_t fvec2_add(fvec2_t v1, fvec2_t v2){
    fvec2_t res = {
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };

    return res;
}

fvec2_t fvec2_sub(fvec2_t v1, fvec2_t v2){
    fvec2_t res = {
        .x = v1.x - v2.x,
        .y = v1.y - v2.y
    };

    return res;
}

fvec2_t fvec2_ew_mult(fvec2_t v1, fvec2_t v2){
    fvec2_t res = {
        .x = v1.x * v2.x,
        .y = v1.y * v2.y
    };

    return res;
}

fvec2_t fvec2_scale(fvec2_t v1, float scale){
    fvec2_t res = {
        .x = v1.x * scale,
        .y = v1.y * scale
    };

    return res;
}

fvec2_t fvec2_dot_prod(fvec2_t v1, fvec2_t v2){
    fvec2_t res = {
        .x = v1.x * v2.x + v1.y + v2.y,
        .y = 0
    };

    return res;
}

// We do not have a sqrt function.
// int fvec2_mag_sq(fvec2_t v1);

bool fvec2_equal(fvec2_t v1, fvec2_t v2){
    if (v1.x == v2.x && v1.y == v2.y){
        return true;
    }
    else
        return false;
}

imat2x2_t imat2x2_add(imat2x2_t m1, imat2x2_t m2){
    ivec2_t r1 = {
        .x = m1.c1.x + m2.c1.x,
        .y = m1.c1.y + m2.c1.y
    };

    ivec2_t r2 = {
        .x = m1.c2.x + m2.c2.x,
        .y = m1.c2.y + m2.c2.y
    };

    imat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

imat2x2_t imat2x2_sub(imat2x2_t m1, imat2x2_t m2){
    ivec2_t r1 = {
        .x = m1.c1.x - m2.c1.x,
        .y = m1.c1.y - m2.c1.y
    };

    ivec2_t r2 = {
        .x = m1.c2.x - m2.c2.x,
        .y = m1.c2.y - m2.c2.y
    };

    imat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

imat2x2_t imat2x2_scale(imat2x2_t m1, int scale){
    ivec2_t r1 = {
        .x = m1.c1.x * scale,
        .y = m1.c1.y * scale
    };

    ivec2_t r2 = {
        .x = m1.c2.x * scale,
        .y = m1.c2.y * scale
    };

    imat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}
imat2x2_t imat2x2_matmul(imat2x2_t m1, imat2x2_t m2){
    ivec2_t r1 = {
        .x = m1.c1.x * m2.c1.x + m1.c2.x * m2.c1.y,
        .y = m1.c1.y * m2.c1.x + m1.c2.y * m2.c1.y
    };

    ivec2_t r2 = {
        .x = m1.c1.x * m2.c2.x + m1.c2.x * m2.c2.y,
        .y = m1.c1.y * m2.c2.x + m1.c2.y * m2.c2.y
    };

    imat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

bool imat2x2_equal(imat2x2_t m1, imat2x2_t m2){
    if (ivec2_equal(m1.c1, m2.c1) && ivec2_equal(m1.c2, m2.c2)){
        return true;
    }
    else{
        return false;
    }
}

fmat2x2_t fmat2x2_add(fmat2x2_t m1, fmat2x2_t m2){
    fvec2_t r1 = {
        .x = m1.c1.x + m2.c1.x,
        .y = m1.c1.y + m2.c1.y
    };

    fvec2_t r2 = {
        .x = m1.c2.x + m2.c2.x,
        .y = m1.c2.y + m2.c2.y
    };

    fmat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

fmat2x2_t fmat2x2_sub(fmat2x2_t m1, fmat2x2_t m2){
    fvec2_t r1 = {
        .x = m1.c1.x - m2.c1.x,
        .y = m1.c1.y - m2.c1.y
    };

    fvec2_t r2 = {
        .x = m1.c2.x - m2.c2.x,
        .y = m1.c2.y - m2.c2.y
    };

    fmat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

fmat2x2_t fmat2x2_scale(fmat2x2_t m1, int scale){
    fvec2_t r1 = {
        .x = m1.c1.x * scale,
        .y = m1.c1.y * scale
    };

    fvec2_t r2 = {
        .x = m1.c2.x * scale,
        .y = m1.c2.y * scale
    };

    fmat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

fmat2x2_t fmat2x2_matmul(fmat2x2_t m1, fmat2x2_t m2){
    fvec2_t r1 = {
        .x = m1.c1.x * m2.c1.x + m1.c2.x * m2.c1.y,
        .y = m1.c1.y * m2.c1.x + m1.c2.y * m2.c1.y
    };

    fvec2_t r2 = {
        .x = m1.c1.x * m2.c2.x + m1.c2.x * m2.c2.y,
        .y = m1.c1.y * m2.c2.x + m1.c2.y * m2.c2.y
    };

    fmat2x2_t res = {
        .c1 = r1,
        .c2 = r2
    };

    return res;
}

bool fmat2x2_equal(fmat2x2_t m1, fmat2x2_t m2){
    if (fvec2_equal(m1.c1, m2.c1) && fvec2_equal(m1.c2, m2.c2)){
        return true;
    }
    else{
        return false;
    }
}