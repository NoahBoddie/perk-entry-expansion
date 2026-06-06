-- set required xmake version
set_xmakever("2.8.2")
includes("lib/commonlibsse-ng")

-- setup
set_project("PerkEntryExpansion")
set_version("2.0.0")
set_license("Apache-2.0")

-- defaults
set_languages("c++23")
set_warnings("allextra")

-- xmake rules
add_rules("mode.debug", "mode.releasedbg")
set_defaultmode("releasedbg")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
set_config("skse_xbyak", true)
add_requires("zydis", "magic_enum")

target("PerkEntryExpansion")
    add_deps("commonlibsse-ng")
    add_packages("zydis", "magic_enum")
    add_rules("commonlibsse-ng.plugin", {
        name = "PerkEntryExpansion",
        author = "noah and styyx",
        description = "An SKSE plugin that adds a perk entry point library for other mods to use."
    })

    add_files("src/*.cpp")
    add_headerfiles("src/**.h", "src/**.hpp")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")