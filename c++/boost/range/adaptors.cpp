#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <array>
#include <map>
#include <string>
#include <utility>
#include <iterator>
#include <iostream>

int main( int argc, char *argv[] )
{
  std::array<int, 3> a {0, 1, 2};
  std::map<std::string, int *> m;
  m.insert( std::make_pair( "a", &a[0] ) );
  m.insert( std::make_pair( "b", &a[1] ) );
  m.insert( std::make_pair( "c", &a[2] ) );

  //print map keys
  boost::copy( m | boost::adaptors::map_keys,
               std::ostream_iterator<std::string> {std::cout, ","} );
  std::cout << '\n';

  //print map values
  boost::copy( m | boost::adaptors::map_values | boost::adaptors::indirected,
               std::ostream_iterator<int> {std::cout, ","} );
  std::cout << '\n';

  //or even better...
  std::vector<int> v;
  boost::push_back( v, m | boost::adaptors::map_values |
                    boost::adaptors::indirected );
  boost::copy( v, std::ostream_iterator<int> {std::cout, ","} );
  std::cout << '\n';

  //
  std::vector<int> res;
  boost::copy( boost::adaptors::unique( boost::adaptors::reverse( a ) ),
               std::back_inserter( res ) );
  boost::copy( res, std::ostream_iterator<int> {std::cout, ","} );
  std::cout << '\n';

  // more efficient (no extra allocations) + flexible
  std::vector<int> vec;
  boost::push_back( vec, a | boost::adaptors::reversed );
  boost::copy( vec, std::ostream_iterator<int> {std::cout, ","} );
  std::cout << '\n';
}
