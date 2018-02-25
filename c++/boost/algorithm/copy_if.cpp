#include <boost/algorithm/cxx11/copy_if.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

int main( int argc, char *argv[] )
{
  std::array<int, 10> a = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> result;

  boost::algorithm::copy_if( a.begin(), a.end(), std::back_inserter( result ),
  []( int i ) { return ( i % 2 == 1 ); } );

  std::copy( result.begin(), result.end(),
             std::ostream_iterator<int>( std::cout, " " ) );
  std::cout << '\n';
}
