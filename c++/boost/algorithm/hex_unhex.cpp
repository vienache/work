#include <boost/algorithm/hex.hpp>
#include <string>
#include <vector>
#include <iostream>

int main( int argc, char *argv[] )
{
  std::vector<uint8_t> data = { 'd', 'a', 't', 'a' };
  std::string hex;

  boost::algorithm::hex( data.begin(), data.end(), std::back_inserter( hex ) );

  std::cout << "Hex data: [ " << hex << " ]" << std::endl;

  assert( hex == "64617461" );

  //----------------------------------------------------

  std::string unhex;
  boost::algorithm::unhex( hex.begin(), hex.end(), std::back_inserter( unhex ) );

  std::cout << "Unhex data: [ " << unhex << " ]" << std::endl;

  assert( unhex == "data" );

  return 0;
}
