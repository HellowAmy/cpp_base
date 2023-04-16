#ifndef MAP_TH_H
#define MAP_TH_H

#include <map>
#include <mutex>
#include <memory>

//!
//! 功能: 对map容器加锁,保证多线程下调用接口安全,不保证迭代器安全
//! 原理:保护继承map容器,实现大部分函数接口, 使用继承构造获取大部分父类构造函数
//!
template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>,
      typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
class map_th : protected std::map<_Key,_Tp,_Compare,_Alloc>
{
public:
    typedef _Key                            key_type;
    typedef _Tp                             mapped_type;
    typedef std::pair<const _Key, _Tp>		value_type;
    typedef _Compare                        key_compare;
    typedef _Alloc                          allocator_type;

    typedef typename __gnu_cxx::__alloc_traits<_Alloc>::
            template rebind<value_type>::other                          _Pair_alloc_type;
    typedef std::_Rb_tree<key_type, value_type, std::_Select1st<value_type>,
             key_compare, _Pair_alloc_type>                             _Rep_type;

    typedef __gnu_cxx::__alloc_traits<_Pair_alloc_type>         _Alloc_traits;
    typedef typename _Alloc_traits::pointer                     pointer;
    typedef typename _Alloc_traits::const_pointer               const_pointer;
    typedef typename _Alloc_traits::reference                   reference;
    typedef typename _Alloc_traits::const_reference             const_reference;
    typedef typename _Rep_type::iterator                        iterator;
    typedef typename _Rep_type::const_iterator                  const_iterator;
    typedef typename _Rep_type::size_type                       size_type;
    typedef typename _Rep_type::difference_type                 difference_type;
    typedef typename _Rep_type::reverse_iterator                reverse_iterator;
    typedef typename _Rep_type::const_reverse_iterator          const_reverse_iterator;

    //this class
    typedef std::map<_Key,_Tp,_Compare,_Alloc>      parent;
    typedef std::lock_guard<std::mutex>             lock_t;

public:
    //继承构造函数,并覆盖父类的拷贝构造
    using std::map<_Key,_Tp,_Compare,_Alloc>::map;
    map_th(const map_th& __m) : parent(__m) {}
    map_th(map_th&& __m) : parent(std::move(__m)) {}

    mapped_type& at(const key_type& __k)
    { lock_t lock(_mutex); return parent::at(__k); }

    mapped_type& operator[](const key_type& __k)
    { lock_t lock(_mutex); return parent::operator[](__k); }

    mapped_type& operator[](key_type&& __k)
    { lock_t lock(_mutex); return parent::operator[](std::forward<key_type>(__k)); }

    iterator begin() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::begin(); }

    iterator end() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::end(); }

    reverse_iterator rbegin() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::rbegin(); }

    reverse_iterator rend() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::rend(); }

    iterator find(const key_type& __x)
    { lock_t lock(_mutex); return parent::find(__x); }

    void clear() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::clear(); }

    iterator erase(const_iterator __position)
    { lock_t lock(_mutex); return parent::erase(__position); }

    iterator erase(iterator __position)
    { lock_t lock(_mutex); return parent::erase(__position); }

    size_type erase(const key_type& __x)
    { lock_t lock(_mutex); return parent::erase(__x); }

    iterator erase(const_iterator __first, const_iterator __last)
    { lock_t lock(_mutex); return parent::erase(__first,__last); }

    std::pair<iterator, bool> insert(const value_type& __x)
    { lock_t lock(_mutex); return parent::insert(__x); }

    std::pair<iterator, bool> insert(value_type&& __x)
    { lock_t lock(_mutex); return parent::insert(move(__x)); }

    template<typename _Pair>
    std::__enable_if_t<std::is_constructible<value_type, _Pair>::value,std::pair<iterator, bool>>
    insert(_Pair&& __x)
    { lock_t lock(_mutex); return parent::insert(std::forward<_Pair>(__x)); }

    void insert(std::initializer_list<value_type> __list)
    { lock_t lock(_mutex); return parent::insert(__list); }

    iterator insert(const_iterator __position, const value_type& __x)
    { lock_t lock(_mutex); return parent::insert(__position,__x); }

    iterator insert(const_iterator __position, value_type&& __x)
    { lock_t lock(_mutex); return parent::insert(__position,move(__x)); }

    template<typename _Pair>
    std::__enable_if_t<std::is_constructible<value_type, _Pair>::value, iterator>
    insert(const_iterator __position, _Pair&& __x)
    { lock_t lock(_mutex); return parent::insert(__position,std::forward<_Pair>(__x)); }

    template<typename _InputIterator>
    void insert(_InputIterator __first, _InputIterator __last)
    { lock_t lock(_mutex); return parent::insert(__first,__last); }

    template<typename... _Args>
    std::pair<iterator, bool> emplace(_Args&&... __args)
    { lock_t lock(_mutex); return parent::emplace(std::forward<_Args>(__args)...); }

    template<typename... _Args>
    iterator emplace_hint(const_iterator __pos, _Args&&... __args)
    { lock_t lock(_mutex); return parent::emplace_hint(__pos,std::forward<_Args>(__args)...); }

    _GLIBCXX_NODISCARD bool empty() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::empty(); }

    size_type size() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::size(); }

    size_type max_size() _GLIBCXX_NOEXCEPT
    { lock_t lock(_mutex); return parent::max_size(); }

protected:
    std::mutex _mutex;
};
#endif // MAP_TH_H
