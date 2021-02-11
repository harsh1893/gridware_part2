
# C++ wrapper compiler
CCPP = g++

# Python3 compiler
CP = python3
# protoc generator
CPC = protoc

# C/C++ Inc flags
CFLAGS = -I . 						# include base project folder
CFLAGS += -I nanopb/				# include nanopb lib
CFLAGS += -I protobuf/  			# include compiled protobuf 
CFLAGS += -I cpp_wrapper/  			# include c++ wrapper 


# C/C++ source code files that are required
CSRC  = cpp_wrapper/comm.cpp        # The C++ wrapper
CSRC += protobuf/gridware.pb.c      # The compiled protocol definition
CSRC += nanopb/pb_encode.c  		# The nanopb encoder
CSRC += nanopb/pb_decode.c  		# The nanopb decoder
CSRC += nanopb/pb_common.c  		# The nanopb common parts

# Output compiled location/flag
COP   = -o bin/comm.o  				# C++ compiled file with flag
POP   = build/gridware.pb 			# protoc generated file
NPBOP = -obuild/gridware.pb			# protoc generated file with flag

# proto src file
PSRC = protobuf/gridware.proto
PSRC += nanopb.proto 

#proto inc file
PINC = -I=.
PINC += -I=nanopb/generator/proto/


# Misc Location

NPBGEN = nanopb/generator/nanopb_generator.py 

project:
	@ $(CPC) $(PINC)  $(NPBOP) $(PSRC)
	@ $(CP) $(NPBGEN) $(POP)   
	@ $(CCPP) $(CFLAGS) $(CSRC) $(COP)
	@ echo "Done."
	
	
	


	
