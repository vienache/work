#ifndef CCALLSTUB_HPP
#define CCALLSTUB_HPP

#include <memory>
#include "ISimCaller.hpp"

class ISimEngine;

class CCallStub : public ISimCaller
{
public:
   CCallStub( std::shared_ptr<ISimEngine> &engine );

   virtual ~CCallStub();

   virtual void simCall( const std::string &module,
                         const std::string &method,
                         const std::string &value ) override;
};

#endif //CCALLSTUB_HPP
