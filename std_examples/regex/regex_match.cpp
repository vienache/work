#include <iostream>
#include <string>
#include <regex>

int main()
{
  try
  {
    //NOTE: it works with GCC 7.1
    std::regex r("_E[0-9]+\\.[0-9]+_", std::regex_constants::extended);
    std::string s("_E209.002_");

    if (std::regex_match(s, r))
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
