#include <cstdio>
#include <cstdlib>

#include <cassert>
#include <iostream>

#include "dump.hpp"

namespace sml = boost::sml;

struct Booted
{
};

struct NetworkUp
{
    std::string ip = {};
};

struct SessionActive
{
};
struct SessionIdle
{
};

struct DhcpTimeout
{
};
struct DhcpLease
{
};
struct RouterTimeout
{
};
struct RouterConnection
{
};
struct Mapping
{
};
struct ControlChange
{
};
struct CloseSession
{
};

struct IsValidMapping
{
    [[nodiscard]] constexpr auto operator()() const noexcept -> bool { return true; }
};

struct IsInvalidMapping
{
    [[nodiscard]] constexpr auto operator()() const noexcept -> bool { return false; }
};

struct ApplyMapping
{
    void operator()() noexcept { }
};

struct StateMachine
{

    using Self = StateMachine;
    auto operator()() const noexcept
    {
        using namespace boost::sml;

        // clang-format off
        return make_transition_table(
            *state<Booted>         + event<DhcpTimeout>                                = state<Booted>
            ,state<Booted>         + event<DhcpLease>                                  = state<NetworkUp>
            ,state<NetworkUp>      + event<RouterTimeout>                              = state<NetworkUp>
            ,state<NetworkUp>      + event<RouterConnection>                           = state<SessionIdle>
            ,state<SessionIdle>    + event<Mapping>[IsInvalidMapping{}]                = state<SessionIdle>
            ,state<SessionIdle>    + event<Mapping>[IsValidMapping{}] / ApplyMapping{} = state<SessionActive>
            ,state<SessionActive>  + event<ControlChange>                              = state<SessionActive>
            ,state<SessionActive>  + event<CloseSession>                               = state<SessionIdle>
        );
        // clang-format on
    }

    std::string version = "0.1.0";
};

int main()
{
    auto bootedState        = Booted {};
    auto networkUpState     = NetworkUp {};
    auto sessionIdleState   = SessionIdle {};
    auto sessionActiveState = SessionActive {};
    auto globalData         = StateMachine {};
    auto sm                 = boost::sml::sm<StateMachine> {
        globalData, bootedState, networkUpState, sessionIdleState, sessionActiveState,
    };
    sm.process_event(DhcpLease {});
    sm.process_event(RouterConnection {});
    sm.process_event(Mapping {});

    th::dump(sm);
}