#include <fmt/format.h>

#include "graph.hpp"
#include "representation/list_graph.hpp"

template <gg::Graph G>
auto print_graph() -> void {
    fmt::print("Graph<List_Graph> == {}\n", gg::Graph<G>);
}

auto main() -> int {
    print_graph<gg::List_Graph<std::size_t, float>>();

    gg::Graph auto graph = gg::List_Graph<int, float>();

    graph.add_vertex(5);
    graph.add_vertex(10);

    graph.add_edge(0, 1);

    auto [e, valid] = graph.get_edge(0, 5);

    fmt::print("Edge {} {} {} [{}]\n", e.i, e.j, e.w, valid);

    return 0;
}
