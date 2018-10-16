#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>

int main()
{
  std::vector<std::string> v{"Boost", "C++", "Libraries"};
  std::cout << boost::algorithm::join( v, " " ) << '\n';
}
