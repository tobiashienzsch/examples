#include <etl/linalg.hpp>

#undef NDEBUG
#include <cassert>

namespace ns {
struct my_complex
{
    float real;
    float imag;
};

static auto real(my_complex const& cplx) -> float { return cplx.real; }

static auto imag(my_complex const& cplx) -> float { return cplx.imag; }

static auto conj(my_complex const& cplx) -> my_complex { return {cplx.real, -cplx.imag}; }

}  // namespace ns

static constexpr auto test() -> bool
{
    namespace detail = etl::linalg::detail;

    assert(not detail::has_adl_abs<unsigned char>);
    assert(not detail::has_adl_abs<unsigned short>);
    assert(not detail::has_adl_abs<unsigned int>);
    assert(not detail::has_adl_abs<unsigned long>);
    assert(not detail::has_adl_abs<unsigned long long>);

    assert(detail::has_adl_abs<signed int>);
    assert(detail::has_adl_abs<signed long>);
    assert(detail::has_adl_abs<signed long long>);

    assert(detail::has_adl_abs<float>);
    assert(detail::has_adl_abs<double>);
    assert(detail::has_adl_abs<long double>);

    assert(not detail::has_adl_abs<ns::my_complex>);  // no overload defined
    assert(detail::has_adl_abs<std::complex<float>>);
    assert(detail::has_adl_abs<std::complex<double>>);
    assert(detail::has_adl_abs<std::complex<long double>>);

    assert(not detail::has_adl_conj<signed char>);
    assert(not detail::has_adl_conj<short>);
    assert(not detail::has_adl_conj<int>);
    assert(not detail::has_adl_conj<long>);
    assert(not detail::has_adl_conj<long long>);

    assert(not detail::has_adl_conj<signed char>);
    assert(not detail::has_adl_conj<unsigned short>);
    assert(not detail::has_adl_conj<unsigned int>);
    assert(not detail::has_adl_conj<unsigned long>);
    assert(not detail::has_adl_conj<unsigned long long>);
    assert(not detail::has_adl_conj<int>);

    assert(not detail::has_adl_conj<float>);
    assert(not detail::has_adl_conj<double>);
    assert(not detail::has_adl_conj<long double>);

    assert(detail::has_adl_conj<ns::my_complex>);
    assert(detail::has_adl_conj<std::complex<float>>);
    assert(detail::has_adl_conj<std::complex<double>>);
    assert(detail::has_adl_conj<std::complex<long double>>);

    assert(not detail::has_adl_real<signed char>);
    assert(not detail::has_adl_real<short>);
    assert(not detail::has_adl_real<int>);
    assert(not detail::has_adl_real<long>);
    assert(not detail::has_adl_real<long long>);

    assert(not detail::has_adl_real<signed char>);
    assert(not detail::has_adl_real<unsigned short>);
    assert(not detail::has_adl_real<unsigned int>);
    assert(not detail::has_adl_real<unsigned long>);
    assert(not detail::has_adl_real<unsigned long long>);
    assert(not detail::has_adl_real<int>);

    assert(not detail::has_adl_real<float>);
    assert(not detail::has_adl_real<double>);
    assert(not detail::has_adl_real<long double>);

    assert(detail::has_adl_real<ns::my_complex>);
    assert(detail::has_adl_real<std::complex<float>>);
    assert(detail::has_adl_real<std::complex<double>>);
    assert(detail::has_adl_real<std::complex<long double>>);

    assert(not detail::has_adl_imag<signed char>);
    assert(not detail::has_adl_imag<short>);
    assert(not detail::has_adl_imag<int>);
    assert(not detail::has_adl_imag<long>);
    assert(not detail::has_adl_imag<long long>);

    assert(not detail::has_adl_imag<signed char>);
    assert(not detail::has_adl_imag<unsigned short>);
    assert(not detail::has_adl_imag<unsigned int>);
    assert(not detail::has_adl_imag<unsigned long>);
    assert(not detail::has_adl_imag<unsigned long long>);
    assert(not detail::has_adl_imag<int>);

    assert(not detail::has_adl_imag<float>);
    assert(not detail::has_adl_imag<double>);
    assert(not detail::has_adl_imag<long double>);

    assert(detail::has_adl_imag<ns::my_complex>);
    assert(detail::has_adl_imag<std::complex<float>>);
    assert(detail::has_adl_imag<std::complex<double>>);
    assert(detail::has_adl_imag<std::complex<long double>>);

    return true;
}

auto main() -> int
{
    assert(test());
    static_assert(test());
    return EXIT_SUCCESS;
}