#ifndef ISIMSETTER_HPP
#define ISIMSETTER_HPP

class ISimSetter
{
public:
   virtual ~ISimSetter() {};

   virtual void simSet( const std::string &module,
                        const std::string &value ) = 0;
};

#endif  //ISIMSETTER_HPP
