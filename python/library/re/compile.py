#!/usr/bin/python

import re

def checkMatch(string, pattern):
    prog = re.compile(pattern)
    match = prog.match(string)
    if match is None:
        print "Not matched. String='", string, "' pattern='", pattern, "'"
    else:
        print "It matched. String='", string, "' pattern='", pattern, "'"

checkMatch("string", "[0-9a-z]*")
checkMatch("string", "([0-9]*|[a-z]*)")
checkMatch("string", "^[0-9][a-z]*")
checkMatch("1string", "[0-9][a-z]*")
