#!/usr/bin/python

import os, fnmatch
import sys, getopt

def findReplace(directory, find, replace, filePattern):
    for path, dirs, files in os.walk(os.path.abspath(directory)):
        for filename in fnmatch.filter(files, filePattern):
            filepath = os.path.join(path, filename)
            with open(filepath) as f:
                s = f.read()
            s = s.replace(find, replace)
            with open(filepath, "w") as f:
                f.write(s)

def main(argv):
   searchstring = ''
   replacestring = ''
   try:
      opts, args = getopt.getopt(argv,"hs:r:",["search=","replace="])
   except getopt.GetoptError:
      print 'findReplace.py -s <search> -r <replace>'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'findReplace.py -s <search> -r <replace>'
         sys.exit()
      elif opt in ("-s", "--search"):
         searchstring = arg
      elif opt in ("-r", "--replace"):
         replacestring = arg
   print 'Search string "', searchstring, '"'
   print 'Replace with string "', replacestring, '"'
   findReplace(".", searchstring, replacestring, '*')

if __name__ == "__main__":
   main(sys.argv[1:])