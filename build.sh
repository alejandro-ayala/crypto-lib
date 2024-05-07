#!/bin/bash
tests_dir="./test"
lib_dir="./crypt_lib"
cmd_app_dir="./cmd_line_app"
common_dir="./common"
ouput_dir="./out"

compile_cmd_app() {
    echo "Building console line application"
    compile_library
    cp $ouput_dir/libcrypt.so .
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH: &&
    gcc -I./ -o $ouput_dir/cmd_cryp_app $cmd_app_dir/main.c $cmd_app_dir/cmd_line_helper.c -L. -lcrypt
    if [ $? -eq 0 ]; then
        echo "cmd_app compiled"
    else
        echo "Error during cmd_app build process"
    fi

}

compile_tests() {
    echo "Building tests_automatics"
    compile_library
    cp $ouput_dir/libcrypt.so .
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH: &&
    gcc -I.$tests_dir/acutest/include/ -I./ -o $ouput_dir/automatic_test $tests_dir/automatic_test.c $tests_dir/acutest/include/acutest.h $cmd_app_dir/cmd_line_helper.c -L. -lcrypt
    if [ $? -eq 0 ]; then
        echo "tests_automatics compiled"
    else
        echo "Error during tests_automatics build process"
    fi

}

compile_library() {
    echo "Building libcrypt"
    gcc -shared -o $ouput_dir/libcrypt.so $lib_dir/crypt.c -I./
    if [ $? -eq 0 ]; then
        echo "libcrypt compiled"
    else
        echo "Error during libcrypt build process"
    fi
}

show_help() {
    echo "Usage: $0 [tests | lib | cmd_app]"
    echo
    echo "Arguments:"
    echo "  cmd_app   : Compile the cmd_app application.The output is the binary: \"cmd_app\""    
    echo "  lib       : Compile the crypto library.The output is the library: \"libcrypt.so\""       
    echo "  tests     : Compile the automatic tests. The output is the binary: \"automatic_test\""
    exit 1
}

mkdir -p $ouput_dir
if [ $# -eq 0 ]; then
    echo "Building all targets"
    compile_library
    compile_tests
    compile_cmd_app    
    exit 1
fi

case "$1" in
    "tests")
        compile_tests
        ;;
    "lib")
        compile_library
        ;;
    "cmd_app")
        compile_cmd_app
        ;;
    "-h")
        show_help
        ;;        
    *)
	echo "Incorrect arguments"
	show_help
        exit 1
        ;;
esac

exit 0

