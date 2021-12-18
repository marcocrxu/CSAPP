/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include "mm.h"
#include "config.h"
#include "memlib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "XU",
    /* First member's full name */
    "xuqg",
    /* First member's email address */
    "xuqg@mail.ustc.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define BLK_SIZE 8

#define FREE 0
#define ALLOCATED 1

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

void logging(char* s) {
    // printf(s);
}

/**
 * @brief 写header
 *
 * @param addr
 * @param type
 * @param size
 */
void write_header(void *addr, int type, int size) {
    memset(addr, 0, BLK_SIZE);
    int *s_int = addr;
    *s_int++ = type;
    *s_int = size;
    // printf("write_header at %p, context is %d and %x, blc_index = %ld\n", addr, *(int*)addr, *(int*)(addr+4), (addr-mem_heap_lo())/BLK_SIZE);
}

/**
 * @brief 解析一个header，返回类型和size信息
 *
 * @param addr header起始的地址，由调用方确认地址是否合理
 * @return int
 * 如果是空闲的块，返回一个正数，数值是大小；如果返回一个负数，代表一个占用过的块，数值是大小。大小单位是字节。
 */
int analyze_header(void *addr) {
    int size, type;
    type = *(int *)(addr);
    size = *(int *)(addr + 4);
    return type == FREE ? size : -size;
}

void rm_header(void* addr) {
    memset(addr, 0, BLK_SIZE);
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    logging("start mm init\n");
    void* addr_lo = mem_heap_lo();
    // printf("addr_lo = %p\n", addr_lo);
    write_header(addr_lo, FREE, MAX_HEAP);
    // printf("addr_lo = %p\n", addr_lo + MAX_HEAP - BLK_SIZE);
    write_header(addr_lo + MAX_HEAP - BLK_SIZE, FREE, MAX_HEAP);
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    // 分配一个内存还要维护空闲区块的链表
    // 从最低处开始遍历，开始寻找第一个空闲的足够分配需要内存的空闲块,
    // 如果没有地方分配就返回NULL
    // printf("alloc memory with required size = %lu\n", size);
    void *addr = mem_heap_lo();
    void *heap_end = addr + MAX_HEAP;
    // 计算需要的块的数量，加2是因为开头和结尾需要两个标识
    int blk_cnt = (size+BLK_SIZE-1)/BLK_SIZE + 2;
    int need_size = blk_cnt*BLK_SIZE;
    // printf("blk_cnt = %d, need_size = %d\n", blk_cnt, need_size);
    while (addr < heap_end) {
        int cur_size = analyze_header(addr);
        int type = cur_size > 0 ? FREE : ALLOCATED;
        cur_size = abs(cur_size);
        // // printf("type = %d, cur_size = %d\n", type, cur_size);
        if (type == FREE && cur_size > need_size) {
            // printf("find a memory block to allocate!\n");
            // 是空闲的块则判断是否可以满足要求
            // 需不需要写下面部分的header，如果直接分配内存之后，剩下的内存数量只有一个BLOCK，这个时候写header和tailer都没地方了，不如该给你的都直接给你算了
            int allocate_size = cur_size - need_size >= 2*BLK_SIZE ? need_size : cur_size;
            
            write_header(addr, ALLOCATED, allocate_size);
            write_header(addr + allocate_size-BLK_SIZE, ALLOCATED, allocate_size);
    
            if (allocate_size < cur_size) {
                write_header(addr + allocate_size, FREE, cur_size-allocate_size);
                write_header(addr + cur_size-BLK_SIZE, FREE, cur_size-allocate_size);
            }
            
            if (addr+allocate_size > mem_heap_hi()) {
                mem_sbrk(addr+allocate_size-mem_heap_hi());
            }
            // printf("allocate memory index = %ld\n", (addr+BLK_SIZE-mem_heap_lo())/BLK_SIZE);
            return addr+BLK_SIZE;
        } else {
            // 如果是占用的块或者大小不够，则继续寻找下一个地址
            addr += cur_size;
        }
    }
    // printf("allocate memory failed!\n");
    return NULL;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
    // printf("free memory addr = %p\n", ptr);
    // free 函数主要设置一个内存的空闲区域
    int cur_size = abs(analyze_header(ptr-BLK_SIZE));
    int pre_size = 0; int next_size = 0;
    if (ptr > mem_heap_lo()) {
        // 如果前面没有，千万不要访问到前面的内存
        pre_size = analyze_header(ptr-2*BLK_SIZE);
    }
    
    if (ptr+cur_size < mem_heap_lo()+MAX_HEAP) {
        next_size = analyze_header(ptr+cur_size-BLK_SIZE);
    }

    if (pre_size > 0 && next_size > 0) {
        // 前后两个都是空闲的
        write_header(ptr-pre_size-BLK_SIZE, FREE, cur_size+pre_size+next_size);
        write_header(ptr+cur_size+next_size, FREE, cur_size+pre_size+next_size);

    } else if (pre_size > 0 && next_size < 0) {
        // 前面是空闲的，后面不是
        write_header(ptr-pre_size-BLK_SIZE, FREE, cur_size+pre_size);
        write_header(ptr+cur_size, FREE, cur_size+pre_size);
    } else if (pre_size < 0 && next_size > 0) {
        // 后面是空闲的，前面的不是
        write_header(ptr-BLK_SIZE, FREE, cur_size+next_size);
        write_header(ptr+cur_size+next_size, FREE, cur_size+next_size);
    } else {
        // 前后都不是空闲的
        write_header(ptr-BLK_SIZE, FREE, cur_size);
        write_header(ptr+cur_size, FREE, cur_size);
    }
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
        copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
