#include <cstdio>

#include <algorithm>
#include <optional>
#include <vector>

namespace mc
{
template<class T>
class registry
{
public:
    using value_type = T;
    using size_type  = std::size_t;

    auto append(T element) -> size_type
    {
        map_.emplace_back(id_, std::move(element));
        ++size_;
        return id_++;
    }

    auto erase(size_type id) -> void
    {
        auto p = std::lower_bound(begin(map_), end(map_), id, [](auto const& a, auto const& b) { return a.first < b; });

        if (p == end(map_) || p->first != id)
        {
            return;
        }

        p->second.reset();
        --size_;

        if (size_ < (map_.size() / 2))
        {
            map_.erase(std::remove_if(begin(map_), end(map_), [](auto const& e) { return !e.second; }), end(map_));
        }
    }

    template<typename F>
    auto for_each(F f) -> void
    {
        for (auto const& e : map_)
        {
            if (e.second)
            {
                f(*e.second);
            }
        }
    }

private:
    std::vector<std::pair<size_type, std::optional<T>>> map_;
    size_type size_ = 0;
    size_type id_   = 0;
};
}  // namespace mc

auto use_registry(mc::registry<int>& reg)
{
    auto const ticket = reg.append(143);
    reg.for_each([](auto const& val) { std::printf("before: %d\n", val); });
    reg.erase(ticket);
    reg.for_each([](auto const& val) { std::printf("after: %d\n", val); });
}

int main(int, char**)
{
    auto reg = mc::registry<int> {};
    use_registry(reg);
    return 0;
}
