#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

AddOption('--tests_enabled', nargs=0)



# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

def generate(env):
    # Some stuff that improves build performance according to: https://github.com/SCons/scons/wiki/GoFastButton
    env.Decider("content-timestamp")
    env.SetOption("max_drift", 1)
    env.SetOption("implicit_cache", 1)

    # Enable C++20
    if env.get("is_msvc", False):
        env["CXXFLAGS"].remove("/std:c++17")
        env["CXXFLAGS"].insert(0, "/std:c++20")
        env["CXXFLAGS"].insert(0, "/Zc:preprocessor")
    else:
        env["CXXFLAGS"].remove("-std=c++17")
        env["CXXFLAGS"].insert(0, "-std=c++20")

generate(env)

if GetOption('tests_enabled') != None:
    env.Append(CPPDEFINES=["TESTS_ENABLED"])

# tweak this if you want to use different folders, or more folders, to store your source code in.
# Edit: Added the specialsensors subfolder.
env.Append(CPPPATH=["tests/", "src/", "src/sensors", "src/considerations", "src/node_query_system", "src/node_query_system/search_spaces", "src/node_query_system/search_criteria", "src/node_query_system/search_spaces/point_grid", "src/behaviour_tree"])
sources = Glob("src/*.cpp")
sources += Glob("src/agent_behaviours/*.cpp")
sources += Glob("src/behaviour_tree/*.cpp")
sources += Glob("src/debugger/*.cpp")
sources += Glob("src/editor/*.cpp")
sources += Glob("src/node_query_system/*.cpp")
sources += Glob("src/resources/*.cpp")
sources += Glob("src/state_tree/*.cpp")
sources += Glob("src/*/*/*.cpp")
sources += Glob("src/*/*/*/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/addons/utility_ai/bin/libutilityai.{}.{}.framework/libutilityai.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demo/addons/utility_ai/bin/libutilityai{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

#env.NoCache(library)
Default(library)
