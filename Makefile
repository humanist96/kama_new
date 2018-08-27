SWIG=swig
PREFIX=kama
LIB_PATH=.
INSTALL_PATH=.

all: cpp python install

clean:
	rm lib lib64 build *.cxx -rf
	rm $(PREFIX).py $(PREFIX) -rf
	
python:	
	$(SWIG) -Wall -python -shadow -c++ $(PREFIX).i
	python setup.py build
	
cpp:
	gcc -Wall -ggdb -o $(PREFIX) $(PREFIX).cpp -lmecab -lstdc++ -I$(LIB_PATH)

install:
	python setup.py install --prefix=$(INSTALL_PATH)
	
