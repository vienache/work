#include "CActions.hpp"
#include "IActionVisitor.hpp"
#include <iostream>

CActionSet::CActionSet( const std::string &module, const std::string &value )
   : mModule( module )
   , mValue( value )
{
}

CActionSet::~CActionSet()
{
}

void CActionSet::execute( IActionVisitor &visitor )
{
   visitor.visit( shared_from_this() );
}

const std::string &CActionSet::getModule() const
{
   return mModule;
}
const std::string &CActionSet::getValue() const
{
   return mValue;
}

//===============================

CActionCall::CActionCall( const std::string &module, const std::string &method,
                          const std::string &value )
   : mModule( module )
   , mMethod( method )
   , mValue( value )
{
}

CActionCall::~CActionCall()
{
}

void CActionCall::execute( IActionVisitor &visitor )
{
   visitor.visit( shared_from_this() );
}

const std::string &CActionCall::getModule() const
{
   return mModule;
}
const std::string &CActionCall::getMethod() const
{
   return mMethod;
}
const std::string &CActionCall::getValue() const
{
   return mValue;
}

//===============================

CActionWait::CActionWait( const std::string &module,
                          const std::string &trigger )
   : mModule( module )
   , mTrigger( trigger )
{
}

CActionWait::~CActionWait()
{
}

void CActionWait::execute( IActionVisitor &visitor )
{
   visitor.visit( shared_from_this() );
}

const std::string &CActionWait::getModule() const
{
   return mModule;
}
const std::string &CActionWait::getTrigger() const
{
   return mTrigger;
}


//===============================

CActionWaitUntil::CActionWaitUntil( const std::string &timeout )
   : mTimeout( timeout )
{
}

CActionWaitUntil::~CActionWaitUntil()
{
}

void CActionWaitUntil::execute( IActionVisitor &visitor )
{
   visitor.visit( shared_from_this() );
}

const std::string &CActionWaitUntil::getTimeout() const
{
   return mTimeout;
}

//===============================

CActionWaitGroup::Condition::Condition( const std::string &module,
                                        const std::string &trigger )
   : mModule( module )
   , mTrigger( trigger )
{
}

CActionWaitGroup::CActionWaitGroup( const std::list<Condition> &conditions )
   : mConditions( conditions )
{
}

CActionWaitGroup::CActionWaitGroup( std::list<Condition> &&conditions )
   : mConditions( std::move( conditions ) )
{
}

CActionWaitGroup::~CActionWaitGroup()
{
}

void CActionWaitGroup::execute( IActionVisitor &visitor )
{
   visitor.visit( shared_from_this() );
}

const std::list<CActionWaitGroup::Condition> &CActionWaitGroup::getConditions()
const
{
   return mConditions;
}
