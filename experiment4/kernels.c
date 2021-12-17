/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include "defs.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
 * Please fill in the following team struct
 */
team_t team = {
    "xuqg", /* Team name */

    "xu qingguo",            /* First member full name */
    "xuqg@mail.ustc.edu.cn", /* First member email address */

    "", /* Second member full name (leave blank if none) */
    ""  /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

// void func(int dim, pixel *src, pixel *dst, int n, int k) {
//     printf("src addr = %lx, dst addr = %lx, dim = %d, n = %d, k = %d\n", src,
//     dst, dim, n, k); int i, j; for (j = dim/n*k; j < dim / n*(k+1) && j <
//     dim; j++)
//         for (i = 0; i < dim; i++)
//             dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
// }

#define NUM_THREADS 6
/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) {
    int i, j, k, l;
    int block_size = 32;
    // omp_set_num_threads(NUM_THREADS);
    // #pragma omp parallel for
    for (i = 0; i < dim; i += block_size) {
        // #pragma omp parallel for
        for (j = 0; j < dim; j += block_size) {
            int x_end = i+block_size;
            int y_end = j+block_size;
            // #pragma omp parallel for
            for (k = i; k < x_end; k++) {
                // #pragma omp simd
                for (l = j; l < y_end; l++) {
                    dst[RIDX(dim - 1 - k, l, dim)] = src[RIDX(l, k, dim)];
                }
            }
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions() {
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum) {
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p) {
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

static void accumulate_sum_without_num(pixel_sum *sum, pixel p) {
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    return;
}

static void accumulate_pixel_sum(pixel_sum *sum, pixel_sum p) {
    sum->red += p.red;
    sum->green += p.green;
    sum->blue += p.blue;
    sum->num += p.num;
    return;
}

static void accumulate_pixel_sum_without_num(pixel_sum *sum, pixel_sum p) {
    sum->red += p.red;
    sum->green += p.green;
    sum->blue += p.blue;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) {
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

static void assign_sum_to_pixel_default(pixel *current_pixel, pixel_sum sum) {
    current_pixel->red = (unsigned short)(sum.red / 9);
    current_pixel->green = (unsigned short)(sum.green / 9);
    current_pixel->blue = (unsigned short)(sum.blue / 9);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src) {
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

static pixel avg_mid(int dim, int i, int j, pixel *src) {
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);

    accumulate_sum(&sum, src[RIDX(i, j-1, dim)]);
    accumulate_sum(&sum, src[RIDX(i, j, dim)]);
    accumulate_sum(&sum, src[RIDX(i, j+1, dim)]);

    accumulate_sum(&sum, src[RIDX(i-1, j-1, dim)]);
    accumulate_sum(&sum, src[RIDX(i-1, j, dim)]);
    accumulate_sum(&sum, src[RIDX(i-1, j+1, dim)]);

    accumulate_sum(&sum, src[RIDX(i+1, j-1, dim)]);
    accumulate_sum(&sum, src[RIDX(i+1, j, dim)]);
    accumulate_sum(&sum, src[RIDX(i+1, j+1, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

static pixel_sum avg3col(int dim, int i, int j, pixel *src) {
    pixel_sum sum;

    initialize_pixel_sum(&sum);

    accumulate_sum_without_num(&sum, src[RIDX(i, j-1, dim)]);
    accumulate_sum_without_num(&sum, src[RIDX(i, j, dim)]);
    accumulate_sum_without_num(&sum, src[RIDX(i, j+1, dim)]);

    // assign_sum_to_pixel(&current_pixel, sum);
    return sum;
}

static pixel avg3row(int dim, int i, int j, pixel_sum *src) {
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);

    accumulate_pixel_sum_without_num(&sum, src[RIDX(i-1, j, dim)]);
    accumulate_pixel_sum_without_num(&sum, src[RIDX(i, j, dim)]);
    accumulate_pixel_sum_without_num(&sum, src[RIDX(i+1, j, dim)]);

    assign_sum_to_pixel_default(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) { 
    int i, j;

    // 四个角
    dst[RIDX(0, 0, dim)] = avg(dim, 0, 0, src);
    dst[RIDX(dim-1, 0, dim)] = avg(dim, dim-1, 0, src);
    dst[RIDX(0, dim-1, dim)] = avg(dim, 0, dim-1, src);
    dst[RIDX(dim-1, dim-1, dim)] = avg(dim, dim-1, dim-1, src);

    // 四条边
    for (i = 1; i < dim-1; i++) {
        dst[RIDX(0, i, dim)] = avg(dim, 0, i, src);
        dst[RIDX(i, 0, dim)] = avg(dim, i, 0, src);
        dst[RIDX(dim-1, i, dim)] = avg(dim, dim-1, i, src);
        dst[RIDX(i, dim-1, dim)] = avg(dim, i, dim-1, src);
    }

    pixel_sum* temp = malloc(dim*dim*sizeof(pixel_sum));
    // 避免重复计算
    for (i = 0; i < dim; i++) {
        for (j = 1; j < dim-1; j++) {
            temp[RIDX(i, j, dim)] = avg3col(dim, i, j, src);
        }
    }

    // 矩阵卷积
    for (i = 1; i < dim-1; i++)
        for (j = 1; j < dim-1; j++)
            dst[RIDX(i, j, dim)] = avg3row(dim, i, j, temp);
}

void smooth_temp(int dim, pixel *src, pixel *dst) { 
    int i, j;

    // 四个角
    dst[RIDX(0, 0, dim)] = avg(dim, 0, 0, src);
    dst[RIDX(dim-1, 0, dim)] = avg(dim, dim-1, 0, src);
    dst[RIDX(0, dim-1, dim)] = avg(dim, 0, dim-1, src);
    dst[RIDX(dim-1, dim-1, dim)] = avg(dim, dim-1, dim-1, src);

    // 四条边
    for (i = 1; i < dim-1; i++) {
        dst[RIDX(0, i, dim)] = avg(dim, 0, i, src);
        dst[RIDX(i, 0, dim)] = avg(dim, i, 0, src);
        dst[RIDX(dim-1, i, dim)] = avg(dim, dim-1, i, src);
        dst[RIDX(i, dim-1, dim)] = avg(dim, i, dim-1, src);
    }

    // 矩阵卷积
    for (i = 1; i < dim-1; i++)
        for (j = 1; j < dim-1; j++)
            dst[RIDX(i, j, dim)] = avg_mid(dim, i, j, src);
}

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}
