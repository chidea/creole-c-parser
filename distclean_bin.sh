#!/bin/sh
#
# Make clean for binary distribution.
#

rm -f ./bin/creoled.*
rm -f ./bin/parsertest*.*

rm -rf ./doc/api/html
rm -rf ./doc/impl/html

rm -f ./lib/creoled.lib
rm -f ./lib/creolelibd.lib

rm -f ./test/data/*.html

rm -rf ./playground/csharp/converter/bin
rm -rf ./playground/csharp/converter/obj
rm -f ./playground/csharp/converter/converter.csproj.*
rm -f ./playground/csharp/converter/converter.suo

