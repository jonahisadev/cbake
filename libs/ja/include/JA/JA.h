#pragma once

#include "JA/pointer.h"
#include "JA/alloc.h"
#include "JA/log.h"

#ifndef JA_SYSTEM_ALLOC
    #ifndef JA_CUSTOM_ALLOC
        #define JA_CUSTOM_ALLOC 1024 * 1024 * 16
    #endif
    extern RefPtr<Allocator> ja_mem;
    #define ALLOC(T, x) ja_mem->alloc_size<T>((x))
    #define FREE(T, ptr) ja_mem->release<T>(ptr)
#else
    #define ALLOC(T, x) (T*) malloc(sizeof(T) * x)
    #define FREE(T, ptr) free(ptr)
#endif

#ifdef JA_IMPL
RefPtr<Allocator> ja_mem = make_ref_ptr<Allocator>(JA_CUSTOM_ALLOC);
#endif