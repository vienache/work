#include <iostream>
#include <string>
#include <boost/regex.hpp>

int main()
{
  try
  {
    boost::basic_regex<char, boost::cpp_regex_traits<char>> r("_E[0-9]+\\.[0-9]+_");

    std::string s("_E209.002_");

    if (boost::regex_match(s, r))
    {
      std::cout << "matched!" << std::endl;
    }
    else
    {
      std::cout << "not matched!" << std::endl;
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
  }
}
