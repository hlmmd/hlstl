#ifndef HLSTL_VECTOR_H
#define HLSTL_VECTOR_H

#include "hlstl_alloc.h"
#include "hlstl_allocator.h"
#include "hlstl_iterator.h"

namespace hl
{

template <typename T, typename Alloc>
class VectorBase
{
public:
    using allocator_type = Alloc;
    allocator_type get_allocator() const { return allocator_type(); }

    VectorBase(const Alloc&) : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr)
    {
    }

    VectorBase(size_t n, const Alloc&)
    {
        start_ = this->allocate(n);
        finish_ = start_;
        end_of_storage_ = start_ + n;
    }

    ~VectorBase()
    {
        this->deallocate(start_, end_of_storage_ - start_);
    }

protected:
    T* start_;
    T* finish_;
    T* end_of_storage_;
    using data_allocator_ = simple_alloc<T, Alloc>;
    T* allocate(size_t n)
    {
        return data_allocator_::allocate(n);
    }

    void deallocate(T* p, size_t n)
    {
        data_allocator_::deallocate(p, n);
    }
};

template <typename T, typename Alloc = Allocator<T>>
class vector : protected VectorBase<T, Alloc>
{
protected:
    using BaseType = VectorBase<T, Alloc>;

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

    using const_reverse_iter = reverse_iterator<const_iterator>;
    using reverse_iter = reverse_iterator<iterator>;
    using allocator_type = typename BaseType::allocator_type;
    allocator_type get_allocator() const { return allocator_type::get_allocator(); }

    using BaseType::allocate;
    using BaseType::deallocate;
    using BaseType::end_of_storage_;
    using BaseType::finish_;
    using BaseType::start_;
    vector(const allocator_type& alloc = allocator_type()) : BaseType(alloc) {}

    vector(size_type n) : BaseType(n, allocator_type())
    {
        finish_ = uninitialized_fill_n(start_, n, value_type());
    }

    vector(size_type n, const_reference value,
           const allocator_type& alloc = allocator_type()) : BaseType(n, alloc)
    {
        finish_ = uninitialized_fill_n(start_, n, value);
    }

    iterator begin() { return start_; }
    const_iterator cbegin() const { return begin(); }
    iterator end() { return finish_; }
    const_iterator cend() const { return end(); }

    reference operator[](size_t n) { return *(begin() + n); }
    const_reference operator[](size_t n) const { return *(begin() + n); }

    // TODO rbegin ...

    size_type size() { return finish_ - start_; }
    size_type max_size() { return size_type(-1) / sizeof(value_type); }
    size_type capacity() { return end_of_storage_ - start_; }

protected:
};

} // namespace hl

#endif

