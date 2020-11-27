#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

struct Edge
{
    std::uint32_t Source;
    std::uint32_t Sink;
};

auto operator<<(std::ostream& out, Edge const& edge) -> std::ostream&
{
    out << "[" << edge.Source << ", " << edge.Sink << "]";
    return out;
}

using EdgeList = std::vector<Edge>;
struct Node
{
    EdgeList Edges;
    std::function<double(double)> Handler = nullptr;
};

using Graph = std::map<std::uint32_t, Node>;

/*
 * @brief Performs a depth first search on the graph to give us the topological
 * ordering we want. Instead of maintaining a stack of the nodes we see we
 * simply place them inside the ordering vector in reverse order for simplicity.
 */
auto DepthFirstSearch(std::uint32_t orderIndex, std::uint32_t currentNodeID,
                      std::vector<bool>& visited,
                      std::vector<std::uint32_t>& ordering, Graph const& graph)
    -> std::uint32_t
{

    visited[currentNodeID] = true;

    if (auto const& node = graph.at(currentNodeID); !node.Edges.empty())
    {
        for (auto const& edge : node.Edges)
        {
            if (!visited[edge.Sink])
            {
                orderIndex = DepthFirstSearch(orderIndex, edge.Sink, visited,
                                              ordering, graph);
            }
        }
    }

    ordering[orderIndex] = currentNodeID;
    return orderIndex - 1;
}

/*
 * @brief Finds a topological ordering of the nodes in a Directed Acyclic Graph
 * (DAG). The input to this function is an adjacency list for a graph.
 */
auto TopologicalSort(Graph const& graph) -> std::vector<std::uint32_t>
{
    auto const numNodes = graph.size();
    auto ordering       = std::vector<std::uint32_t>(numNodes);
    auto visited        = std::vector<bool>(numNodes);

    auto i = numNodes - 1;
    for (std::uint32_t at = 0; at < numNodes; at++)
    {
        if (!visited[at])
        {
            i = DepthFirstSearch(i, at, visited, ordering, graph);
        }
    }

    return ordering;
}

auto Test_TopologicalSort() -> bool
{
    auto graph = Graph {};
    for (std::uint32_t i = 0; i < 7; i++)
    {
        graph.emplace(i, Node {std::vector<Edge> {}});
    }
    graph.at(0).Edges.push_back(Edge {0, 1});
    graph.at(0).Edges.push_back(Edge {0, 2});
    graph.at(0).Edges.push_back(Edge {0, 5});
    graph.at(1).Edges.push_back(Edge {1, 3});
    graph.at(1).Edges.push_back(Edge {1, 2});
    graph.at(2).Edges.push_back(Edge {2, 3});
    graph.at(2).Edges.push_back(Edge {2, 4});
    graph.at(3).Edges.push_back(Edge {3, 4});
    graph.at(5).Edges.push_back(Edge {5, 4});

    auto const expected = std::array<std::uint32_t, 7> {6, 0, 5, 1, 2, 3, 4};
    auto const ordering = TopologicalSort(graph);
    return std::equal(begin(ordering), end(ordering), begin(expected));
}

class FindComponents
{
public:
    explicit FindComponents(Graph const& graph)
        : graph_(graph)
        , numNodes_(graph_.size())
        , count_(0)
        , components_(numNodes_)
        , visited_(numNodes_)
    {
        assert(components_.size() == numNodes_);
        assert(visited_.size() == numNodes_);

        for (std::uint32_t i = 0; i < numNodes_; ++i)
        {
            if (!visited_[i])
            {
                ++count_;
                dfs({i});
            }
        }
    }

    auto Get() const -> std::pair<std::uint32_t, std::vector<std::uint32_t>>
    {
        return std::make_pair(count_, components_);
    }

private:
    auto dfs(std::uint32_t currentNode) -> void
    {
        visited_[currentNode]    = true;
        components_[currentNode] = count_;

        if (auto const& node = graph_.at(currentNode); !node.Edges.empty())
        {
            for (auto const& edge : node.Edges)
            {
                if (!visited_[edge.Sink])
                {
                    dfs(edge.Sink);
                }
            }
        }
    }

    Graph const& graph_;
    std::uint32_t numNodes_;
    std::uint32_t count_;
    std::vector<std::uint32_t> components_;
    std::vector<bool> visited_;
};

auto main() -> int
{
    if (!Test_TopologicalSort())
    {
        std::cerr << "TopologicalSort test failed\n";
        return EXIT_FAILURE;
    }

    auto stream         = std::ostream_iterator<std::uint32_t>(std::cout, " ");
    auto defaultHandler = [](auto x) { return x * 2.0; };
    // auto const graph    = Graph {
    //     {0, Node {{Edge {0, 1}}, [](auto /*x*/) { return 1.0; }}},
    //     {1, Node {{Edge {1, 2}}, defaultHandler}},
    //     {2, Node {{Edge {2, 3}, Edge {2, 4}}, defaultHandler}},
    //     {3, Node {{}, defaultHandler}},
    //     {4, Node {{}, defaultHandler}},
    // };

    auto const graph = Graph {
        {0, Node {{Edge {0, 1}}, [](auto /*x*/) { return 1.0; }}},
        {1, Node {{Edge {1, 4}}, defaultHandler}},
        {2, Node {{Edge {2, 3}}, [](auto /*x*/) { return 2.0; }}},
        {3, Node {{Edge {3, 4}}, defaultHandler}},
        {4, Node {{},
                  [](auto val) {
                      std::cout << "final: " << val << '\n';
                      return val;
                  }}},
    };

    std::cout << "Graph: \n";
    for (auto const& node : graph)
    {
        std::cout << node.first << ": [";
        for (auto edge : node.second.Edges)
        {
            std::cout << edge.Sink << " ";
        }
        std::cout << "]\n";
    }

    auto const components = FindComponents(graph).Get();
    std::cout << "\nComponents: \n";
    std::cout << "Count: " << components.first << '\n';
    std::copy(begin(components.second), end(components.second), stream);
    std::cout << '\n';

    auto const ordering = TopologicalSort(graph);
    std::cout << "\nOrdering (TopologicalSort): \n";
    std::copy(begin(ordering), end(ordering), stream);
    std::cout << '\n';

    std::cout << "\nSimulation:\n";
    auto buffers = std::map<std::uint32_t, double> {};
    for (auto const& node : graph)
    {
        buffers.emplace(std::make_pair(node.first, 0.0));
    }

    for (auto id : ordering)
    {
        auto const& node  = graph.find(id);
        auto const input  = buffers.at(id);
        auto const output = node->second.Handler(input);
        // std::cout << id << ": with: " << input << '\n';

        auto const& outEdges = node->second.Edges;
        for (auto const& edge : outEdges)
        {
            buffers.at(edge.Sink) += output;
        }
    }

    for (auto const& buffer : buffers)
    {
        std::cout << buffer.first << ": " << buffer.second << '\n';
    }

    return EXIT_SUCCESS;
}