#ifndef CSCENPARSER_HPP
#define CSCENPARSER_HPP
#include <string>
#include <memory>
#include <list>
#include "IAction.hpp"

class CScenParser final
{
public:
    CScenParser( const std::string &scenName );

    ~CScenParser();

    bool parseScenario( std::list< std::shared_ptr<IAction> > &actions );

private:
    static const char *SET;
    static const char *CALL;
    static const char *WAIT;
    static const char *WAIT_GROUP;
    static const char *WAIT_UNTIL;

    std::string mScenName;
};

#endif  //CSCENPARSER_HPP
