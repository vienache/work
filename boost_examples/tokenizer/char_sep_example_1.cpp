#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

//------------------------------------------------------
void foo()
{
  std::string str = ";;Hello|world||-foo--bar;yow;baz|";
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

  std::cout << str << "\n";

  boost::char_separator<char> sep("-;|");
  tokenizer tokens(str, sep);
  for (tokenizer::iterator tok_iter = tokens.begin();
       tok_iter != tokens.end(); ++tok_iter)
  {
    std::cout << "<" << *tok_iter << "> ";
  }
  std::cout << "\n";

  size_t noTokens = std::distance(tokens.begin(), tokens.end());
  std::cout << "Number of tokens is: " << noTokens << std::endl;

  auto it = tokens.begin();
  std::advance(it, 5);
  std::cout << "Last token is: " << *it << std::endl;
}

//------------------------------------------------------
void bar(const std::string &str)
{
  //tokenize version
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

  std::cout << str << "\n";

  boost::char_separator<char> sep("_");
  tokenizer tokens(str, sep);

  for (tokenizer::iterator tok_iter = tokens.begin();
       tok_iter != tokens.end(); ++tok_iter)
  {
    std::cout << "<" << *tok_iter << "> ";
  }
  std::cout << "\n";

  size_t noTokens = std::distance(tokens.begin(), tokens.end());
  std::cout << "Number of tokens is: " << noTokens << std::endl;

}

//------------------------------------------------------
int main()
{
  std::string version = "PL_NTG6_E209.003_18071AC2";

  foo();

  std::cout << "-------------------------\n\n";

  bar(version);

  return EXIT_SUCCESS;
}
