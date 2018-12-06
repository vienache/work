#!/bin/bash

sed s/day/night/ old.txt >new.txt

echo '=>Replace string with another'
echo day | sed s/day/night/ #night

echo '=>Using & as the matched string'
echo "123 abc" | sed 's/[0-9]*/(&)/' #(123) abc
echo '=>same... enable extended regex (-r)'
echo "123 abc" | sed -r 's/[0-9]+/(&)/' #(123) abc

echo '=>Sed rememberd patterns (\1..\9)'
echo "abcd123" | sed 's/\([a-z]*\).*/\1/' #abcd

echo '=>Multiple commands with -e command'
sed -e 's/a/A/g' \
    -e 's/e/E/g' \
    -e 's/i/I/g' \
    -e 's/o/O/g' \
    -e 's/u/U/g'  old.txt >new.txt
cat new.txt