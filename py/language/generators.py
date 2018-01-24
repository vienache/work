#!/usr/bin/python

def reverse(data):
    for index in range(len(data)-1, -1, -1):
        yield data[index]

list = [1, 2, 3]

for i in list:
    print(i),

print('')

for i in reverse(list):
    print(i),
