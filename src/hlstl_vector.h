#ifndef HLSTL_VECTOR_H
#define HLSTL_VECTOR_H

#include "hlstl_alloc.h"
#include "hlstl_allocator.h"
#include "hlstl_iterator.h"
#include "hlstl_type_traits.h"
#include "hlstl_uninitialized.h"
#include <initializer_list>

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
    // 返回父类的 get_allocator
    allocator_type get_allocator() const { return BaseType::get_allocator(); }

    using BaseType::allocate;
    using BaseType::deallocate;
    using BaseType::end_of_storage_;
    using BaseType::finish_;
    using BaseType::start_;

    // 构造函数
    vector(const allocator_type& alloc = allocator_type()) : BaseType(alloc) {}

    vector(size_type n) : BaseType(n, allocator_type())
    {
        finish_ = uninitialized_fill_n(start_, n, (pointer) nullptr);
    }

    vector(size_type n, const T& value,
           const allocator_type& alloc = allocator_type()) : BaseType(n, alloc)
    {
        finish_ = uninitialized_fill_n(start_, n, value);
    }

    // 拷贝构造
    vector(const vector<T, Alloc>& another) : BaseType(another.size(), another.get_allocator())
    {
        finish_ = uninitialized_copy(another.begin(), another.end(), start_);
    }

    // 移动构造
    vector(vector<T, Alloc>&& another) : BaseType(another.get_allocator())
    {
        if (this == &another)
            return;
        start_ = another.start_;
        finish_ = another.finish_;
        end_of_storage_ = another.end_of_storage_;
        another.start_ = another.finish_ = another.end_of_storage_ = nullptr;
    }

    // initializer_list 构造
    vector(std::initializer_list<value_type> l, const allocator_type& alloc = allocator_type()) : BaseType(alloc)
    {
        range_initialize(l.begin(), l.end(), random_access_iterator_tag());
    }

    ~vector() { destroy(start_, finish_); }

    // 由迭代器[first,last)构造
    // 这里要判断 InputIterator 是否是一个整数， 与vector(size, value)做区分。
    template <typename InputIterator>
    vector(InputIterator first, InputIterator last,
           const allocator_type& alloc = allocator_type()) : BaseType(alloc)
    {
        using is_interger = typename __is_integer<InputIterator>::is_integer;
        initialize_with_iterator(first, last, is_interger());
    }

protected:
    template <typename Integer>
    void initialize_with_iterator(Integer n, Integer value, __true_type)
    {
        start_ = allocate(n);
        finish_ = uninitialized_fill_n(start_, n, value);
        end_of_storage_ = start_ + n;
    }

    template <typename InputIterator>
    void initialize_with_iterator(InputIterator first, InputIterator last, __false_type)
    {
        range_initialize(first, last, __ITERATOR_CATEGORY(first));
    }

    template <typename InputIterator>
    void range_initialize(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            push_back(*first);
    }

    template <typename ForwardIter>
    void range_initialize(ForwardIter first, ForwardIter last, forward_iterator_tag)
    {
        size_type n = distance(first, last);
        start_ = allocate(n);
        finish_ = uninitialized_copy(first, last, start_);
        end_of_storage_ = start_ + n;
    }

public:
    iterator begin() { return start_; }
    iterator begin() const { return start_; }
    const_iterator cbegin() const { return begin(); }
    iterator end() { return finish_; }
    iterator end() const { return finish_; }
    const_iterator cend() const { return end(); }

    reverse_iter rbegin() { return reverse_iter(end()); }
    reverse_iter rbegin() const { return reverse_iter(end()); }
    const_reverse_iter crbegin() const { return rbegin(); }
    reverse_iter rend() { return reverse_iter(begin()); }
    reverse_iter rend() const { return reverse_iter(begin()); }
    const_reverse_iter crend() const { return rend(); }

    size_type size() const { return finish_ - start_; }
    size_type max_size() const { return size_type(-1) / sizeof(value_type); }
    size_type capacity() const { return end_of_storage_ - start_; }

    reference operator[](size_t n) { return *(begin() + n); }
    const_reference operator[](size_t n) const { return *(begin() + n); }

    // TODO 异常处理
    reference at(size_t n) { return (*this)[n]; }
    const_reference at(size_t n) const { return (*this)[n]; }

    reference front() { return *begin(); }
    const_reference front() const { return front(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return back(); }

    vector<T, Alloc>& operator=(std::initializer_list<value_type> l)
    {
        assign_aux(l.begin(), l.end(), random_access_iterator_tag());
        return *this;
    }

    vector<T, Alloc>& operator=(const vector<T, Alloc>& another)
    {
        if (&another == this)
            return *this;
        if (another.size() > this->capacity())
        {
            // 当新 vector 的 size>capacity，需要重新申请空间。先释放原空间
            destroy(start_, finish_);
            deallocate(start_, this->capacity());
            start_ = allocate_and_copy(another.size(), another.begin(), another.end());
            finish_ = end_of_storage_ = start_ + another.size();
        }
        else if (another.size() > this->size())
        {
            // 前一段使用copy构造，后半段调用构造函数
            copy(another.begin(), another.begin() + this->size(), start_);
            uninitialized_copy(another.begin() + this->size(), another.end(), finish_);
            finish_ = start_ + another.size();
        }
        else
        {
            copy(another.begin(), another.end(), start_);
            destroy(start_ + another.size(), finish_);
            finish_ = start_ + another.size();
        }
        return *this;
    }

    vector<T, Alloc>& operator=(vector<T, Alloc>&& another)
    {
        if (&another == this)
            return *this;
        start_ = another.start_;
        finish_ = another.finish_;
        end_of_storage_ = another.end_of_storage_;
        another.start_ = another.finish_ = another.end_of_storage_ = nullptr;
        return *this;
    }

    // 只有当 reserve 的size > capacity的时候调用才有效
    void reserve(size_type n)
    {
        if (n > capacity())
        {
            const size_type old_size = size();
            // 申请更大的空间，将原对象copy过来
            iterator tmp = allocate_and_copy(n, start_, finish_);
            // 释放原对象
            destroy(start_, finish_);
            // 释放原空间
            deallocate(start_, end_of_storage_ - start_);
            start_ = tmp;
            finish_ = tmp + old_size;
            end_of_storage_ = start_ + n;
        }
    }

    // assign 这个接口感觉没有用过
    void assign(size_type n, const_reference value)
    {
        if (n > capacity())
        {
            vector<T, Alloc> tmp(n, value, get_allocator());
            tmp.swap(*this);
        }
        else if (n > size())
        {
            fill(begin(), end(), value);
            finish_ = uninitialized_fill_n(finish_, n - size(), value);
        }
        else
        {
            iterator tmp = fill_n(begin(), n, value);
            destroy(tmp, finish_);
            finish_ = tmp;
        }
    }
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        using is_integer = typename __is_integer<InputIterator>::is_integer;
        assign_with_iterator(first, last, is_integer());
    }

protected:
    template <typename InputIterator>
    void assign_with_iterator(InputIterator first, InputIterator last, __true_type)
    {
        assign(static_cast<size_type>(first), static_cast<value_type>(last));
    }

    template <typename InputIterator>
    void assign_with_iterator(InputIterator first, InputIterator last, __false_type)
    {
        assign_aux(first, last, __ITERATOR_CATEGORY(first));
    }

    template <typename InputIterator>
    void assign_aux(InputIterator first, InputIterator last, input_iterator_tag)
    {
        iterator cur = begin();
        for (; cur != end() && first != last; ++cur, ++first)
            *cur = *first;
        if (first == last) // cur没到底，说明原来size大一点，把最后的erase掉
            erase(cur, end());
        else // 把多出来的insert进来
            insert(end(), first, last);
    }

    template <typename ForwardIter>
    void assign_aux(ForwardIter first, ForwardIter last, forward_iterator_tag)
    {
        size_type len = distance(first, last);
        if (len > capacity())
        {
            destroy(start_, finish_);
            deallocate(start_, this->capacity());
            start_ = allocate_and_copy(len, first, last);
            finish_ = end_of_storage_ = start_ + len;
        }
        else if (len > size())
        {
            ForwardIter mid = first;
            advance(mid, size());
            copy(first, mid, start_);
            finish_ = uninitialized_copy(mid, last, finish_);
        }
        else
        {
            iterator tmp = copy(first, last, start_);
            destroy(tmp, finish_);
            finish_ = tmp;
        }
    }

public:
    void push_back(T&& value)
    {
        if (finish_ != end_of_storage_)
        {
            construct(finish_, std::forward<T>(value));
            ++finish_;
        }
        else
            insert_aux(end(), std::forward<T>(value));
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        if (finish_ != end_of_storage_)
        {
            construct(finish_, std::forward<Args>(args)...);
            ++finish_;
        }
        else
        {
            insert_aux(end(), std::forward<Args>(args)...);
        }
    }
    void shrink_to_fit()
    {
        if (size() < capacity())
        {
            size_type old_size = size();
            iterator new_start = allocate_and_copy(old_size, start_, finish_);
            destroy(start_, finish_);
            deallocate(start_, capacity());
            start_ = new_start;
            finish_ = end_of_storage_ = start_ + old_size;
        }
    }

    void swap(vector<T, Alloc>& another)
    {
        hl::swap(start_, another.start_);
        hl::swap(finish_, another.finish_);
        hl::swap(end_of_storage_, another.end_of_storage_);
    }

    iterator insert(iterator position, T&& value)
    {
        size_type n = position - begin();
        if (finish_ != end_of_storage_ && position == end())
        {
            construct(finish_, std::forward<T>(value));
            ++finish_;
        }
        else
            insert_aux(position, std::forward<T>(value));
        return begin() + n;
    }

    void insert(iterator position, size_type n, const T& value)
    {
        fill_insert(position, n, value);
    }

    void insert(iterator position, size_type n)
    {
        fill_insert(position, n);
    }

    template <typename InputIterator>
    void insert(iterator position, InputIterator first,
                InputIterator last)
    {
        using is_integer = typename __is_integer<InputIterator>::is_integer;
        insert_dispatch(position, first, last, is_integer());
    }

protected:
    template <typename Integer>
    void insert_dispatch(iterator position, Integer n, Integer value, __true_type)
    {
        fill_insert(position, (size_type)n, (T)value);
    }

    template <typename InputIterator>
    void insert_dispatch(iterator position, InputIterator first, InputIterator last, __false_type)
    {
        range_insert(position, first, last, __ITERATOR_CATEGORY(first));
    }

    template <typename InputIterator>
    void range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
        {
            position = insert(position, *first);
            ++position;
        }
    }

    template <typename ForwardIterator>
    void range_insert(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        if (first != last)
        {
            size_type n = distance(first, last);
            if (size_type(end_of_storage_ - finish_) >= n) // 剩余空间大于n
            {
                // position之后的元素个数大于插入元素个数
                const size_type elems_after_positon = finish_ - position;
                iterator old_finish = finish_;
                if (elems_after_positon > n) // 插入点之后的元素个数大于新增元素个数
                {
                    // 把最后的n个元素往后copy
                    uninitialized_copy(finish_ - n, finish_, finish_);
                    finish_ += n;
                    copy_backward(position, old_finish - n, old_finish);
                    copy(first, last, position);
                }
                else
                {
                    ForwardIterator mid = first;
                    advance(mid, elems_after_positon);
                    // 先copy插入数据的后半段
                    uninitialized_copy(mid, last, finish_);
                    // 将position 到 finish_的数据移到末尾
                    uninitialized_copy(position, finish_, n + position);
                    finish_ += n;
                    copy(first, mid, position);
                }
            }
            else
            {
                const size_type old_size = size();
                const size_type new_size = old_size + hl::max(old_size, n);
                iterator new_start = allocate(new_size);
                iterator new_finish = new_start;
                new_finish = uninitialized_copy(start_, position, new_finish);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(position, finish_, new_finish);
                destroy(start_, finish_);
                deallocate(start_, end_of_storage_ - start_);
                start_ = new_start;
                finish_ = new_finish;
                end_of_storage_ = start_ + new_size;
            }
        }
    }

public:
    // iterator emplace(){}

    void pop_back()
    {
        --finish_;
        destroy(finish_);
    }
    iterator erase(iterator position)
    {
        if (position + 1 != end())
            copy(position + 1, end(), position);
        --finish_;
        destroy(finish_);
        return position;
    }
    iterator erase(iterator first, iterator last)
    {
        iterator tmp = copy(last, end(), first);
        destroy(tmp, end());
        finish_ = finish_ - (last - first);
        return first;
    }

    void resize(size_type new_size, const T& value)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), value);
    }
    void resize(size_type new_size)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size());
    }
    void clear()
    {
        erase(begin(), end());
    }

protected:
    template <typename ForwardIterator>
    iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last)
    {
        // TODO exception
        iterator result = allocate(n);
        uninitialized_copy(first, last, result);
        return result;
    }

    void insert_aux(iterator position, T&& value)
    {
        if (finish_ != end_of_storage_)
        {
            // 这里position一定不会是finish_ 除非有bug
            construct(finish_, *(finish_ - 1));
            ++finish_;
            // 从右往左依次copy每个元素
            copy_backward(position, finish_ - 2, finish_ - 1);
            *position = std::forward<T>(value);
        }
        else
        {
            // 这里可以不考虑size()*2溢出，因为目前的硬件达不到那么大?
            size_type old_size = size();
            size_type new_size = (old_size == 0) ? 1 : old_size * 2;
            iterator new_start = allocate(new_size);
            iterator new_finish = new_start;

            new_finish = uninitialized_copy(start_, position, new_finish);
            construct(new_finish, std::forward<T>(value));
            ++new_finish;
            new_finish = uninitialized_copy(position, finish_, new_finish);

            destroy(start_, finish_);
            deallocate(start_, old_size);

            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = new_start + new_size;
        }
    }

    template <typename... Args>
    void insert_aux(iterator position, Args&&... args)
    {
        if (finish_ != end_of_storage_)
        {
            ++finish_;
            // 从右往左依次copy每个元素 should be move copy
            copy_backward(position, finish_ - 1, finish_);
            construct(position, std::forward<Args>(args)...);
        }
        else
        {
            // 这里可以不考虑size()*2溢出，因为目前的硬件达不到那么大?
            size_type old_size = size();
            size_type new_size = (old_size == 0) ? 1 : old_size * 2;
            iterator new_start = allocate(new_size);
            iterator new_finish = new_start;

            new_finish = uninitialized_copy(start_, position, new_finish);
            construct(new_finish, std::forward<Args>(args)...);
            ++new_finish;
            new_finish = uninitialized_copy(position, finish_, new_finish);

            destroy(start_, finish_);
            deallocate(start_, old_size);

            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = new_start + new_size;
        }
    }

    void fill_insert(iterator position, size_type n, const T& value)
    {
        if (n > 0)
        {
            size_type remain = end_of_storage_ - finish_;

            if (remain >= n)
            {
                // position之后的元素个数大于插入元素个数
                const size_type elems_after_positon = finish_ - position;
                iterator old_finish = finish_;
                if (elems_after_positon > n) // 插入点之后的元素个数大于新增元素个数
                {
                    // 把最后的n个元素往后copy
                    uninitialized_copy(finish_ - n, finish_, finish_);
                    finish_ += n;
                    copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, value);
                }
                else
                {
                    uninitialized_fill(finish_, n + position, value);
                    uninitialized_copy(position, finish_, n + position);
                    fill(position, finish_, value);
                    finish_ += n;
                }
            }
            else // 需要重新申请空间
            {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n);
                iterator new_start = allocate(len);
                iterator new_finish = new_start;
                new_finish = uninitialized_copy(start_, position, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, value);
                new_finish = uninitialized_copy(position, finish_, new_finish);

                destroy(start_, finish_);
                deallocate(start_, end_of_storage_ - start_);
                start_ = new_start;
                finish_ = new_finish;
                end_of_storage_ = new_start + len;
            }
        }
    }

    void fill_insert(iterator position, size_type n)
    {
        if (n > 0)
        {
            size_type remain = end_of_storage_ - finish_;

            if (remain >= n)
            {
                // position之后的元素个数大于插入元素个数
                const size_type elems_after_positon = finish_ - position;
                iterator old_finish = finish_;
                if (elems_after_positon > n) // 插入点之后的元素个数大于新增元素个数
                {
                    // 把最后的n个元素往后copy
                    uninitialized_copy(finish_ - n, finish_, finish_);
                    finish_ += n;
                    copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, pointer(nullptr));
                }
                else
                {
                    uninitialized_fill(finish_, n + position, pointer(nullptr));
                    uninitialized_copy(position, finish_, n + position);
                    fill(position, finish_, pointer(nullptr));
                    finish_ += n;
                }
            }
            else // 需要重新申请空间
            {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n);
                iterator new_start = allocate(len);
                iterator new_finish = new_start;
                new_finish = uninitialized_copy(start_, position, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, pointer(nullptr));
                new_finish = uninitialized_copy(position, finish_, new_finish);

                destroy(start_, finish_);
                deallocate(start_, end_of_storage_ - start_);
                start_ = new_start;
                finish_ = new_finish;
                end_of_storage_ = new_start + len;
            }
        }
    }
};

} // namespace hl

#endif

