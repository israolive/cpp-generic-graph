#ifndef GRAPH_THEORY_MAYBE_HPP
#define GRAPH_THEORY_MAYBE_HPP

#include <concepts>
#include <utility>

struct Blank_Type {};

template <class Data>
struct Some {
    Data data;

    explicit Some(Data && data)
     : data(std::forward<Data>(data)) {}

    explicit Some(Data const& data)
     : data(data) {}
};

struct None{};

/**
 * Maybe<R>
 * The struct always has some *data* (with type R),
 * but sometimes it isn't *valid*.
 *
 * Accessing *data* when *valid* is false (aka None)
 * has undefined behavior.
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
            : data(), valid(false) {}

    explicit operator bool() const {
        return valid;
    }
};

#endif //GRAPH_THEORY_MAYBE_HPP
