#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>

int main()
{
  std::string s = "Boost C++ Libraries";
  std::vector<std::string> v;
  boost::algorithm::split( v, s, boost::algorithm::is_space() );
  std::cout << v.size() << '\n';

  for ( const auto &it : v )
  {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
}
