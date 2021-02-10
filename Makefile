
project:
	python3 nanopb/generator/nanopb_generator.py build/gridware.pb 
	protoc -I=. -I=nanopb/generator/proto/ -obuild/gridware.pb protobuf/gridware.proto nanopb.proto 

	
	
	


	
