#!/bin/bash
rm -f makefile

echo -e "CFLAGS = -std=c++11 -c -Wall \c">>makefile

find ./include -type d |while read fname; do
  echo -e "-I$fname/ \c">>makefile
done

echo -e "\n" >>makefile
find ./src -name "*.cpp" |while read fname; do
  echo "build${fname:5:-3}o : ${fname:2} include${fname:5:-3}h">>makefile
  echo -e "\tg++ ${fname:2} -o build${fname:5:-3}o \$(CFLAGS)">>makefile
  echo "">>makefile
done


echo -e "game : main.cpp \c">>makefile
find ./src -name "*.cpp" |while read fname; do
  echo -e "build${fname:5:-3}o \c">>makefile
  
done
echo "">>makefile
echo -e "\tg++ main.cpp \$(CFLAGS)">>makefile
echo "">>makefile