#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

int main()
{
  std::string s = "Boost C++ Libraries";
  std::cout << boost::algorithm::to_upper_copy( s ) << std::endl;

  boost::algorithm::to_lower( s );
  std::cout << s << std::endl;
}
