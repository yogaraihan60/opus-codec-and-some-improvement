#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <vector>
#include <queue>
#include <mutex>
#include <memory>

template <typename T>
class memory_pool {
public:
    memory_pool(size_t chunk_size, size_t initial_pool_size = 10) 
        : _chunk_size(chunk_size) {
        for (size_t i = 0; i < initial_pool_size; ++i) {
            _pool.push(std::make_shared<std::vector<T>>(_chunk_size));
        }
    }

    std::shared_ptr<std::vector<T>> acquire() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_pool.empty()) {
            return std::make_shared<std::vector<T>>(_chunk_size);
        }
        auto ptr = _pool.front();
        _pool.pop();
        return ptr;
    }

    void release(std::shared_ptr<std::vector<T>> ptr) {
        // Only return if it matches our chunk size
        if (ptr->capacity() >= _chunk_size) {
            std::lock_guard<std::mutex> lock(_mutex);
            ptr->clear(); // Reuse the vector
            ptr->resize(_chunk_size); // Ensure size is reset if it was changed
            _pool.push(ptr);
        }
    }

private:
    size_t _chunk_size;
    std::queue<std::shared_ptr<std::vector<T>>> _pool;
    std::mutex _mutex;
};

#endif // MEMORY_POOL_HPP
