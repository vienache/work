#include <iostream>
#include <string>
#include <regex>

static const std::string string("***abc_E209.002_123***");
static const std::string pattern = "_E[0-9]+\\.[0-9]+_";

int main()
{
  //NOTE: it works with GCC 7.1

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
      std::cout << match[0].str() << std::endl;
    }
  }
  else
  {
    std::cout << "not matched!" << std::endl;
  }
}
