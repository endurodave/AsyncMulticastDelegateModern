#ifndef _MAKE_TUPLE_HEAP_H
#define _MAKE_TUPLE_HEAP_H

#include <tuple>
#include <list>
#include <memory>
#include <type_traits>

namespace DelegateLib 
{

// std::shared_ptr reference arguments are not allowed with asynchronous delegates as the behavior is 
// undefined. In other words:
// void MyFunc(std::shared_ptr<T> data)		// Ok!
// void MyFunc(std::shared_ptr<T>* data)	// Error if DelegateAsync or DelegateSpAsync target!
template<class T>
struct is_shared_ptr : std::false_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>&> : std::true_type {};

template<class T>
struct is_shared_ptr<const std::shared_ptr<T>&> : std::true_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>*> : std::true_type {};

template<class T>
struct is_shared_ptr<const std::shared_ptr<T>*> : std::true_type {};
   
/// @brief Base class for all deleter's
class heap_arg_deleter_base
{
public:
    virtual ~heap_arg_deleter_base() = default;
};

/// @brief Frees heap memory for reference heap argument
template<typename T>
class heap_arg_deleter : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T& arg) : m_arg(arg) { }
    virtual ~heap_arg_deleter()
    {
        delete &m_arg;
    }
private:
    T& m_arg;
};

/// @brief Frees heap memory for pointer heap argument
template<typename T>
class heap_arg_deleter<T*> : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T* arg) : m_arg(arg) { }
    virtual ~heap_arg_deleter()
    {
        delete m_arg;
    }
private:
    T* m_arg;
};

/// @brief Frees heap memory for pointer to pointer heap argument
template<typename T>
class heap_arg_deleter<T**> : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T** arg) : m_arg(arg) {}
    virtual ~heap_arg_deleter()
    {
        delete *m_arg;
        delete m_arg;
    }
private:
    T** m_arg;
};

/// @brief Append a pointer to pointer argument to the tuple
template <typename Arg, typename... TupleElem>
auto tuple_append(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, const std::tuple<TupleElem...> &tup, Arg** arg)
{
    Arg** heap_arg = nullptr;
    try 
    {
        heap_arg = new Arg*();
        *heap_arg = new Arg(**arg);

        std::shared_ptr<heap_arg_deleter_base> deleter(new heap_arg_deleter<Arg**>(heap_arg));
        heapArgs.push_back(deleter);

        return std::tuple_cat(tup, std::make_tuple(heap_arg));
    }
    catch (std::bad_alloc&)
    {
        if (*heap_arg)
            delete *heap_arg;
        if (heap_arg)
            delete heap_arg;
        throw;
    }
}

/// @brief Append a pointer argument to the tuple
template <typename Arg, typename... TupleElem>
auto tuple_append(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, const std::tuple<TupleElem...> &tup, Arg* arg)
{
    Arg* heap_arg = nullptr;
    try
    {
        heap_arg = new Arg(*arg);

        std::shared_ptr<heap_arg_deleter_base> deleter(new heap_arg_deleter<Arg*>(heap_arg));
        heapArgs.push_back(deleter);

        return std::tuple_cat(tup, std::make_tuple(heap_arg));
    }
    catch (std::bad_alloc&)
    {
        if (heap_arg)
            delete heap_arg;
        throw;
    }
}

/// @brief Append a reference argument to the tuple
template <typename Arg, typename... TupleElem>
auto tuple_append(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, const std::tuple<TupleElem...> &tup, Arg& arg)
{
    Arg& heap_arg = *(new Arg(arg));
    try
    {
        std::shared_ptr<heap_arg_deleter_base> deleter(new heap_arg_deleter<Arg&>(heap_arg));
        heapArgs.push_back(deleter);

        auto temp = std::make_tuple(std::forward_as_tuple(heap_arg));
        auto new_type = std::get<0>(temp);
        return std::tuple_cat(tup, new_type);
    }
    catch (std::bad_alloc&)
    {
        delete &heap_arg;
        throw;
    }
}

/// @brief Terminate the template metaprogramming argument loop
template<typename... Ts>
auto make_tuple_heap(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, std::tuple<Ts...> tup)
{
    return tup;
}

/// @brief Creates a tuple with all tuple elements created on the heap using
/// operator new. Call with an empty list and empty tuple. The empty tuple is concatenated
/// with each heap element. The list contains heap_arg_deleter_base objects for each 
/// argument heap memory block that will be automatically deleted after the bound
/// function is invoked on the target thread. 
template<typename Arg1, typename... Args, typename... Ts>
auto make_tuple_heap(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, std::tuple<Ts...> tup, Arg1 arg1, Args... args)
{
    static_assert(!(
        (is_shared_ptr<Arg1>::value && (std::is_lvalue_reference_v<Arg1> || std::is_pointer_v<Arg1>))),
        "std::shared_ptr reference argument not allowed");

    auto new_tup = tuple_append(heapArgs, tup, arg1);
    return make_tuple_heap(heapArgs, new_tup, args...);
}

}

#endif
