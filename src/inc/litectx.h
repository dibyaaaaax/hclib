#ifndef _LITECTX_H_
#define _LITECTX_H_

#include "hclib_common.h"
#include <stdlib.h>
#include "fcontext.h"
#include <string.h>

#define LITECTX_ALLOC(bytes) malloc(bytes)
#define LITECTX_FREE(ptr) free(ptr)
// #define LITECTX_FREE(ptr)
#define LITECTX_SIZE 0x10000 /* 64KiB */

typedef struct LiteCtxStruct {
    struct LiteCtxStruct *volatile prev;
    void *volatile arg;
    fcontext_t _fctx;
    char _stack[];
} LiteCtx;

#define LITECTX_STACK_SIZE ((LITECTX_SIZE) - sizeof(LiteCtx))

static __inline__ LiteCtx *LiteCtx_create(void (*fn)(LiteCtx*)) {
    LiteCtx *ctx = (LiteCtx *)LITECTX_ALLOC(LITECTX_SIZE);
    char *const stack_top = ctx->_stack + LITECTX_STACK_SIZE;
    ctx->prev = NULL;
    ctx->arg = NULL;
    ctx->_fctx = make_fcontext(stack_top, LITECTX_STACK_SIZE,
            (void (*)(void *))fn);
#ifdef VERBOSE
    fprintf(stderr, "LiteCtx_create: %p\n", ctx);
#endif
    return ctx;
}

static __inline__ void LiteCtx_destroy(LiteCtx *ctx) {
#ifdef VERBOSE
    fprintf(stderr, "LiteCtx_destroy: ctx=%p\n", ctx);
#endif
    LITECTX_FREE(ctx);
}

static __inline__ LiteCtx *LiteCtx_proxy_create(const char *lbl) {
    LiteCtx *ctx = (LiteCtx *)LITECTX_ALLOC(sizeof(*ctx));
    memset(ctx, 0, sizeof(*ctx));
#ifdef VERBOSE
    fprintf(stderr, "LiteCtx_proxy_create[%s]: %p\n", lbl, ctx);
#endif
    return ctx;
}

static __inline__ void LiteCtx_proxy_destroy(LiteCtx *ctx) {
#ifdef VERBOSE
    fprintf(stderr, "LiteCtx_proxy_destroy: ctx=%p\n", ctx);
#endif
    LITECTX_FREE(ctx);
}

#endif /* _LITECTX_H_ */
