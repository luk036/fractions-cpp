set_languages("c++20")
add_repositories("local-repo", "$(projectdir)/packages")
includes("bench")

add_rules("mode.debug", "mode.release", "mode.coverage")
add_requires("doctest", "fmt", "spdlog")

if is_mode("coverage") then
    add_cxflags("-ftest-coverage", "-fprofile-arcs", {force = true})
end

if is_plat("linux") then
    set_warnings("all")
    add_cxflags("-Wconversion", {force = true})
    -- Termux/Android ships its libc++ headers under $PREFIX
    local prefix = os.getenv("PREFIX")
    if prefix then
        add_cxflags("-Wno-unused-command-line-argument", {force = true})
        add_sysincludedirs(prefix .. "/include/c++/v1", prefix .. "/include", {public = true})
    end
elseif is_plat("windows") then
    add_cxflags("/utf-8 /W4 /WX /wd4702", {force = true})
end

target("test_frac")
    add_includedirs("include", {public = true})
    add_files("test/source/*.cpp")
    add_packages("doctest", "fmt", "spdlog")
    add_tests("default")

    -- rapidcheck is optionally provided by a CMake build (build_test first, then build)
    local rapidcheck_dir = path.join(os.projectdir(), "build_test", "_deps", "rapidcheck-src")
    local rapidcheck_lib_dir = path.join(os.projectdir(), "build_test", "_deps", "rapidcheck-build")
    if is_plat("windows") then
        rapidcheck_lib_dir = path.join(rapidcheck_lib_dir, "Release")
    end

    if not os.isdir(rapidcheck_dir) then
        rapidcheck_dir = path.join(os.projectdir(), "build", "_deps", "rapidcheck-src")
        rapidcheck_lib_dir = path.join(os.projectdir(), "build", "_deps", "rapidcheck-build")
        if is_plat("windows") then
            rapidcheck_lib_dir = path.join(rapidcheck_lib_dir, "Release")
        end
    end

    if os.isdir(rapidcheck_dir) and os.isdir(rapidcheck_lib_dir) then
        add_includedirs(path.join(rapidcheck_dir, "include"))
        add_linkdirs(rapidcheck_lib_dir)
        add_links("rapidcheck")
        add_defines("RAPIDCHECK_H")
    end
