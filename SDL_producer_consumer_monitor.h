
typedef struct
{
    void (*append) (void *, void *);
    void *(*take) (void *);
         bool(*buf_is_empty) (void *);
         bool(*buf_is_full) (void *);
} SDL_ProdConsMon_cb_t;

void* SDL_ProdConsMon_init(void* buf, SDL_ProdConsMon_cb_t* cb);

void SDL_ProdConsMon_append(void* monitor, void* item);

void* SDL_ProdConsMon_take(void* monitor);

