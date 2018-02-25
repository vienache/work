#include <algorithm>
#include <array>
#include <iostream>
#include <iterator> //for std::ostream_iterator

int main()
{
  std::array<int, 9> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  auto is_even = []( int i ) { return i % 2 == 0; };
  std::cout.setf( std::ios_base::boolalpha );
  std::copy( v.begin(), v.end(), std::ostream_iterator<int>( std::cout, " " ) );
  std::cout << ": is_partitioned: " << std::is_partitioned( v.begin(), v.end(), is_even ) << '\n';

  std::partition( v.begin(), v.end(), is_even );
  std::copy( v.begin(), v.end(), std::ostream_iterator<int>( std::cout, " " ) );
  std::cout << ": is_partitioned: " << std::is_partitioned( v.begin(), v.end(), is_even ) << '\n';

  std::reverse( v.begin(), v.end() );
  std::copy( v.begin(), v.end(), std::ostream_iterator<int>( std::cout, " " ) );
  std::cout << ": is_partitioned: " << std::is_partitioned( v.begin(), v.end(), is_even ) << '\n';
}
