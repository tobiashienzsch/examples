#ifdef _MSC_VER
#define WIN32 1
#else
#define MACOSX 1
#endif

// clang-format off
#include "immintrin.h"
#include "XHelpers.h"
#include "XDSP.h"
#include "MathOps_Common.h"
#include "XIOAdapter.h"
#include "YBasicAmp.h"
#include "YEQFilter.h"
// clang-format on

#define MathOps_AVX_v MathOps_AVX2
#include "MathOps_AVX.h"

void process(YEQFilter<1>::Node* node,
             const XDSP::ProcessGlobals& process_globals)
{
    XDSP::NodeTmpl<YEQFilter<1>>::template ProcessAllVoices<
        typename IOAdapter<MathOps_AVX2<4>, YEQFilter<1>>::Worker>(
        process_globals, node);
}

auto main() -> int { return 0; }