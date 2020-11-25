#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <array>
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

using Graph = std::map<std::uint32_t, std::vector<Edge>>;

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

    if (auto const& edges = graph.at(currentNodeID); !edges.empty())
    {
        for (auto const& edge : edges)
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
    for (std::size_t at = 0; at < numNodes; at++)
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
    for (auto i = 0; i < 7; i++)
    {
        graph.emplace(i, std::vector<Edge> {});
    }
    graph.at(0).push_back(Edge {0, 1});
    graph.at(0).push_back(Edge {0, 2});
    graph.at(0).push_back(Edge {0, 5});
    graph.at(1).push_back(Edge {1, 3});
    graph.at(1).push_back(Edge {1, 2});
    graph.at(2).push_back(Edge {2, 3});
    graph.at(2).push_back(Edge {2, 4});
    graph.at(3).push_back(Edge {3, 4});
    graph.at(5).push_back(Edge {5, 4});

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
                dfs(i);
            }
        }
    }

    auto Get() const -> std::pair<std::uint32_t, std::vector<std::uint32_t>>
    {
        return std::make_pair(count_, components_);
    }

private:
    auto dfs(std::uint32_t currentNodeID) -> void
    {
        visited_[currentNodeID]    = true;
        components_[currentNodeID] = count_;

        if (auto const& edges = graph_.at(currentNodeID); !edges.empty())
        {
            for (auto const& edge : edges)
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

    auto stream      = std::ostream_iterator<std::uint32_t>(std::cout, " ");
    auto const graph = Graph {
        {0, {Edge {0, 1}}},
        {1, {Edge {1, 2}}},
        {2, {Edge {2, 3}, Edge {2, 6}}},
        {3, {}},
        {4, {Edge {4, 5}}},
        {5, {Edge {5, 2}}},
        {6, {}},
    };

    std::cout << "Graph: \n";
    for (auto const& node : graph)
    {
        std::cout << node.first << ": [";
        for (auto edge : node.second)
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

    return EXIT_SUCCESS;
}