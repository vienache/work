#include <boost/range/algorithm.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <array>
#include <deque>
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
    std::array<int, 6> a {0, 1, 2, 3, 4, 5};
    boost::random_shuffle( a );
    boost::copy( a, std::ostream_iterator<int> {std::cout, ","} );
    std::cout << "\n" << *boost::max_element( a ) << '\n';
    std::cout << boost::accumulate( a, 0 ) << '\n';
  }

  //Range algorithms without counterparts in the standard library
  {
    std::cout << "----1. is_sorted, remove_erase, push_back ----" << '\n';
    std::array<int, 6> a{0, 1, 2, 3, 4, 5};
    std::cout << std::boolalpha << "is_sorted: " << boost::is_sorted( a ) << '\n';

    std::deque<int> d;
    boost::push_back( d, a );
    boost::copy( d, std::ostream_iterator<int> {std::cout, ","} );
    std::cout << '\n';

    boost::remove_erase( d, 2 );
    boost::copy( d, std::ostream_iterator<int> {std::cout, ","} );
    std::cout << '\n';

    boost::copy_n( d, 3, std::ostream_iterator<int> {std::cout, ","} );
    boost::copy( d, std::ostream_iterator<int> {std::cout, ","} );
    std::cout << '\n';
  }

}
