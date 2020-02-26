#include <iostream>
#include <string>
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>

int main()
{
   std::string msg( "www.google.com" );
   std::cout << msg << std::endl;
   boost::replace_all( msg, ".", "\\." );
   std::cout << msg << std::endl;
}
