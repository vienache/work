#include "CCallStub.hpp"
#include "ISimEngine.hpp"
#include <iostream>

CCallStub::CCallStub( std::shared_ptr<ISimEngine> &engine )
   : ISimCaller()
{
   engine->addCaller( this );
}

CCallStub::~CCallStub()
{
}

void CCallStub::simCall( const std::string &module,
                         const std::string &method,
                         const std::string &value )
{
   std::cout << "CALL module:" << module << " method:"
             << method << " value:" << value << std::endl;
}
