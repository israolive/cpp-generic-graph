#ifndef GRAPH_THEORY_MAYBE_HPP
#define GRAPH_THEORY_MAYBE_HPP

#include <concepts>
#include <utility>

template <class Data>
struct Some {
    Data data;

    Some(Data && data)
     : data(std::forward<Data>(data)) {}

    Some(Data const& data)
     : data(data) {}
};

struct None {};

/**
 * Maybe<R>
 * The struct always has some *data* (with type R),
 * but sometimes it isn't *valid*.
 * */
template <class Data>
struct Maybe {
    Data data;
    bool valid;

    Maybe(Data && data, bool valid)
     : data(data), valid(valid) {}

    Maybe(Some<Data> && other)
     : data(std::forward<Data>(other.data)), valid(true) {}

    Maybe(None && none)
     : data({}), valid(false) {}
};

#endif //GRAPH_THEORY_MAYBE_HPP
