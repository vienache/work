#ifndef IACTION_HPP
#define IACTION_HPP

class IActionVisitor;

class IAction
{
public:
    virtual ~IAction() {};

    virtual void execute( IActionVisitor &visitor ) = 0;
};

#endif  //IACTION_HPP
