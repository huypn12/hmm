#!/bin/sh


function clean {
    rm -rf build/"$1"
    rm compile_commands.json
}

function gen_cmake_command {
    cmake_cmd="cmake -H. -GNinja"
    cmake_cmd="$cmake_cmd -Bbuild/$1"
    cmake_cmd="$cmake_cmd -DCMAKE_BUILD_TYPE=$1"
    if [ ! -z "$2" ]; then
        cmake_cmd="$cmake_cmd -DCMAKE_PREFIX_PATH=$2"
    fi
    cmake_cmd="$cmake_cmd -DCMAKE_EXPORT_COMPILE_COMMANDS=Yes"
}

function link_compile_command {
    compile_commands_file="build/$1/compile_commands.json"
    ln -s "$compile_commands_file"
}

function gen_ccls {
    ccls="%compile_commands.json \n"
    ccls="%cpp %hpp -std=gnu++17 -stdlib=libstdc++ \n"
    for i in "${@}"; do
        modules_path="$ccls -I"
    done
}

function get_opts {
    if [ "$1" = "debug" ]; then
        build_type="Debug"
    else
        build_type="Release"
    fi

    modules_path="$2"
    for i in "${@:3}"; do
        modules_path="$modules_path\;$i"
    done
}

get_opts "${@}"
echo "Build type config: $build_type"
echo "CMake libraries search path: $modules_path"


clean "$build_type"
echo "Project cleaned."

echo "Generating CMake command"
gen_cmake_command "$build_type" "$modules_path"
echo "CMake command: $cmake_cmd"
echo "Generating CMake build directory..."
eval "$cmake_cmd"
echo "Generating .ccls project config"
link_compile_command "$build_type"
echo "Generated compile command:"
cat compile_commands.json
echo ""
echo "Done."
