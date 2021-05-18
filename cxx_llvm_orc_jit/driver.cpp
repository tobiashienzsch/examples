#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace orc = llvm::orc;
namespace fs  = std::filesystem;

int main(int argc, char** argv)
{
    auto const cwd = fs::current_path();
    std::cout << cwd << '\n';

    if (argc != 2)
    {
        std::cerr << "Need input source file name. No extension!\n";
        return EXIT_FAILURE;
    }

    auto const source          = cwd / std::string(argv[1]);
    auto const* compiler       = "clang++-10 -std=c++17 ";
    auto const* perf_flags     = "-O3 -mavx2 -flto -fno-strict-aliasing ";
    auto const* compiler_flags = "-S -emit-llvm -fno-exceptions -fno-rtti ";

    auto cmd = std::string();
    cmd.append(compiler);
    cmd.append(compiler_flags);
    cmd.append(perf_flags);
    cmd.append(source.string());
    cmd.append(".cpp -o ");
    cmd.append(source.string());
    cmd.append(".ll");

    std::cout << "Running: " << cmd << '\n';
    if (auto code = std::system(cmd.c_str()); code != 0)
    {
        std::cerr << "Failed to run clang++: " << code / 256 << '\n';
        std::cerr << cmd << '\n';
        return EXIT_FAILURE;
    }

    auto init = llvm::InitLLVM(argc, argv);
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    auto target = orc::JITTargetMachineBuilder::detectHost();
    if (!target)
    {
        llvm::outs() << target.takeError() << '\n';
        return EXIT_FAILURE;
    }

    auto builder = orc::LLJITBuilder();
    builder.setJITTargetMachineBuilder(target.get());
    auto expectedJit = builder.create();
    if (!expectedJit)
    {
        llvm::outs() << expectedJit.takeError() << '\n';
        return EXIT_FAILURE;
    }

    auto& jit     = expectedJit.get();
    auto& libMain = jit->getMainJITDylib();

    llvm::SMDiagnostic diagnostics = {};
    llvm::LLVMContext context      = {};
    auto mod = llvm::parseIRFile(source.string().append(".ll"), diagnostics,
                                 context);
    if (mod == nullptr)
    {
        llvm::outs() << "could not create module" << '\n';
        return EXIT_FAILURE;
    }

    auto tsm = orc::ThreadSafeModule(std::move(mod),
                                     std::make_unique<llvm::LLVMContext>());
    if (auto err = jit->addIRModule(libMain, std::move(tsm)); err)
    {
        llvm::outs() << err << '\n';
        return EXIT_FAILURE;
    }

    if (auto lookup = jit->lookup("foo"); !lookup)
    {
        llvm::outs() << lookup.takeError() << '\n';
        return EXIT_FAILURE;
    }
    else
    {
        using FooType = int (*)(int);
        auto* foo     = reinterpret_cast<FooType>(lookup->getAddress());
        std::cout << foo(41) << '\n';
    }

    if (auto lookup = jit->lookup("apply_gain"); !lookup)
    {
        llvm::outs() << lookup.takeError() << '\n';
        return EXIT_FAILURE;
    }
    else
    {
        using GainType = void (*)(double, double*, int);
        auto buffer    = std::array<double, 64> {};
        buffer.fill(0.95);
        auto* apply_gain = reinterpret_cast<GainType>(lookup->getAddress());
        apply_gain(0.75, buffer.data(), buffer.size());
        std::cout << buffer[0] << '\n';
    }

    return EXIT_SUCCESS;
}
