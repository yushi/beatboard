all: bbirc.so

bbirc.so: Makefile libbbirc_wrap.cxx
	make

Makefile: libbbirc_wrap.cxx
	ruby extconf.rb

libbbirc_wrap.cxx:
	swig -c++ -ruby libbbirc.i
	
clean:
	rm -rf bbirc.so libbbirc_wrap.cxx  libbbirc_wrap.o  Makefile
