#include <boost/spirit/include/qi.hpp>
#include <string>
#include <iostream>

using namespace boost::spirit;

int main()
{
  std::string s = "  123abc";

  auto it = s.begin();

  bool match = qi::phrase_parse( it, s.end(), ascii::digit, ascii::space );

  std::cout << std::boolalpha << match << '\n';
  if ( it != s.end() )
  {
    std::cout << std::string {it, s.end()} << '\n';
  }
}
