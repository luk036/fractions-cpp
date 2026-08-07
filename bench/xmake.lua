add_requires("nanobench")

target("bench_comparison")
    set_kind("binary")
    add_includedirs("../include", {public = true})
    add_files("bench_comparison.cpp")
    add_packages("nanobench")
