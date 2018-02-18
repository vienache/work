#!/usr/bin/python

class Parent(object):
    
    def __init__(self, name):
        self.name = name
    
    def foo(self, arg):
        print('Parent foo [' + arg + ']')
        

class Child(Parent):

    kind = 'child'         # class variable shared by all instances

    def __init__(self, name):
        self.name = name    # instance variable unique to each instance
        
        print('created Child[' + name + ']')

    def foo(self, arg):
        super(Child, self).foo(arg)
        
        print('Child foo [' + arg + ']')


c = Child('foo')
c.kind = 'kind foo'

c.foo('goo')
