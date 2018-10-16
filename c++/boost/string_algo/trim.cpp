#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

int main()
{
  {
    std::string s = "\t Boost C++ Libraries \t";
    std::cout << "_" << boost::algorithm::trim_left_copy( s ) << "_\n";
    std::cout << "_" << boost::algorithm::trim_right_copy( s ) << "_\n";
    std::cout << "_" << boost::algorithm::trim_copy( s ) << "_\n";
  }

  {
    std::string s = "*--Boost C++ Libraries--*";
    std::cout << boost::algorithm::trim_left_copy_if( s, boost::algorithm::is_any_of( "-*" ) ) << '\n';
    std::cout << boost::algorithm::trim_right_copy_if( s, boost::algorithm::is_any_of( "-*" ) ) << '\n';
    std::cout << boost::algorithm::trim_copy_if( s, boost::algorithm::is_any_of( "-*" ) ) << '\n';
  }
}
