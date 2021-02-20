#ifndef GRAPH_THEORY_GRAPH_HPP
#define GRAPH_THEORY_GRAPH_HPP

#include <type_traits>
#include <tuple>
#include <vector>

namespace graph {

struct graph_base {};

template <typename G>
inline constexpr auto is_graph = std::derived_from<G, graph_base>;

template <typename G>
using enable_graph = std::enable_if_t<is_graph<G>, G>;

template <typename ... T>
struct add_vertex {
    std::tuple<T...> args;
};

template <typename ... T>
add_vertex(T&& ... args) -> add_vertex<T...>;

template <typename G, typename ... T>
inline constexpr auto operator|(G && g, add_vertex<T...> && add) -> G& {
    return std::apply(&std::remove_reference_t<G>::template add_vertex<T...>,
            std::tuple_cat(std::make_tuple(&g), add.args));
}

template <typename VertexType>
struct list_graph {
    std::vector<VertexType> vertex;

    template <typename ... T>
    auto add_vertex(T && ... data) -> list_graph& {
        this->vertex.emplace_back(std::forward<T>(data)...);
        return *this;
    }
};

auto main() -> int {
    auto graph = list_graph<int>()
                        | add_vertex{5}
                        | add_vertex{10}
                        | add_vertex{15};

    return 0;
}

}

#endif //GRAPH_THEORY_GRAPH_HPP
