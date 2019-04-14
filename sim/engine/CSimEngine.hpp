#ifndef CSIMENGINE_HPP
#define CSIMENGINE_HPP

#include <list>
#include "ISimEngine.hpp"
#include "IActionVisitor.hpp"
#include "CScenParser.hpp"

class IAction;

class CSimEngine final
   : public ISimEngine
   , public IActionVisitor
{
public:
   CSimEngine( const std::string &scenName );

   virtual ~CSimEngine();

   virtual void run() override;

   //! interface ISimEngine
   virtual void addSetter( ISimSetter *setter ) override;
   virtual void addCaller( ISimCaller *caller ) override;
   virtual void trigger( const std::string &module,
                         const std::string &value ) override;

   //! interface IActionVisitor
   virtual void visit( const std::shared_ptr<CActionSet> &rv ) override;
   virtual void visit( const std::shared_ptr<CActionCall> &rv ) override;
   virtual void visit( const std::shared_ptr<CActionWait> &rv ) override;
   virtual void visit( const std::shared_ptr<CActionWaitUntil> &rv ) override;
   virtual void visit( const std::shared_ptr<CActionWaitGroup> &rv ) override;

private:
   std::list<ISimCaller *> mCallers;
   std::list<ISimSetter *> mSetters;

   std::list< std::shared_ptr<IAction> > mActions;

   CScenParser mScenParser;
};

#endif  //CSIMENGINE_HPP
