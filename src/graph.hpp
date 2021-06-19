#ifndef GRAPH_THEORY_GRAPH_HPP
#define GRAPH_THEORY_GRAPH_HPP

#include <type_traits>
#include <utility>
#include <concepts>

namespace gg {

struct Blank{};

enum class Relation {
    Undirected = 0, Directed = 1
};

template <class Weight>
struct Edge {
    std::size_t i;
    std::size_t j;
    Weight w;
};

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

template <class G>
using vertex_label_t = typename G::Vertex_Type;

template <class G>
using edge_weight_t = typename G::Edge_Weight;

struct Graph_Base {};

template <class G>
concept Graph = std::derived_from<G, Graph_Base> && requires(G g) {
    { std::declval<vertex_label_t<G>>() };
    { std::declval<edge_weight_t<G>>() };
    { g.get_vertex_count() };
    { g.get_edge_count() };
    { g.has_vertex(std::declval<std::size_t>()) };
    { g.has_edge(std::declval<std::size_t>(), std::declval<std::size_t>()) };
    { g.get_vertex_data(std::declval<std::size_t>()) };
    { g.get_edge_weight(std::declval<std::size_t>(), std::declval<std::size_t>()) };
    { g.add_vertex(std::declval<vertex_label_t<G>>()) };
    { g.add_edge(std::declval<Relation>(), std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<edge_weight_t<G>>()) };
    { g.add_edge(std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<edge_weight_t<G>>()) };
    { g.get_neighbors(std::declval<std::size_t>()) };
    { g.set_vertex_data(std::declval<std::size_t>(), std::declval<vertex_label_t<G>>()) };
    { g.set_edge_weight(std::declval<Relation>(), std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<edge_weight_t<G>>()) };
    { g.set_edge_weight(std::declval<std::size_t>(), std::declval<std::size_t>(), std::declval<edge_weight_t<G>>()) };
    { g.get_vertex_data_mut(std::declval<std::size_t>()) };
    { g.get_edge_weight_mut(std::declval<std::size_t>(), std::declval<std::size_t>()) };
};

} // gg

#endif //GRAPH_THEORY_GRAPH_HPP
