# Project README

## Project Overview
This project consists of 3 independent modules, each corresponding to the project specifications:

### cmd_line_app
This program consists of two main components:
- **cmd_line_helper**: Responsible for parsing user-input arguments and configuring the execution.
- **file_handler**: Responsible for reading and writing data to input and/or output files of the system.

### test
For the implementation of automatic testing, the "acutest" framework has been utilized, included in the `test` folder. The source code of the tests is located in `automatic_test.c` and includes the following test cases:
1. check_input_key: 
2. check_key_size_valid: to check the valid size of the provided key according to the requirement. 
3. check_key_size_higher_max: to check the error when the key is higher than the maximum allowed.
4. check_key_size_lower_min: to check the error when the key is lower than the minimum allowed.
5. check_library_version: to validate that the API provides a method to read the current API version
6. automatic_test_sample: this test case perform automatically the provided test in the specifications. 
8. automatic_test_encrypt_and_decrypt: to check that reseting the status of the crypt_buffer() funtion the decode process is correct. 
9. automatic_test_encrypt_and_decrypt_fail: test to check that kepting the status of the crypt_buffer() funtion the decode process fails. 

### crypt_lib
This is the library that includes the implementation of the required algorithm. This library is dynamically linked with the other two executables of the project (`automatic_test` and `cmd_cryp_app`).

## Additional Files
Additionally, there are other files organized in the following folders:
- **common**: Includes the versioning file and header files with the constants used in the project.
- **resources**: Contains `.txt` files that contain information about the variables provided for automatic testing (`coded1[]` - `coded5[]` and `key.txt`), converted to ASCII format for use with the console application.
- **build.sh**: Script to automate the compilation process.

## Getting Started
To compile the project, simply run the `build.sh` script.

## Usage
To use the `cmd_line_app`, follow the syntax specified in the program's invocation. For example:

cmd_line_app [-h] [-v] -k <key> | -f <key_file> [-o <output_file>] [<input_file>]


## Testing
To run the automated tests, navigate to the `out` folder (which is generated during the build process with the build.sh script) and execute the test executable (automatic_test).

## Contributors
- Alejandro Ayala del Barco

## License
This project is licensed under the [MIT License](LICENSE).
