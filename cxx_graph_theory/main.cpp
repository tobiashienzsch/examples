#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
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

struct Node
{
    std::function<double(double)> Handler = nullptr;
};

struct Graph
{
public:
    using value_type = std::pair<std::uint32_t, Node>;

    explicit Graph(std::initializer_list<value_type> ilist) : nodes_(ilist) { }

    auto AddNode(std::uint32_t id, Node node) -> bool
    {
        if (idExists(id))
        {
            return false;
        }
        nodes_.emplace_back(std::make_pair(id, std::move(node)));
        sortNodes();
        return true;
    }

    auto Connect(std::uint32_t source, std::uint32_t sink) -> void
    {
        edges_.emplace_back(Edge {source, sink});
        sortEdges();
    }

    [[nodiscard]] auto GetNode(std::uint32_t id) -> Node&
    {
        auto sameID = [id](auto const& n) { return n.first == id; };
        return std::find_if(begin(nodes_), end(nodes_), sameID)->second;
    }

    [[nodiscard]] auto GetNode(std::uint32_t id) const -> Node const&
    {
        auto sameID = [id](auto const& n) { return n.first == id; };
        return std::find_if(begin(nodes_), end(nodes_), sameID)->second;
    }

    [[nodiscard]] auto NumNodes() const noexcept -> std::size_t
    {
        return nodes_.size();
    }

    [[nodiscard]] auto GetInEdgesFor(std::uint32_t nodeID) const
        -> std::vector<Edge>
    {
        auto result = std::vector<Edge> {};
        std::copy_if(
            begin(edges_), end(edges_), std::back_inserter(result),
            [nodeID](auto const& edge) { return edge.Sink == nodeID; });
        return result;
    }

    [[nodiscard]] auto GetOutEdgesFor(std::uint32_t nodeID) const
        -> std::vector<Edge>
    {
        auto result = std::vector<Edge> {};
        std::copy_if(
            begin(edges_), end(edges_), std::back_inserter(result),
            [nodeID](auto const& edge) { return edge.Source == nodeID; });
        return result;
    }

    template<typename Func>
    auto ForEachNode(Func func) const -> void
    {
        for (auto const& node : nodes_)
        {
            func(node);
        }
    }

private:
    [[nodiscard]] auto idExists(std::uint32_t id) const noexcept -> bool
    {
        auto equal = [id](auto const& node) { return node.first == id; };
        return std::find_if(begin(nodes_), end(nodes_), equal) != end(nodes_);
    }

    auto sortNodes() -> void
    {
        std::sort(begin(nodes_), end(nodes_),
                  [](auto const& lhs, auto const& rhs) {
                      return lhs.first < rhs.first;
                  });
    }

    auto sortEdges() -> void
    {
        std::sort(begin(edges_), end(edges_),
                  [](auto const& lhs, auto const& rhs) {
                      return lhs.Source < rhs.Source;
                  });
    }

    std::vector<value_type> nodes_;
    std::vector<Edge> edges_;
};

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

    if (auto const& edges = graph.GetOutEdgesFor(currentNodeID); !edges.empty())
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
    auto const numNodes = graph.NumNodes();
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
        graph.AddNode(i, Node {});
    }

    graph.Connect(0, 1);
    graph.Connect(0, 2);
    graph.Connect(0, 5);
    graph.Connect(1, 3);
    graph.Connect(1, 2);
    graph.Connect(2, 3);
    graph.Connect(2, 4);
    graph.Connect(3, 4);
    graph.Connect(5, 4);

    auto const expected = std::array<std::uint32_t, 7> {6, 0, 5, 1, 2, 3, 4};
    auto const ordering = TopologicalSort(graph);
    return std::equal(begin(ordering), end(ordering), begin(expected));
}

class FindComponents
{
public:
    explicit FindComponents(Graph const& graph)
        : graph_(graph)
        , numNodes_(graph_.NumNodes())
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
    auto dfs(std::uint32_t currentNode) -> void
    {
        visited_[currentNode]    = true;
        components_[currentNode] = count_;

        if (auto const& edges = graph_.GetOutEdgesFor(currentNode);
            !edges.empty())
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

    auto stream         = std::ostream_iterator<std::uint32_t>(std::cout, " ");
    auto defaultHandler = [](auto x) { return x * 2.0; };

    auto graph = Graph {
        {0, Node {[](auto /*x*/) { return 1.0; }}},
        {1, Node {defaultHandler}},
        {2, Node {[](auto /*x*/) { return 2.0; }}},
        {3, Node {defaultHandler}},
        {4, Node {[](auto x) { return x; }}},
        {5, Node {[](auto x) { return x; }}},
        {6, Node {[](auto x) { return x; }}},
    };

    graph.Connect(0, 1);
    graph.Connect(1, 4);
    graph.Connect(2, 3);
    graph.Connect(3, 4);
    graph.Connect(5, 6);

    std::cout << "Graph: \n";
    graph.ForEachNode([&graph](auto const& node) {
        std::cout << node.first << ": [";
        for (auto edge : graph.GetOutEdgesFor(node.first))
        {
            std::cout << edge.Sink << " ";
        }
        std::cout << "]\n";
    });

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
    graph.ForEachNode([&buffers](auto const& node) {
        buffers.emplace(std::make_pair(node.first, 0.0));
    });

    for (auto id : ordering)
    {
        auto const& node  = graph.GetNode(id);
        auto const input  = buffers.at(id);
        auto const output = node.Handler(input);
        for (auto const& edge : graph.GetOutEdgesFor(id))
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