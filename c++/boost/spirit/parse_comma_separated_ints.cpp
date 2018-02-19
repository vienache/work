#include <boost/spirit/include/qi.hpp>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

using namespace boost::spirit;

int main()
{
  std::string s = "1,2,3,4,5";

  auto it = s.begin();
  std::vector<int> v;

  if (qi::phrase_parse(it, s.end(), qi::int_ % ',', ascii::space, v))
  {
    std::ostream_iterator<int> out(std::cout, "; ");
    std::copy(v.begin(), v.end(), out);

    std::cout << std::endl;
  }
}
