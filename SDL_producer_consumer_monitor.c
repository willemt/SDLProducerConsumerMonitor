
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

/*----------------------------------------------------------------------------*/

/*
 * producer/consumer monitor.
 *
 */

typedef struct {
    SDL_ProdConsMon_cb_t* cb;
    void* buf;
    void* cond_not_empty;
    void* cond_not_full;
    void* mutex;
} SDL_ProdConsMon_t;

void* SDL_ProdConsMon_init(void* buf, SDL_ProdConsMon_cb_t* cb)
{
    SDL_ProdConsMon_t* mon;

    mon = calloc(1,sizeof(SDL_ProdConsMon_t));
    mon->mutex = SDL_CreateMutex();
    mon->cond_not_empty = SDL_CreateCond();
    mon->cond_not_full = SDL_CreateCond();
    mon->buf = buf;
    mon->cb = cb;
    return mon;
}

/**
 * Produce an item. */
void SDL_ProdConsMon_append(void* monitor, void* item)
{
    SDL_ProdConsMon_t* mon;
    
    mon = monitor;

    if (-1 == SDL_mutexP(mon->mutex))
        assert(FALSE);

    if (mon->cb->buf_is_full(mon->buf))
        SDL_CondWait(mon->cond_not_full, mon->mutex);

    mon->cb->append(mon->buf, item);
    SDL_CondSignal(mon->cond_not_empty);

    if (-1 == SDL_mutexV(mon->mutex))
        assert(FALSE);
}

/**
 * Consume an item */
void* SDL_ProdConsMon_take(void* monitor)
{
    SDL_ProdConsMon_t* mon;
    void* item;

    mon = monitor;

    if (-1 == SDL_mutexP(mon->mutex))
        assert(FALSE);

    if (mon->cb->buf_is_empty(mon->buf))
        SDL_CondWait(mon->cond_not_empty, mon->mutex);

    item = mon->cb->take(mon->buf);
    SDL_CondSignal(mon->cond_not_full);

    if (-1 == SDL_mutexV(mon->mutex))
        assert(FALSE);

    return item;
}

