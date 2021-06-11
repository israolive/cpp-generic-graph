#ifndef GRAPH_THEORY_LIST_GRAPH_HPP
#define GRAPH_THEORY_LIST_GRAPH_HPP

#include "../graph.hpp"

#include <ranges>
#include <vector>

#include <optional>
#include <exception>

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
    using V_Handle = Vertex_Handle<Vertex, Edge<Edge_Weight>>;

protected:
    std::vector<V_Handle> m_handles;

    auto find_edge(std::size_t i, std::size_t j) {
        return std::make_pair(
                std::ranges::find_if(m_handles[i].neighbors, [j]<class E>(E && e) { return j == e.j; }),
        std::end(m_handles[i].neighbors));
    }

    template <class Pred>
    auto find_edge_if(std::size_t i, std::size_t j, Pred pred) {
        return std::make_pair(std::ranges::find_if(m_handles[i].neighbors, pred), std::end(m_handles[i].neighbors));
    }

public:
    auto has_vertex(std::size_t at) -> bool {
        return at >= 0 && at < std::size(m_handles);
    }

    auto has_edge(std::size_t i, std::size_t j) -> bool {
        auto [it, end] = find_edge(i, j);
        return it != end;
    }

    auto get_vertex(std::size_t at) -> Maybe<Vertex_Type> {
        if (has_vertex(at)) {
            return Some(m_handles[at].data);
        }
        return None();
    }

    auto get_edge(std::size_t i, std::size_t j) -> Maybe<E_Handle> {
        if (auto [it, end] = find_edge(i, j); it != end) {
            return Some(std::forward<E_Handle>(*it));
        }
        return None();
    }

    auto add_vertex(Vertex_Type && v) -> bool {
        m_handles.push_back({ .data = std::forward<Vertex_Type>(v), .neighbors = {} });
        return true;
    }

    auto add_edge(std::size_t i, std::size_t j, Edge_Weight && w = default_weight<Edge_Weight>(), bool directed = false) -> bool {
        auto [it, end] = find_edge(i, j);

        if (it == end) {
            m_handles[i].neighbors.push_back({ i, j, w });

            if (!directed) {
                return add_edge(j, i, std::forward<Edge_Weight>(w), true);
            }

            return true;
        }

        return false;
    }
};

}

#endif //GRAPH_THEORY_LIST_GRAPH_HPP
