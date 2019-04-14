#ifndef ISIMENGINE_HPP
#define ISIMENGINE_HPP

#include <memory>

class ISimCaller;
class ISimSetter;

class ISimEngine
{
public:
   virtual ~ISimEngine() {};

   virtual void run() = 0;

   virtual void addSetter( ISimSetter *setter ) = 0;
   virtual void addCaller( ISimCaller *caller ) = 0;

   virtual void trigger( const std::string &module, const std::string &value ) = 0;
};

#endif  //ISIMENGINE_HPP
