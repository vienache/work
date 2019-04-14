#include "CSetStub.hpp"
#include "ISimEngine.hpp"
#include <iostream>

CSetStub::CSetStub( std::shared_ptr<ISimEngine> &engine )
   : ISimSetter()
   , mEngine( engine )
{
   engine->addSetter( this );
}

CSetStub::~CSetStub()
{
}

void CSetStub::simSet( const std::string &module, const std::string &value )
{
   std::cout << "SET module:" << module << " value:" << value << std::endl;
   //mEngine->trigger( "CSetStub", "set" );
}
