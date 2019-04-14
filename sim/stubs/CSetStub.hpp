#ifndef CSETSTUB_HPP
#define CSETSTUB_HPP

#include <memory>
#include "ISimSetter.hpp"

class ISimEngine;

class CSetStub : public ISimSetter
{
public:
   CSetStub( std::shared_ptr<ISimEngine> &engine );

   virtual ~CSetStub();

   virtual void simSet( const std::string &module,
                        const std::string &value ) override;

private:
   std::shared_ptr<ISimEngine> mEngine;
};

#endif //CSETSTUB_HPP
