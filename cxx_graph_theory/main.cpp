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

    auto add(std::uint32_t id, Node node) -> bool
    {
        if (idExists(id))
        {
            return false;
        }
        nodes_.emplace_back(std::make_pair(id, std::move(node)));
        sortNodes();
        return true;
    }

    auto connect(std::uint32_t source, std::uint32_t sink) -> void
    {
        edges_.emplace_back(Edge {source, sink});
        sortEdges();
    }

    [[nodiscard]] auto node(std::uint32_t id) -> Node&
    {
        auto sameID = [id](auto const& n) { return n.first == id; };
        return std::find_if(begin(nodes_), end(nodes_), sameID)->second;
    }

    [[nodiscard]] auto node(std::uint32_t id) const -> Node const&
    {
        auto sameID = [id](auto const& n) { return n.first == id; };
        return std::find_if(begin(nodes_), end(nodes_), sameID)->second;
    }

    [[nodiscard]] auto size() const noexcept -> std::size_t
    {
        return nodes_.size();
    }

    [[nodiscard]] auto inEdges(std::uint32_t nodeID) const -> std::vector<Edge>
    {
        auto result = std::vector<Edge> {};
        std::copy_if(begin(edges_), end(edges_), std::back_inserter(result),
                     [nodeID](auto const& edge)
                     { return edge.Sink == nodeID; });
        return result;
    }

    [[nodiscard]] auto outEdges(std::uint32_t nodeID) const -> std::vector<Edge>
    {
        auto result = std::vector<Edge> {};
        std::copy_if(begin(edges_), end(edges_), std::back_inserter(result),
                     [nodeID](auto const& edge)
                     { return edge.Source == nodeID; });
        return result;
    }

    template<typename Func>
    auto forEach(Func func) const -> void
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
                  [](auto const& lhs, auto const& rhs)
                  { return lhs.first < rhs.first; });
    }

    auto sortEdges() -> void
    {
        std::sort(begin(edges_), end(edges_),
                  [](auto const& lhs, auto const& rhs)
                  { return lhs.Source < rhs.Source; });
    }

    std::vector<value_type> nodes_;
    std::vector<Edge> edges_;
};

/*
 * @brief Performs a depth first search on the graph to give us the topological
 * ordering we want. Instead of maintaining a stack of the nodes we see we
 * simply place them inside the ordering vector in reverse order for simplicity.
 */
auto depthFirstSearch(std::uint32_t orderIndex, std::uint32_t currentNodeID,
                      std::vector<bool>& visited,
                      std::vector<std::uint32_t>& ordering, Graph const& graph)
    -> std::uint32_t
{

    visited[currentNodeID] = true;

    if (auto const& edges = graph.outEdges(currentNodeID); !edges.empty())
    {
        for (auto const& edge : edges)
        {
            if (!visited[edge.Sink])
            {
                orderIndex = depthFirstSearch(orderIndex, edge.Sink, visited,
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
auto topologicalSort(Graph const& graph) -> std::vector<std::uint32_t>
{
    auto const size = graph.size();
    auto ordering   = std::vector<std::uint32_t>(size);
    auto visited    = std::vector<bool>(size);

    auto i = size - 1;
    for (std::uint32_t at = 0; at < size; at++)
    {
        if (!visited[at])
        {
            i = depthFirstSearch(static_cast<std::uint32_t>(i), at, visited,
                                 ordering, graph);
        }
    }

    return ordering;
}

auto Test_TopologicalSort() -> bool
{
    auto graph = Graph {};
    for (std::uint32_t i = 0; i < 7; i++)
    {
        graph.add(i, Node {});
    }

    graph.connect(0, 1);
    graph.connect(0, 2);
    graph.connect(0, 5);
    graph.connect(1, 3);
    graph.connect(1, 2);
    graph.connect(2, 3);
    graph.connect(2, 4);
    graph.connect(3, 4);
    graph.connect(5, 4);

    auto const expected = std::array<std::uint32_t, 7> {6, 0, 5, 1, 2, 3, 4};
    auto const ordering = topologicalSort(graph);
    return std::equal(begin(ordering), end(ordering), begin(expected));
}

class FindComponents
{
public:
    explicit FindComponents(Graph const& graph)
        : graph_(graph)
        , size_(static_cast<std::uint32_t>(graph_.size()))
        , count_(0)
        , components_(size_)
        , visited_(size_)
    {
        assert(components_.size() == size_);
        assert(visited_.size() == size_);

        for (std::uint32_t i = 0; i < size_; ++i)
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

        if (auto const& edges = graph_.outEdges(currentNode); !edges.empty())
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
    std::uint32_t size_;
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

    graph.connect(0, 1);
    graph.connect(1, 4);
    graph.connect(2, 3);
    graph.connect(3, 4);
    graph.connect(5, 6);

    std::cout << "Graph: \n";
    graph.forEach(
        [&graph](auto const& node)
        {
            std::cout << node.first << ": [";
            for (auto edge : graph.outEdges(node.first))
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

    auto const ordering = topologicalSort(graph);
    std::cout << "\nOrdering (TopologicalSort): \n";
    std::copy(begin(ordering), end(ordering), stream);
    std::cout << '\n';

    std::cout << "\nSimulation:\n";
    auto buffers = std::map<std::uint32_t, double> {};
    graph.forEach([&buffers](auto const& node)
                  { buffers.emplace(std::make_pair(node.first, 0.0)); });

    for (auto id : ordering)
    {
        auto const& node  = graph.node(id);
        auto const input  = buffers.at(id);
        auto const output = node.Handler(input);
        for (auto const& edge : graph.outEdges(id))
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