# C++ wrapper compiler
CCPP = g++
# Python3 compiler
CPY = python3


# C/C++ Inc flags
CFLAGS = -I . 						# include base project folder
CFLAGS += -I nanopb/				# include nanopb lib
CFLAGS += -I protobuf/  			# include compiled protobuf 


# Output compiled location/flag
COP   = -o bin/comm.o  				# C++ compiled file with flag
TOP   = -o bin/api_test.o

# proto src file
PSRC = protobuf/gridware.proto
PSRC += nanopb.proto 

#proto inc file
INC_P = -I=.
INC_P += -I=nanopb/generator/proto/

#.PB file generater
PBGEN = protoc $(INC_P) -obuild/gridware.pb $(PSRC)

#.PB file location
PBFILE   = build/gridware.pb

# Misc Location/flag
NPBGEN = nanopb/generator/nanopb_generator.py 
THREAD = -pthread

#Compiled files location
BUILT_DIR = build/*.o



project:
	@ $(PBGEN)
	@ $(CPY) $(NPBGEN) $(PBFILE) 
	@ $(CCPP)  $(CFLAGS)  -c protobuf/gridware.pb.c   -o build/gridware.pb.o
	@ $(CCPP) $(CFLAGS)   -c nanopb/pb_common.c -o build/pb_common.o
	@ $(CCPP)  $(CFLAGS)  -c nanopb/pb_decode.c   -o build/pb_decode.o
	@ $(CCPP)  $(CFLAGS)  -c nanopb/pb_encode.c   -o build/pb_encode.o
	@ $(CCPP)  $(CFLAGS)  -I cpp_wrapper/ -c cpp_wrapper/comm.cpp   $(COP) 
	@ echo "Project built."

test:project
	@ $(CCPP) $(CFLAGS) -I cpp_wrapper/ -I test/ $(BUILT_DIR) $(THREAD) test/api_test.cpp $(TOP)
	@ echo "Test Built"
clean:
	rm -f bin/*.o build/* protobuf/gridware.pb.c protobuf/gridware.pb.h  
 	
	


	
