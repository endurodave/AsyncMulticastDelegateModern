#ifndef _MAKE_TUPLE_HEAP_H
#define _MAKE_TUPLE_HEAP_H

#include <tuple>
#include <list>
#include <memory>

extern int newDeleteCnt;    // TODO REMOVE

class heap_arg_deleter_base
{
public:
    virtual ~heap_arg_deleter_base() {}
};

/// @brief Frees heap memory for each type of heap argument
template<typename T>
class heap_arg_deleter : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T& arg) : m_arg(arg) { }
    virtual ~heap_arg_deleter()
    {
        newDeleteCnt--;
        delete &m_arg;
    }
private:
    T& m_arg;
};

template<typename T>
class heap_arg_deleter<T&> : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T& arg) : m_arg(arg) { }
    virtual ~heap_arg_deleter()
    {
        newDeleteCnt--;
        delete &m_arg;
    }
private:
    T& m_arg;
};

template<typename T>
class heap_arg_deleter<T*> : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T* arg) : m_arg(arg) { }
    virtual ~heap_arg_deleter()
    {
        newDeleteCnt--;
        delete m_arg;
    }
private:
    T* m_arg;
};

template<typename T>
class heap_arg_deleter<T**> : public heap_arg_deleter_base
{
public:
    heap_arg_deleter(T** arg) : m_arg(arg) {}
    virtual ~heap_arg_deleter()
    {
        newDeleteCnt--;
        newDeleteCnt--;
        delete *m_arg;
        delete m_arg;
    }
private:
    T** m_arg;
};


/// @brief Append an argument to the tuple
template <typename Arg, typename... TupleElem>
auto tuple_append(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, const std::tuple<TupleElem...> &tup, Arg** arg)
{
    newDeleteCnt++;
    newDeleteCnt++;
    Arg** heap_arg = 0;
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

template <typename Arg, typename... TupleElem>
auto tuple_append(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, const std::tuple<TupleElem...> &tup, Arg* arg)
{
    newDeleteCnt++;
    Arg* heap_arg = 0;
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

template <typename Arg, typename... TupleElem>
auto tuple_append(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, const std::tuple<TupleElem...> &tup, Arg& arg)
{
    newDeleteCnt++;
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
        if (&heap_arg)
            delete &heap_arg;
        throw;
    }
}

template<typename... Ts>
auto make_tuple_heap(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, std::tuple<Ts...> tup)
{
    return tup;
}

/// @brief Creates a tuple with all tuple elements created on the heap using
/// operator new. Call with an empty list and tuple. The empty tuple is concatenated
/// with each heap element. The list contains pointers to each heap memory block
/// that will be automatically deleted.
template<typename Arg1, typename... Args, typename... Ts>
auto make_tuple_heap(std::list<std::shared_ptr<heap_arg_deleter_base>>& heapArgs, std::tuple<Ts...> tup, Arg1 arg1, Args... args)
{
    auto new_tup = tuple_append(heapArgs, tup, arg1);
    return make_tuple_heap(heapArgs, new_tup, args...);
}

#endif