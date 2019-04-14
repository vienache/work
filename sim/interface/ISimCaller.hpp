#ifndef ISIMCALLER_HPP
#define ISIMCALLER_HPP

#include <string>

class ISimCaller
{
public:
   virtual ~ISimCaller() {};

   virtual void simCall( const std::string &module,
                         const std::string &method,
                         const std::string &value ) = 0;
};

#endif  //ISIMCALLER_HPP
