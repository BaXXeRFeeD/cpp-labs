#include <iostream>
#include <memory>

struct Memory {
    size_t *arr;
    size_t count = 0;
    size_t i = 0;

    template<typename Arg, typename... Args>
    void find(Arg arg, Args... args) {
        count++;
        find(args...);
    }

    template<typename Arg>
    void find(Arg arg) {
        count++;
    }

    template<typename Arg, typename... Args>
    void make_arr(Arg arg, Args... args) {
        arr[i] = arg;
        i++;
        make_arr(args...);
    }

    template<typename Arg>
    void make_arr(Arg arg) {
        arr[i] = arg;
    }

    template<typename... args>
    Memory(args... args) {
        find(args...);
        arr = new size_t[count];
        make_arr(args...);
    }

    ~Memory() {
        if (count > 0)
            delete[] arr;
        else
            delete arr;
    }
};

struct Chunk {
    void *data;
    char used = 0;
    Chunk *next;
};

struct Pool {
    void *data;
    Chunk *chunk_start;
    size_t chunk_count = 2;
    size_t pool_size;
};

template<typename T, size_t... pools>
class PoolAllocator {
public:

    typedef T value_type;
//     typedef T* pointer;
//     typedef const T* const_pointer;
//     typedef std::size_t size_type;
//     typedef std::ptrdiff_t difference_type;

    template<typename U>
    struct rebind {
        using other = PoolAllocator<U, pools...>;
    };

    Pool *getBlocks() const {
        return blocks;
    }

    template<typename U>
    PoolAllocator(const PoolAllocator<U, pools...> &alloc) {
        Memory mem = Memory(pools...);
        if (mem.count % 2 != 0)
            std::terminate();
        count = mem.count / 2;
        blocks = new Pool[count];
        for (size_t i = 0; i < count; i++)
            blocks[i] = alloc.getBlocks()[i];
    }

    PoolAllocator() {
        Memory mem = Memory(pools...);
        if (mem.count % 2 != 0)
            std::terminate();
        count = mem.count / 2;
        blocks = new Pool[mem.count / 2]();
        for (size_t i = 0; i < count; i++) {
            blocks[i].pool_size = mem.arr[2 * i];
            blocks[i].chunk_count = mem.arr[2 * i + 1];
            if (blocks[i].pool_size < blocks[i].chunk_count || blocks[i].pool_size <= 0 || blocks[i].chunk_count <= 0)
                std::terminate();
        }

        for (size_t i = 0; i < mem.count / 2; i++) {
            blocks[i].chunk_start = new Chunk;
            blocks[i].data = reinterpret_cast<void *>(malloc(blocks[i].pool_size));
            blocks[i].chunk_start->data = blocks[i].data;

            Chunk *chunk = blocks[i].chunk_start;

            for (int j = 0; j < blocks[i].chunk_count - 1; j++) {
                chunk->next = new Chunk;
                chunk->next->data =
                        reinterpret_cast<void *>(reinterpret_cast<char *>(chunk->data) +
                                                 blocks[i].pool_size / blocks[i].chunk_count);
                chunk = chunk->next;
            }

            chunk->next = nullptr;
        }
    }

    ~PoolAllocator() {
//        std::cout << "s";
//        for (size_t i = 0; i < count; i++) {
//            Chunk* chunk = blocks[i].chunk_start;
//            while(chunk != nullptr) {
//                Chunk* next = chunk->next;
//                delete chunk;
//                chunk = next;
//            }
//            free(blocks[i].data);
//        }
        delete[] blocks;
    }

    T *allocate(size_t count_t = 1);

    void deallocate(void *ptr, size_t size);

private:
    Pool *blocks;
    size_t count;
};

size_t ask_memory(Pool &pool, size_t memory) {
    size_t pool_ct = memory / (pool.pool_size / pool.chunk_count);
    Chunk *chunk = pool.chunk_start;
    bool flag;
    size_t seat = 0;
    for (; chunk != nullptr; chunk = chunk->next) {
        flag = true;
        Chunk *cur_chunk = chunk;
        for (size_t i = 0; i < pool_ct && flag && i < pool.chunk_count; i++) {
            if (cur_chunk->used) {
                flag = false;
            }
            cur_chunk = cur_chunk->next;
        }
        if (flag) {
            return seat + 1;
        }
        seat++;
    }
    return 0;
}

template<typename T, size_t... pools>
T *PoolAllocator<T, pools...>::allocate(size_t count_t = 1) {
    size_t memory_needed = sizeof(T) * count_t;
    size_t min_memory = 0;
    size_t min_place = 0;
    size_t ct = 0;

    for (size_t i = 0; i < count; i++) {
        size_t chunk_size = blocks[i].pool_size / blocks[i].chunk_count;
        size_t min = memory_needed / chunk_size;
        if (memory_needed % chunk_size != 0)
            min++;
        size_t place = ask_memory(blocks[i], min * chunk_size);
        if (min_memory == 0 && place != 0) {
            min_memory = min;
            min_place = place;
            ct = i;
        } else if (min < min_memory && place != 0) {
            min_memory = min;
            min_place = place;
            ct = i;
        }
    }

    if (min_memory == 0) {
        std::terminate();
    }

    Chunk *free_chunk = blocks[ct].chunk_start;
//    std::cout << min_place << "!\n";
    while (min_place > 1) {
        free_chunk = free_chunk->next;
        min_place--;
    }

    Chunk *current_chunk = free_chunk;
    while (min_memory > 0) {
        current_chunk->used = 1;
        current_chunk = current_chunk->next;
        min_memory--;
    }

    return reinterpret_cast<T *>(free_chunk->data);
}

template<typename T, size_t... pools>
void PoolAllocator<T, pools...>::deallocate(void *chunk, size_t size) {
    for (size_t i = 0; i < count; i++) {
        Chunk *chunk_search = blocks[i].chunk_start;
        bool flag = false;
        while (chunk_search != nullptr) {
            if (chunk_search->data == chunk) {
                flag = true;
                break;
            }
            chunk_search = chunk_search->next;
        }
        if (flag) {
            size_t chunk_size = blocks[i].pool_size / blocks[i].chunk_count;
            size_t min = (size * sizeof(T)) / (chunk_size);
            if ((size * sizeof(T)) % (chunk_size))
                min++;
            while (min > 0) {
                chunk_search->used = 0;
                min--;
                chunk_search = chunk_search->next;
            }
            break;
        }
    }
}

