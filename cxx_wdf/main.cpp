#include <array>

namespace mc
{
template<typename T>
struct State
{
    T Rp {1};  // Port resistance
    T a {0};   // incident wave (incoming wave)
    T b {0};   // reflected wave (outgoing wave)
};

template<typename T>
struct ElectricalComponent
{
    [[nodiscard]] auto voltage() noexcept
    {
        auto& state = static_cast<T*>(this)->getState();
        return (state.a + state.b) / 2.0;
    }

    [[nodiscard]] auto current() noexcept
    {
        auto& state = static_cast<T*>(this)->getState();
        return (state.a - state.b) / (state.Rp + state.Rp);
    }

    [[nodiscard]] auto R() noexcept
    {  // resistance
        auto& state = static_cast<T*>(this)->getState();
        return state.Rp;
    }
    [[nodiscard]] auto G() noexcept
    {  // conductance
        auto& state = static_cast<T*>(this)->getState();
        return 1.0 / state.Rp;
    }
};

template<typename T>
struct Resistor : ElectricalComponent<Resistor<T>>
{
    explicit Resistor(T R) noexcept : state_ {R, 0, 0} { }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void { state_.a = wave; }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = 0;
        return state_.b;
    }

private:
    State<T> state_ {};
};

template<typename T>
struct Capacitor : ElectricalComponent<Capacitor<T>>
{
    Capacitor(T C, T sampleRate) noexcept
        : state_ {sampleRate / T {2} * C, 0, 0}
    {
    }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void
    {
        state_.a  = wave;
        capState_ = state_.a;
    }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = capState_;
        return state_.b;
    }

private:
    State<T> state_ {};
    T capState_ {0};
};

template<typename T>
struct Inductor : ElectricalComponent<Inductor<T>>
{
    Inductor(T L, T sampleRate) noexcept : state_ {T {2} * L / sampleRate, 0, 0}
    {
    }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void
    {
        state_.a  = wave;
        capState_ = state_.a;
    }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = -capState_;
        return state_.b;
    }

private:
    State<T> state_ {};
    T capState_ {0};
};

template<typename T>
struct OpenCircuit : ElectricalComponent<OpenCircuit<T>>
{
    explicit OpenCircuit(T R) noexcept : state_ {R, 0, 0} { }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void { state_.a = wave; }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = state_.a;
        return state_.b;
    }

private:
    State<T> state_ {};
};

template<typename T>
struct ShortCircuit : ElectricalComponent<ShortCircuit<T>>
{
    explicit ShortCircuit(T R) noexcept : state_ {R, 0, 0} { }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void { state_.a = wave; }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = -state_.a;
        return state_.b;
    }

private:
    State<T> state_ {};
};

template<typename T>
struct VoltageSource : ElectricalComponent<VoltageSource<T>>
{
    VoltageSource(T V, T R) noexcept : state_ {R, 0, 0}, Vs_ {V} { }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void { state_.a = wave; }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = -state_.a + 2.0 * Vs_;
        return state_.b;
    }

private:
    State<T> state_ {};
    T Vs_;
};

template<typename T>
struct CurrentSource : ElectricalComponent<CurrentSource<T>>
{
    CurrentSource(T I, T R) noexcept : state_ {R, 0, 0}, Is_ {I} { }

    [[nodiscard]] auto getState() noexcept -> State<T>& { return state_; }

    auto incident(T wave) noexcept -> void { state_.a = wave; }

    [[nodiscard]] auto reflected() noexcept -> T
    {
        state_.b = state_.a + 2.0 * this->R() * Is_;
        return state_.b;
    }

private:
    State<T> state_ {};
    T Is_;
};

}  // namespace mc

auto resistor(mc::Resistor<double>& R, double in) -> double
{
    R.incident(in);
    return R.reflected();
}
auto capacitor(mc::Capacitor<double>& C, double in) -> double
{
    C.incident(in);
    return C.reflected();
}
auto inductor(mc::Inductor<double>& L, double in) -> double
{
    L.incident(in);
    return L.reflected();
}
auto open_circuit(mc::OpenCircuit<double>& OC, double in) -> double
{
    OC.incident(in);
    return OC.reflected();
}
auto short_circuit(mc::ShortCircuit<double>& SC, double in) -> double
{
    SC.incident(in);
    return SC.reflected();
}
auto voltage_source(mc::VoltageSource<double>& Vs, double in) -> double
{
    Vs.incident(in);
    return Vs.reflected();
}
auto current_source(mc::CurrentSource<double>& Is, double in) -> double
{
    Is.incident(in);
    return Is.reflected();
}
auto ideal_transformer(mc::CurrentSource<double>& Is, double in) -> double
{
    Is.incident(in);
    return Is.reflected();
}

auto main() -> int { return 0; }