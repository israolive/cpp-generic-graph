#ifndef GRAPH_THEORY_GRAPH_HPP
#define GRAPH_THEORY_GRAPH_HPP

#include <type_traits>
#include <utility>
#include <concepts>

namespace gg {

template <class N>
concept Real = std::integral<N> || std::floating_point<N>;

template <Real R>
constexpr auto default_weight() -> R {
    return R(1);
}

template <class N>
concept Default_Weightable = requires(N n) {
    { default_weight<N>() };
};

struct Graph_Base {};

template <class G>
concept Graph = std::derived_from<G, Graph_Base> && requires(G g) {
    { std::declval<typename G::Vertex_Type>() };
    { std::declval<typename G::Edge_Weight>() };
    { g.has_vertex(std::declval<std::size_t>()) };
    { g.has_edge(std::declval<std::size_t>(), std::declval<std::size_t>()) };
    { g.get_vertex(std::declval<std::size_t>()) };
    { g.get_edge(std::declval<std::size_t>(), std::declval<std::size_t>()) };
    { g.add_vertex(std::declval<typename G::Vertex_Type>()) };
    { g.add_edge(std::declval<std::size_t>(), std::declval<std::size_t>(),
                    std::declval<typename G::Edge_Weight>(), std::declval<bool>()) };
};

template <class Weight>
struct Edge {
    std::size_t i;
    std::size_t j;
    Weight w;
};

}

#endif //GRAPH_THEORY_GRAPH_HPP
