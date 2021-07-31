#ifndef HLSTL_VECTOR_H
#define HLSTL_VECTOR_H

#include "hlstl_allocator.h"

namespace hl
{

template <typename T, typename Alloc = Allocator<T>>
class vector
{
protected:
    class VectorBase
    {
    public:
        using allocator_type = Alloc;
        allocator_type GetAllocator() const { return allocator_type(); }

        ~VectorBase()
        {
            this->DeAllocate(start_, end_of_storage_ - start_);
        }

        VectorBase() : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr)
        {
        }

        VectorBase(size_t n)
        {
            start_ = this->Allocate(n);
            finish_ = start_;
            end_of_storage_ = start_ + n;
        }

        VectorBase(const Alloc&) : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr)
        {
        }

        // VectorBase(size_t n, const Alloc&)
        // {
        //     //start_ =
        // }

        // friend class vector;
        // protected:

        T* start_;
        T* finish_;
        T* end_of_storage_;
        using data_allocator = Alloc;
        T* Allocate(size_t n)
        {
            return data_allocator::Allocate(n);
        }

        void DeAllocate(T* p, size_t n)
        {
            data_allocator::DeAllocate(p, n);
        }
    };

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using iterator = value_type*;
    using const_iterator = const value_type*;

    using allocator_type = typename VectorBase::allocator_type;
    allocator_type GetAllocator() const { return allocator_type::GetAllocator(); }
    //Vector(const allocator_type& alloc = GetAllocator()) : base_(alloc) {}
    vector() {}

    vector(size_type n)
    {
       // base_.finish_ = uninitialized_fill_n(base_.start_, n, size_type());
    }

    vector(size_type n, const_reference value) {}

    iterator begin() { return base_.start_; }
    const_iterator cbegin() { return base_.start_; }
    iterator end() { return base_.finish_; }
    const_iterator cend() { return base_.finish_; }

    // TODO rbegin ...

    size_type size() { return base_.finish_ - base_.start_; }
    size_type max_size() { return size_type(-1) / sizeof(value_type); }
    size_type capacity() { return base_.end_of_storage_ - base_.start_; }

protected:
    //template <typename T, typename Alloc>

    VectorBase base_;
};

} // namespace hl

#endif
