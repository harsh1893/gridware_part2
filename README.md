
## Library version used in the project:
1. Protobuf: v3.14.0
2. nanopb: v0.4.4


## Support Library Installation 
1. Create a new directory say gridware
2. Follow instruction to install protobuf c++ support https://github.com/protocolbuffers/protobuf/blob/master/src/README.md
3. Python3 tools- run the command "sudo apt-get install python3-distutils python3-setuptools python3-dev" on the terminal.
4. Follow instruction to install protobuf python support https://github.com/protocolbuffers/protobuf/tree/master/python/README.md


## Repository instruction
1. Open terminal and run following command "git clone https://github.com/harsh1893/gridware_part2.git"
2. then run "git submodule update --init --recursive"


## Testing the API
Inside the project directory, run "make test"
then run "./bin/api_test.o" 

## Compiling API
Inside the project directory, run "make project"

## Cleaning compiled files
Inside the project directory, run "make clean"

## Repository Structure
- GRIDWARE_PART2
-- bin : contains compiled file 
-- build : contains .pb file
-- cpp_wrapper : contains .cpp/.h files for APIs
-- nanopb : nanopb library
-- protobuf : .proto file and its generated.c/.h compiled files
-- test:  .cpp/.h files to test APIs


