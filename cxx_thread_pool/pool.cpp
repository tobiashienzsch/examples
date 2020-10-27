#include "pool.hpp"

namespace mc
{
auto ThreadPool::GlobalInstance() -> ThreadPool&
{
    static ThreadPool instance {8};
    return instance;
}

}  // namespace mc