#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <array>
#include <iostream>

int main()
{
  {
    std::cout << "----1. boost::count ----" << '\n';
    std::vector<int> a = {0, 1, 0, 1, 0, 1, 0};
    std::cout << boost::count( a, 0 ) << '\n';
  }

  {
    std::cout << "----2. boost::max_element, boost::accumulate ----" << '\n';
    std::array<int, 6> a {{0, 1, 2, 3, 4, 5}};
    boost::random_shuffle( a );
    boost::copy( a, std::ostream_iterator<int> {std::cout, ","} );
    std::cout << "\n" << *boost::max_element( a ) << '\n';
    std::cout << boost::accumulate( a, 0 ) << '\n';
  }
}
