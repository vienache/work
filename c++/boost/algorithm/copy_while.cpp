#include <boost/algorithm/cxx11/copy_if.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

int main( int argc, char *argv[] )
{
  std::array<int, 5> a = {0, 1, 2, 3, 4};
  std::vector<int> result;

  boost::algorithm::copy_while( a, std::back_inserter( result ),
  []( int i ) { return i <= 2; } );

  std::copy( result.begin(), result.end(),
             std::ostream_iterator<int>( std::cout, " " ) );
  std::cout << '\n';
}
