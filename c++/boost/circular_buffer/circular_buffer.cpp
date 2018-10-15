#include <boost/circular_buffer.hpp>
#include <iostream>

int main()
{
  boost::circular_buffer<int> cb{3};

  std::cout << "capacity t0: " << cb.capacity() << '\n';
  std::cout << "size t0: " << cb.size() << '\n';

  cb.push_back( 0 );
  cb.push_back( 1 );
  cb.push_back( 2 );

  std::cout << "size t1: " << cb.size() << '\n';

  cb.push_back( 3 );
  cb.push_back( 4 );
  cb.push_back( 5 );

  std::cout << "size t2: " << cb.size() << '\n';

  std::cout << "elements:";
  for ( int i : cb )
  {
    std::cout << ' ' << i;
  }
  std::cout << '\n';
}
