#include <iostream>
#include <iterator>
#include <algorithm>
int main()
{
  std::ostream_iterator<int> i1( std::cout, ", " );
  std::fill_n( i1, 5, -1 );
  std::ostream_iterator<double> i2( std::cout );
  *i2++ = 3.14;
  std::cout << '\n';
}
