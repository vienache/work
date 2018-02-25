#include <iostream>
#include <iterator> //for std::ostream_iterator
#include <vector>
#include <algorithm>

template<class ForwardIt>
void selection_sort( ForwardIt begin, ForwardIt end )
{
  for ( ForwardIt i = begin; i != end; ++i )
  {
    std::iter_swap( i, std::min_element( i, end ) );
  }
}

int main()
{
  std::vector<int> v = { -1, 2, 0, -4, 5, 3, -5};

  std::cout << "Before sort: ";
  std::copy( v.begin(), v.end(), std::ostream_iterator<int>( std::cout, " " ) );

  selection_sort( v.begin(), v.end() );

  std::cout << "\nAfter sort: ";
  std::copy( v.begin(), v.end(), std::ostream_iterator<int>( std::cout, " " ) );
  std::cout << '\n';
}
