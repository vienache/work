#ifndef IACTIONVISITOR_HPP
#define IACTIONVISITOR_HPP

#include <memory>

class CActionSet;
class CActionCall;
class CActionWait;
class CActionWaitUntil;
class CActionWaitGroup;

class IActionVisitor
{
public:
   virtual ~IActionVisitor() {};

   virtual void visit( const std::shared_ptr<CActionSet> &rv ) = 0;
   virtual void visit( const std::shared_ptr<CActionCall> &rv ) = 0;
   virtual void visit( const std::shared_ptr<CActionWait> &rv ) = 0;
   virtual void visit( const std::shared_ptr<CActionWaitUntil> &rv ) = 0;
   virtual void visit( const std::shared_ptr<CActionWaitGroup> &rv ) = 0;
};

#endif  //IACTIONVISITOR_HPP
