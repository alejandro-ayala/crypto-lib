
Build lib to link dinamically: gcc -shared -o libcrypt.so crypt.c
Build cmd line app and link dinamically with the library:
    Build: gcc -o cmd_cryp_app main.c cmd_line_helper.c -L. -lcrypt
    Add path to LD_LIBRARY_PATH: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:
Build UTs: gcc -I./acutest/include/ -I../ -o automatic_test automatic_test.c acutest/include/acutest.h ../cmd_line_helper.c -L. -lcrypt
