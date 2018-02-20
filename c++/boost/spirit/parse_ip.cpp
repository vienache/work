#include <string>
#include <string.h>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

struct Ipv4 { union { uint32_t as_int; uint8_t as_char[4]; } raw; };

Ipv4 make_ipv4( uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4 )
{
  Ipv4 ip;
  ip.raw.as_char[0] = i1;
  ip.raw.as_char[1] = i2;
  ip.raw.as_char[2] = i3;
  ip.raw.as_char[3] = i4;
  return ip;
}

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
qi::uint_parser<uint8_t, 10, 1, 3> octet;

struct Ipv4Address : qi::grammar<const char *, Ipv4()>
{
  Ipv4Address() : Ipv4Address::base_type( start )
{
  start = ( octet >> qi::lit( '.' ) >> octet >> qi::lit( '.' ) >>
            octet >> qi::lit( '.' ) >> octet
          ) [
            qi::_val = phx::bind( make_ipv4, qi::_1, qi::_2, qi::_3, qi::_4 )
          ]
          ;
}
qi::rule<const char *, Ipv4()> start;
} ipv4_address;

int main()
{
  Ipv4 ip;
  const char *s = "1.2.3.4";
  bool r = qi::parse( s, s + strlen( s ), ipv4_address, ip );
  std::cout << r << " " << ( int )ip.raw.as_char[0] << "." <<
            ( int )ip.raw.as_char[1] << "." <<
            ( int )ip.raw.as_char[2] << "." <<
            ( int )ip.raw.as_char[3] << std::endl;
}
