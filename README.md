# creole-c-parser
Wiki Creole parser and HTML converter based on [creole/c](http://creolec.sourceforge.net/)

## main difference
This source is based on [creole/c](http://creolec.sourceforge.net/) and edited for with features below.
- [x] Fully compilable on linux
  - The original creole/c had some missing headers and permission issues on linux systems.
- [x] Supports input from standard input
  - use `-` as input file name to read from stdin

## how to build on linux
```
git clone https://github.com/chidea/creole-c-parser.git
mkdir build-creole
cd build-creole
cmake -G 'Unix Makefiles' ../creole-c-parser
make
sudo make install
```
