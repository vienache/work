#ifndef CACTIONS_HPP
#define CACTIONS_HPP

#include <list>
#include <memory>
#include <string>
#include "IAction.hpp"

class CActionSet
   : public IAction
   , public std::enable_shared_from_this<CActionSet>
{
public:
   CActionSet( const std::string &module, const std::string &value );
   virtual ~CActionSet();
   virtual void execute( IActionVisitor &visitor );
   const std::string &getModule() const;
   const std::string &getValue() const;

private:
   std::string mModule;
   std::string mValue;
};

//===============================

class CActionCall
   : public IAction
   , public std::enable_shared_from_this<CActionCall>
{
public:
   CActionCall( const std::string &module, const std::string &method,
                const std::string &value );
   virtual ~CActionCall();
   virtual void execute( IActionVisitor &visitor );
   const std::string &getModule() const;
   const std::string &getMethod() const;
   const std::string &getValue() const;

private:
   std::string mModule;
   std::string mMethod;
   std::string mValue;
};

//===============================

class CActionWait
   : public IAction
   , public std::enable_shared_from_this<CActionWait>
{
public:
   CActionWait( const std::string &module, const std::string &trigger );
   virtual ~CActionWait();
   virtual void execute( IActionVisitor &visitor );
   const std::string &getModule() const;
   const std::string &getTrigger() const;

private:
   std::string mModule;
   std::string mTrigger;
};

//===============================

class CActionWaitUntil
   : public IAction
   , public std::enable_shared_from_this<CActionWaitUntil>
{
public:
   CActionWaitUntil( const std::string &timeout );
   virtual ~CActionWaitUntil();
   virtual void execute( IActionVisitor &visitor );
   const std::string &getTimeout() const;

private:
   std::string mTimeout;
};

//===============================

class CActionWaitGroup
   : public IAction
   , public std::enable_shared_from_this<CActionWaitGroup>
{
public:
   struct Condition
   {
      std::string mModule;
      std::string mTrigger;
      Condition( const std::string &module, const std::string &trigger );
   };

   CActionWaitGroup( const std::list<Condition> &conditions );
   CActionWaitGroup( std::list<Condition> &&conditions );
   virtual ~CActionWaitGroup();
   virtual void execute( IActionVisitor &visitor );
   const std::list<Condition> &getConditions() const;

private:
   std::list<Condition> mConditions;
};

#endif  //CACTIONS_HPP
