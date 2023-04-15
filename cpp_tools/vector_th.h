#ifndef VECTOR_TH_H
#define VECTOR_TH_H

#include <vector>
#include <mutex>
#include <memory>

//!
//! 功能: 对vector容器加锁,保证多线程安全
//! 原理:保护继承vector容器,实现大部分函数接口, 使用继承构造获取大部分父类构造函数
//!
template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class vector_th : protected std::vector<_Tp, _Alloc>
{
public:
    //this class
    typedef std::lock_guard<std::mutex>                             lock_t;
    typedef std::vector<_Tp, _Alloc>                                vector;
    typedef std::vector<_Tp, _Alloc>                                parent;
    //C++STD
    typedef std::_Vector_base<_Tp, _Alloc>                          _Base;
    typedef typename _Base::_Tp_alloc_type                          _Tp_alloc_type;
    typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type>               _Alloc_traits;
    typedef _Tp                                                     value_type;
    typedef typename _Base::pointer                                 pointer;
    typedef typename _Alloc_traits::const_pointer                   const_pointer;
    typedef typename _Alloc_traits::reference                       reference;
    typedef typename _Alloc_traits::const_reference                 const_reference;
    typedef __gnu_cxx::__normal_iterator<pointer, vector>           iterator;
    typedef __gnu_cxx::__normal_iterator<const_pointer, vector>     const_iterator;
    typedef std::reverse_iterator<const_iterator>                   const_reverse_iterator;
    typedef std::reverse_iterator<iterator>                         reverse_iterator;
    typedef size_t                                                  size_type;
    typedef ptrdiff_t                                               difference_type;
    typedef _Alloc                                                  allocator_type;

    //this class
    using std::vector<_Tp, _Alloc>::vector;
    //C++STD
    using _Base::_M_allocate;
    using _Base::_M_deallocate;
    using _Base::_M_impl;
    using _Base::_M_get_Tp_allocator;

public:
    explicit vector_th(const vector_th& __x) : parent(__x) {}
    explicit vector_th(vector_th&& __x) : parent(std::move(__x)) {}

    reference operator[](size_type __n) _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return this->parent::operator[](__n); }

    reference at(size_type __n)
    { lock_t lock(_mutex); return parent::at(__n); }

    iterator begin() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::begin(); }

    iterator end() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::end(); }

    reverse_iterator rbegin() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::rbegin(); }

    reverse_iterator rend() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::rend(); }

    reference back() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::back(); }

    _Tp* data() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::data(); }

    void clear() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); parent::clear(); }

    reference front() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::front(); }

    void swap(vector& __x) _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); parent::swap(__x); }

    iterator erase(const_iterator __first, const_iterator __last)
    { lock_t lock(_mutex); return parent::erase(__first,__last); }

    iterator erase(const_iterator __position)
    { lock_t lock(_mutex); return parent::erase(__position); }

    void assign(size_type __n, const value_type& __val)
    { lock_t lock(_mutex); parent::assign(__n,__val); }

    template<typename _InputIterator, typename = std::_RequireInputIter<_InputIterator>>
    void assign(_InputIterator __first, _InputIterator __last)
    { lock_t lock(_mutex); parent::assign(__first,__last); }

    void assign(std::initializer_list<value_type> __l)
    { lock_t lock(_mutex); parent::assign(__l); }

    iterator insert(const_iterator __position, const value_type& __x)
    { lock_t lock(_mutex); parent::insert(__position,__x); }

    iterator insert(const_iterator __position, value_type&& __x)
    { lock_t lock(_mutex); parent::insert(__position,std::move(__x)); }

    iterator insert(const_iterator __position, std::initializer_list<value_type> __l)
    { lock_t lock(_mutex); parent::insert(__position,__l); }

    iterator insert(const_iterator __position, size_type __n, const value_type& __x)
    { lock_t lock(_mutex); parent::insert(__position,__n,__x); }

    template<typename _InputIterator, typename = std::_RequireInputIter<_InputIterator>>
    iterator insert(const_iterator __position, _InputIterator __first, _InputIterator __last)
    { lock_t lock(_mutex); parent::insert(__position,__first,__last); }

    void resize(size_type __new_size)
    { lock_t lock(_mutex); parent::resize(__new_size); }

    void resize(size_type __new_size, const value_type& __x)
    { lock_t lock(_mutex); parent::resize(__new_size,__x); }

    void reserve(size_type __n)
    { lock_t lock(_mutex); parent::reserve(__n); }

    void pop_back() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); parent::pop_back(); }

    void push_back(const value_type& __x)
    { lock_t lock(_mutex); parent::push_back(__x); }

    void push_back(value_type&& __x)
    { lock_t lock(_mutex); parent::push_back(std::move(__x)); }

    size_type size() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::size(); }

    size_type max_size() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::max_size(); }

    _GLIBCXX_NODISCARD
    bool empty() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::empty(); }

    template<typename... _Args>
    iterator emplace(const_iterator __position, _Args&&... __args)
    { lock_t lock(_mutex); return parent::emplace(__position,std::forward<_Args>(__args)...); }

    size_type capacity() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::capacity(); }

    template<typename... _Args>
    void emplace_back(_Args&&... __args)
    { lock_t lock(_mutex); parent::emplace_back(std::forward<_Args>(__args)...); }

protected:
    std::mutex _mutex;
};

#endif // VECTOR_TH_H
