#include <iostream>
#include <string>
#include <regex>

int main()
{
  //NOTE: it works with GCC 7.1

  static const std::string string("***abc_E209.002_123***");
  static const std::string pattern = "_E[0-9]+\\.[0-9]+_";
  std::regex r;
  std::smatch match;

  try
  {
    r = std::regex(pattern, std::regex_constants::extended);
  }
  catch (const std::regex_error& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
  }

  if (std::regex_search(string, match, r))
  {
    std::cout << "matched!" << std::endl;
    if ( not match.empty() )
    {
      std::cout << match[0].str();
    }
  }
  else
  {
    std::cout << "not matched!" << std::endl;
  }
}
