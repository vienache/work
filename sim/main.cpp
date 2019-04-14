#include "CSimEngine.hpp"
#include "CCallStub.hpp"
#include "CSetStub.hpp"

int main( int argc, char *argv[] )
{
   std::shared_ptr<ISimEngine> engine =
      std::make_shared<CSimEngine>( "language.xml" );
   CCallStub callStub( engine );
   CSetStub setStub( engine );
   engine->run();

   return 0;
}
