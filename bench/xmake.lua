add_requires("benchmark", {alias = "benchmark"})

target("bench_comparison")
    set_kind("binary")
    add_includedirs("../include", {public = true})
    add_files("bench_comparison.cpp")
    add_packages("benchmark")