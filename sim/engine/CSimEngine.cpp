#include "CSimEngine.hpp"
#include "CActions.hpp"
#include "ISimCaller.hpp"
#include "ISimSetter.hpp"
#include <iostream>

CSimEngine::CSimEngine( const std::string &scenName )
   : mScenParser( scenName )
{
   bool result;

   result = mScenParser.parseScenario( mActions );
}

CSimEngine::~CSimEngine()
{
}

void CSimEngine::run()
{
   for ( const auto &action : mActions )
   {
      if ( action.get() )
      {
         action->execute( *this );
      }
   }
}

void CSimEngine::trigger( const std::string &module, const std::string &value )
{
   std::cout << "trigger module:" << module << " value:" << value << std::endl;
}

void CSimEngine::addSetter( ISimSetter *setter )
{
   if ( setter )
   {
      mSetters.push_back( setter );
   }
}

void CSimEngine::addCaller( ISimCaller *caller )
{
   if ( caller )
   {
      mCallers.push_back( caller );
   }
}

void CSimEngine::visit( const std::shared_ptr<CActionSet> &rv )
{
   for ( const auto &s : mSetters )
   {
      s->simSet( rv->getModule(), rv->getValue() );
   }
}

void CSimEngine::visit( const std::shared_ptr<CActionCall> &rv )
{
   for ( const auto &s : mCallers )
   {
      s->simCall( rv->getModule(), rv->getValue(), rv->getValue() );
   }
}

void CSimEngine::visit( const std::shared_ptr<CActionWait> &rv )
{
   std::cout << "WAIT module:" << rv->getModule() << " trigger:"
             << rv->getTrigger() << std::endl;
}

void CSimEngine::visit( const std::shared_ptr<CActionWaitUntil> &rv )
{
   std::cout << "WAIT_UNTIL timeout:" << rv->getTimeout() << std::endl;
}

void CSimEngine::visit( const std::shared_ptr<CActionWaitGroup> &rv )
{
   std::cout << "WAIT_GROUP :" << std::endl;
   for ( const auto &c : rv->getConditions() )
   {
      std::cout << "   Condition module:" << c.mModule << " trigger:"
                << c.mTrigger << std::endl;
   }
}
