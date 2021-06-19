#include <fmt/format.h>

#include "graph.hpp"
#include "representation/list_graph.hpp"

template <class G>
auto print_graph(G && g) -> void {
    fmt::print("Graph<List_Graph> == {}\n", gg::Graph<G>);
}

auto main() -> int {
    gg::Graph auto graph = gg::List_Graph<int, float>();

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_vertex(5);

    graph.add_edge(0, 1, 0.5);
    graph.add_edge(1, 0, 0.5);
    graph.add_edge(1, 2, 0.7);
    graph.add_edge(1, 3, 0.7);
    graph.add_edge(1, 4, 0.7);
    graph.add_edge(2, 4, 0.8);

    fmt::print("Graph Vertex Count {}\n", graph.get_vertex_count());
    fmt::print("Graph Edge Count {}\n", graph.get_edge_count());
    print_graph(graph);

    /*auto ptr = graph.get_vertex_data_mut(0);
    fmt::print("Index 0 is {}\n", *ptr);
    *ptr = 22;
    fmt::print("Index 0 is {}\n", *ptr);

    auto edge_ptr = graph.get_edge_weight_mut(0, 1);
    fmt::print("Edge 0,1 is {}\n", *edge_ptr);
    */

    for (auto neighbors = graph.get_neighbors(1); auto n : neighbors) {
        fmt::print("Edge with {} has {} weight\n", n.j, n.w);
    }

    graph.remove_vertex(0);
    fmt::print("Graph Vertex Count {}\n", graph.get_vertex_count());
    fmt::print("Graph Edge Count {}\n", graph.get_edge_count());
    graph.for_each_vertex([]<class V>(V && v) { fmt::print("V: {}\n", v.data); });
    graph.for_each_edge([]<class E>(E && e) { fmt::print("{} {} w{}\n", e.i, e.j, e.w); });

    return 0;
}
