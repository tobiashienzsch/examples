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
    namespace abs_if = etl::linalg::detail::abs_if_needed_detail;

    assert(not abs_if::has_abs<unsigned char>);
    assert(not abs_if::has_abs<unsigned short>);
    assert(not abs_if::has_abs<unsigned int>);
    assert(not abs_if::has_abs<unsigned long>);
    assert(not abs_if::has_abs<unsigned long long>);

    assert(abs_if::has_abs<signed int>);
    assert(abs_if::has_abs<signed long>);
    assert(abs_if::has_abs<signed long long>);

    assert(abs_if::has_abs<float>);
    assert(abs_if::has_abs<double>);
    assert(abs_if::has_abs<long double>);

    assert(not abs_if::has_abs<ns::my_complex>);  // no overload defined
    assert(abs_if::has_abs<std::complex<float>>);
    assert(abs_if::has_abs<std::complex<double>>);
    assert(abs_if::has_abs<std::complex<long double>>);

    namespace conj_if = etl::linalg::detail::conj_if_needed_detail;

    assert(not conj_if::has_conj<signed char>);
    assert(not conj_if::has_conj<short>);
    assert(not conj_if::has_conj<int>);
    assert(not conj_if::has_conj<long>);
    assert(not conj_if::has_conj<long long>);

    assert(not conj_if::has_conj<signed char>);
    assert(not conj_if::has_conj<unsigned short>);
    assert(not conj_if::has_conj<unsigned int>);
    assert(not conj_if::has_conj<unsigned long>);
    assert(not conj_if::has_conj<unsigned long long>);
    assert(not conj_if::has_conj<int>);

    assert(not conj_if::has_conj<float>);
    assert(not conj_if::has_conj<double>);
    assert(not conj_if::has_conj<long double>);

    assert(conj_if::has_conj<ns::my_complex>);
    assert(conj_if::has_conj<std::complex<float>>);
    assert(conj_if::has_conj<std::complex<double>>);
    assert(conj_if::has_conj<std::complex<long double>>);

    namespace real_if = etl::linalg::detail::real_if_needed_detail;

    assert(not real_if::has_real<signed char>);
    assert(not real_if::has_real<short>);
    assert(not real_if::has_real<int>);
    assert(not real_if::has_real<long>);
    assert(not real_if::has_real<long long>);

    assert(not real_if::has_real<signed char>);
    assert(not real_if::has_real<unsigned short>);
    assert(not real_if::has_real<unsigned int>);
    assert(not real_if::has_real<unsigned long>);
    assert(not real_if::has_real<unsigned long long>);
    assert(not real_if::has_real<int>);

    assert(not real_if::has_real<float>);
    assert(not real_if::has_real<double>);
    assert(not real_if::has_real<long double>);

    assert(real_if::has_real<ns::my_complex>);
    assert(real_if::has_real<std::complex<float>>);
    assert(real_if::has_real<std::complex<double>>);
    assert(real_if::has_real<std::complex<long double>>);

    namespace imag_if = etl::linalg::detail::imag_if_needed_detail;

    assert(not imag_if::has_imag<signed char>);
    assert(not imag_if::has_imag<short>);
    assert(not imag_if::has_imag<int>);
    assert(not imag_if::has_imag<long>);
    assert(not imag_if::has_imag<long long>);

    assert(not imag_if::has_imag<signed char>);
    assert(not imag_if::has_imag<unsigned short>);
    assert(not imag_if::has_imag<unsigned int>);
    assert(not imag_if::has_imag<unsigned long>);
    assert(not imag_if::has_imag<unsigned long long>);
    assert(not imag_if::has_imag<int>);

    assert(not imag_if::has_imag<float>);
    assert(not imag_if::has_imag<double>);
    assert(not imag_if::has_imag<long double>);

    assert(imag_if::has_imag<ns::my_complex>);
    assert(imag_if::has_imag<std::complex<float>>);
    assert(imag_if::has_imag<std::complex<double>>);
    assert(imag_if::has_imag<std::complex<long double>>);

    return true;
}

auto main() -> int
{
    assert(test());
    static_assert(test());
    return EXIT_SUCCESS;
}