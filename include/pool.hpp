#pragma once
#include <cstddef>
// HW4 — a high-performance fixed-size object pool (O(1) alloc/free, placement new).

#pragma once
#include <cstddef>
#include <cstdint>
#include <new>       // placement new

struct Pool {
    Pool(std::size_t obj_size, std::size_t capacity)
        : slot_(obj_size < sizeof(void*) ? sizeof(void*) : obj_size),
          cap_(capacity) {
        buf_  = static_cast<uint8_t*>(::operator new(slot_ * cap_));
        head_ = nullptr;
        // push every slot onto the free-list, back to front
        for (std::size_t i = cap_; i-- > 0; )
            free(buf_ + i * slot_);
    }
    ~Pool() { ::operator delete(buf_); }
    void* alloc() {
        if (!head_) return nullptr;          // exhausted → null (the test relies on this)
        void* p = head_;
        head_ = *reinterpret_cast<void**>(head_);   // head = head->next
        return p;
    }
    void free(void* p) {
        if (!p) return;
        *reinterpret_cast<void**>(p) = head_;   // p->next = head
        head_ = p;                              // head = p
    }
private:
    std::size_t slot_, cap_;
    uint8_t*    buf_;
    void*       head_;   // top of the free-list
};
// struct Pool {
//     Pool(std::size_t obj_size, std::size_t capacity) { (void)obj_size; (void)capacity;
//         // TODO(student): back this with ONE pre-allocated buffer + a free-list.
//     }
//     void* alloc() { return nullptr;               // TODO(student): pop a free slot, O(1)
//     }
//     void  free(void* p) { (void)p;                // TODO(student): return the slot, O(1)
//     }
// };
