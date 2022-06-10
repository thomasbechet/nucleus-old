#include <nucleus/core/system/allocator/freelist.h>

#include <nucleus/core/system/allocator/memory.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>
#include <nucleus/core/utility/macro.h>

#define ALLOCATION_SIZE (1 << 14) // 32k
#define MEMBER_SIZE(type, member) sizeof(((type*)0)->member)

static inline size_t align(size_t n)
{
    return (n + sizeof(nu_word_t) - 1) & ~(sizeof(nu_word_t) - 1);
}
static inline nu_block_t *block_from_ptr(const nu_word_t *ptr)
{
    return (nu_block_t*)((size_t)ptr + MEMBER_SIZE(nu_block_t, data) - sizeof(nu_block_t));
}
static inline size_t block_total_size(size_t size)
{
    return size + sizeof(nu_block_t) - MEMBER_SIZE(nu_block_t, data);
}
static inline bool is_used(const nu_block_t *block)
{
    return block->flags & NU_BLOCK_FLAG_USED;
}
static inline bool is_allocated(const nu_block_t *block)
{
    return block->flags & NU_BLOCK_FLAG_ALLOCATED;
}
static inline bool block_can_split(const nu_block_t *block, size_t size)
{
    return block->size > block_total_size(size) + sizeof(nu_word_t);
}
static void check_integrity(nu_allocator_freelist_t *allocator)
{
    nu_block_t *block = allocator->first;
    nu_block_t *previous = NULL;
    while (block != NULL) {
        NU_ASSERT(block->prev == previous);
        if (previous) {
            NU_ASSERT(previous->next == block);
        }
        previous = block;
        block = block->next;
    }
}
static inline nu_block_t *block_split(nu_block_t *block, size_t size)
{
    NU_ASSERT(!is_used(block));
    nu_block_t *new_block = (nu_block_t*)((size_t)block->data + size);
    // Initialize new block
    new_block->flags = 0x0;
    new_block->next  = block->next;
    new_block->prev  = block;
    new_block->size  = (block->size - size - (sizeof(nu_block_t) - MEMBER_SIZE(nu_block_t, data)));
    // Update old block
    block->next = new_block;
    block->size = size;
    // Update next block of the new block
    if (new_block->next) {
        new_block->next->prev = new_block;
    }
    return block;
}
static inline nu_block_t *find_first_block(nu_block_t *block, size_t size)
{
    while (block != NULL) {
        if ((block->flags & NU_BLOCK_FLAG_USED) || size > block->size) {
            block = block->next;
            continue;
        }
        return block;
    }
    return NULL;
}
static nu_block_t *add_block(nu_allocator_freelist_t *allocator, size_t size, const char *file, uint32_t line)
{
    // Find the first block
    nu_block_t *block = find_first_block(allocator->first, size);

    // Not suitable block found
    if (!block) {
        // Create a large block
        const size_t allocated_size = ((size / ALLOCATION_SIZE) + 1) * ALLOCATION_SIZE;
        // Create a new big block
        block = (nu_block_t*)nu_allocator_memory_commit(block_total_size(allocated_size));
        block->size  = allocated_size;
        block->flags = NU_BLOCK_FLAG_ALLOCATED;
        block->next  = NULL;
        // Append the new block to the list
        if (allocator->last) {
            allocator->last->next = block;
            block->prev = allocator->last;
        } else {
            allocator->first = block;
            block->prev = NULL;
        }
        allocator->last = block;
    }
    
    NU_ASSERT(!is_used(block));

    // Try to split the block
    if (block_can_split(block, size)) {
        // Split the block
        block = block_split(block, size);
        NU_ASSERT(block->next->prev == block);
        NU_ASSERT(block->size == size);
        if (allocator->last == block) {
            allocator->last = block->next;
        }
    }

    check_integrity(allocator);

    // Configure the block
    block->flags |= NU_BLOCK_FLAG_USED;
#ifdef NU_DEBUG_MEMORY
    block->line = line;
    block->file = file;
#else
    (void)line;
    (void)file;
#endif

    return block;
}
static inline void merge_block(nu_block_t *left, nu_block_t *right, nu_allocator_freelist_t *allocator)
{
    NU_ASSERT(left->next == right);
    NU_ASSERT(right->prev == left);
    NU_ASSERT(!is_used(left));
    NU_ASSERT(!is_used(right));
    NU_ASSERT(!is_allocated(right));

    // Update last block pointer
    if (allocator->last == right) {
        allocator->last = left;
    }
    // Increase size
    left->size += block_total_size(right->size);
    // Reconnect blocks
    left->next = right->next;
    if (right->next) {
        right->next->prev = left;
    }
}
static inline void remove_block(nu_block_t *block, nu_allocator_freelist_t *allocator)
{
    NU_ASSERT(block->size > 0);
    NU_ASSERT(is_used(block));

    // Update status
    block->flags &= ~NU_BLOCK_FLAG_USED;

    // Merge with the next block
    if (block->next && !is_used(block->next) && !is_allocated(block->next)) {
        merge_block(block, block->next, allocator);
    }

    // Merge with the previous block
    if (block->prev && !is_used(block->prev) && !is_allocated(block)) {
        merge_block(block->prev, block, allocator);
    }
}

// +--------------------------------------------------------------------------+
// |                              PRIVATE API                                 |
// +--------------------------------------------------------------------------+

void nu_allocator_freelist_initialize(nu_allocator_freelist_t *allocator)
{
    allocator->first = NULL;
    allocator->last  = NULL;
}
void nu_allocator_freelist_terminate(nu_allocator_freelist_t *allocator)
{
    nu_block_t *block = allocator->first;
    while (block != NULL) {

        // Free the block if allocated
        if (block->flags & NU_BLOCK_FLAG_ALLOCATED) {
            void *p = block;
            block = block->next;
            nu_allocator_memory_uncommit(p);
        } else {
            block = block->next;
        }
    }
}
void *nu_allocator_freelist_malloc(nu_allocator_freelist_t *allocator, size_t size, const char *file, uint32_t line)
{
    // Align size
    size = align(size);

    // Add new block
    nu_block_t *block = add_block(allocator, size, file, line);

    // Return new block
    return block->data;
}
void *nu_allocator_freelist_realloc(nu_allocator_freelist_t *allocator, void *p, size_t s, const char *file, uint32_t line)
{
    // Find current block
    nu_block_t *block = block_from_ptr(p);
    NU_ASSERT(is_used(block));
    // Reallocation
    size_t old_size = block->size;
    if (s > old_size) {
        // TODO: smarter reallocation method (expend current block if possible)
        nu_block_t *new_block = add_block(allocator, s, file, line);
        memcpy(new_block->data, p, old_size);
        remove_block(block, allocator);
        return new_block->data;
    } else {
        return block->data;
    }
}
void nu_allocator_freelist_free(nu_allocator_freelist_t *allocator, void *p, const char *file, uint32_t line)
{
    (void)allocator;
    (void)file;
    (void)line;

    // Recover the allocated block
    nu_block_t *block = block_from_ptr(p);
    NU_ASSERT(is_used(block));

    // Remove the block
    remove_block(block, allocator);
}
void nu_allocator_freelist_profile(nu_allocator_freelist_t *allocator)
{
    nu_block_t *block = allocator->first;
    while (block != NULL) {
        printf("[%zu,%c,%c] ", block->size, 
            block->flags & NU_BLOCK_FLAG_USED ? 'u' : '-', 
            block->flags & NU_BLOCK_FLAG_ALLOCATED ? 'a' : '-');
        block = block->next;
    }
    printf("\n");
}
bool nu_allocator_freelist_check_memory_leak(nu_allocator_freelist_t *allocator)
{
    nu_block_t *block = allocator->first;
    bool leak = false;
    while (block != NULL) {

        // Check free
        #ifdef NU_DEBUG_MEMORY
        if (block->flags & NU_BLOCK_FLAG_USED) {
            nu_warning(nu_logger_get_core(), "Memory leak detected: (%s:%d).", block->file, block->line);
            leak = true;
        }
        #endif
        block = block->next;
    }
    return leak;
}