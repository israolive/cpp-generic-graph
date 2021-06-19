#ifndef GRAPH_THEORY_LIST_GRAPH_HPP
#define GRAPH_THEORY_LIST_GRAPH_HPP

#include "../graph.hpp"

#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

#include "utility/maybe.hpp"

namespace gg {

template <class Data, class Relationship>
struct Vertex_Handle {
    Data data;
    std::vector<Relationship> neighbors;
};

template <class Vertex, Default_Weightable Weight>
class List_Graph: public Graph_Base {
public:
    using Vertex_Type = Vertex;
    using Edge_Weight = Weight;

    using E_Handle = Edge<Weight>;
    using V_Handle = Vertex_Handle<Vertex, E_Handle>;
    using Edge_Set = decltype(V_Handle::neighbors);

protected:
    using Edge_It = typename Edge_Set::iterator;
    using Edge_It_Pair = std::pair<Edge_It, Edge_It>;

    std::vector<V_Handle> m_handles;
    Relation m_relation;

    template <class Pred>
    auto nocheck_find_edge_if(std::size_t i, Pred pred) -> Edge_It_Pair {
        return { std::ranges::find_if(m_handles[i].neighbors, pred), std::end(m_handles[i].neighbors) };
    }

    auto nocheck_find_edge(std::size_t i, std::size_t j) -> Edge_It_Pair {
        return nocheck_find_edge_if(i, [j]<class E>(E && e) -> bool { return j == e.j; });
    }

    template <class Pred>
    auto find_edge_if(std::size_t i, Pred pred) -> Edge_It_Pair {
        if (has_vertex(i)) {
            return nocheck_find_edge_if(i, pred);
        }
        return {};
    }

    auto find_edge(std::size_t i, std::size_t j) -> Edge_It_Pair {
        return find_edge_if(i, [j]<class E>(E && e) -> bool { return j == e.j; });
    }

public:
    List_Graph()
     : m_handles(), m_relation(Relation::Directed)
    {}

    explicit List_Graph(Relation r)
     : m_handles(), m_relation(r)
    {}

    List_Graph(List_Graph<Vertex, Weight> const& other) = default;
    List_Graph(List_Graph<Vertex, Weight> && other) noexcept = default;

    auto get_vertex_count() const noexcept -> std::size_t {
        return std::size(m_handles);
    }

    auto get_edge_count() const noexcept -> std::size_t {
        return std::accumulate(std::begin(m_handles), std::end(m_handles), 0ull, []<class S, class V>(S && s, V && v) {
            return s + std::size(v.neighbors);
        });
    }

    auto has_vertex(std::size_t at) const noexcept -> bool {
        return at >= 0 && at < std::size(m_handles);
    }

    auto has_edge(std::size_t i, std::size_t j) const noexcept -> bool {
        auto [it, end] = find_edge(i, j);
        return it != end;
    }

    auto get_vertex_data(std::size_t at) const noexcept -> Maybe<Vertex_Type> {
        if (has_vertex(at)) {
            return Some(m_handles[at].data);
        }
        return None();
    }

    auto get_edge_weight(std::size_t i, std::size_t j) const noexcept -> Maybe<Edge_Weight> {
        if (auto [it, end] = find_edge(i, j); it != end) {
            return Some(it->w);
        }
        return None();
    }

    auto add_vertex(Vertex_Type && v = Vertex_Type{}) noexcept -> bool {
        m_handles.push_back({ .data = std::forward<Vertex_Type>(v), .neighbors = {} });
        return true;
    }

    auto add_edge(Relation r, std::size_t i, std::size_t j, Edge_Weight && w = default_weight<Edge_Weight>()) noexcept -> bool {
        if (has_vertex(i) && has_vertex(j)) {
            if (auto [it, end] = nocheck_find_edge(i, j); it == end) {
                m_handles[i].neighbors.push_back({ i, j, w });

                if (r == Relation::Undirected) {
                    return add_edge(Relation::Directed, j, i, std::forward<Edge_Weight>(w));
                }

                return true;
            }
        }
        return false;
    }

    auto add_edge(std::size_t i, std::size_t j, Edge_Weight && w = default_weight<Edge_Weight>()) noexcept -> bool {
        return add_edge(m_relation, i, j, std::forward<Edge_Weight>(w));
    }

    auto get_neighbors(std::size_t at) const noexcept -> std::span<std::add_const_t<E_Handle>> {
        if (has_vertex(at)) {
            return std::span(std::cbegin(m_handles[at].neighbors), std::size(m_handles[at].neighbors));
        }
        return {};
    }

    auto set_vertex_data(std::size_t at, Vertex_Type && data) noexcept -> bool {
        if (has_vertex(at)) {
            m_handles[at].data = std::forward<Vertex_Type>(data);
            return true;
        }
        return false;
    }

    auto set_edge_weight(Relation r, std::size_t i, std::size_t j, Edge_Weight && w = default_weight<Edge_Weight>()) noexcept -> bool {
        if (auto [it, end] = find_edge(i, j); it != end) {
            it->w = w;

            if (r == Relation::Undirected) {
                return set_edge_weight(Relation::Directed, j, i, std::forward<Edge_Weight>(w));
            }

            return true;
        }
        return false;
    }

    auto set_edge_weight(std::size_t i, std::size_t j, Edge_Weight && w = default_weight<Edge_Weight>()) noexcept -> bool {
        return set_edge_weight(m_relation, i, j, std::forward<Edge_Weight>(w));
    }

    auto get_vertex_data_mut(std::size_t at) noexcept -> Vertex_Type * {
        if (has_vertex(at)) {
            return std::addressof(m_handles[at].data);
        }
        return nullptr;
    }

    auto get_edge_weight_mut(std::size_t i, std::size_t j) noexcept -> Edge_Weight * {
        if (auto [it, end] = find_edge(i, j); it != end) {
            return std::addressof(it->w);
        }
        return nullptr;
    }

    auto remove_vertex(std::size_t at) -> bool {
        if (has_vertex(at)) {
            m_handles.erase(std::next(std::begin(m_handles), at));

            std::ranges::for_each(m_handles, [at]<class V>(V & v) {
                for (auto it = std::begin(v.neighbors); it != std::end(v.neighbors); /**/) {
                    if (it->j == at) {
                        v.neighbors.erase(it);
                    }
                    else {
                        if (it->i > at) --(it->i);
                        if (it->j > at) --(it->j);
                        ++it;
                    }
                }
            });

            return true;
        }
        return false;
    }

    auto remove_edge(Relation r, std::size_t i, std::size_t j) noexcept -> bool {
        if (has_vertex(i) && has_vertex(j)) {
            if (auto [it, end] = nocheck_find_edge(i, j); it != end) {
                m_handles[i].erase(it);

                if (r == Relation::Undirected) {
                    return remove_edge(Relation::Directed, j, i);
                }

                return true;
            }
        }
        return false;
    }

    auto remove_edge(std::size_t i, std::size_t j) noexcept -> bool {
        return remove_edge(m_relation, i, j);
    }

    template <class Unary>
    auto for_each_vertex(Unary u) -> void {
        std::ranges::for_each(m_handles, u);
    }

    template <class Unary>
    auto for_each_edge(Unary u) -> void {
        std::ranges::for_each(m_handles, [u]<class V>(V && v) {
            std::ranges::for_each(v.neighbors, u);
        });
    }
};

}

#endif //GRAPH_THEORY_LIST_GRAPH_HPP
