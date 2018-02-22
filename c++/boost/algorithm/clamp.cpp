#include <boost/algorithm/clamp.hpp>
#include <iostream>

int main( int argc, char *argv[] )
{
  int foo = 23;
  foo = boost::algorithm::clamp ( foo, 1, 10 );

  std::cout << "clamp(<23>, 1, 10 )= " << foo << std::endl;

  foo = boost::algorithm::clamp ( foo, 11, 20 ); //foo = 10
  std::cout << "clamp(<10>, 11, 20 )= " << foo << std::endl;

  foo = boost::algorithm::clamp ( 15, 11, 20 );
  std::cout << "clamp(<15>, 11, 20 )= " << foo << std::endl;
}
