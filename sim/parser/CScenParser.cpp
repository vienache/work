#include "CScenParser.hpp"
#include "CActions.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <tuple>

/*static*/ const char *CScenParser::SET = "set";
/*static*/ const char *CScenParser::CALL = "call";
/*static*/ const char *CScenParser::WAIT = "wait";
/*static*/ const char *CScenParser::WAIT_GROUP = "wait_group";
/*static*/ const char *CScenParser::WAIT_UNTIL = "wait_until";

CScenParser::CScenParser( const std::string &scenName )
   : mScenName( scenName )
{
}

CScenParser::~CScenParser()
{
}

bool CScenParser::parseScenario( std::list< std::shared_ptr<IAction> >
                                 &actions )
{
   bool result = false;
   boost::property_tree::ptree pt;

   try
   {
      boost::property_tree::read_xml( mScenName, pt );

      for ( const auto &node : pt.get_child( "test_case" ) )
      {
         if ( node.first == SET )
         {
            actions.push_back( std::make_shared<CActionSet>(
                                  node.second.get<std::string>( "<xmlattr>.module" ),
                                  node.second.get<std::string>( "<xmlattr>.value" ) ) );
         }
         else if ( node.first == CALL )
         {
            actions.push_back( std::make_shared<CActionCall>(
                                  node.second.get<std::string>( "<xmlattr>.module" ),
                                  node.second.get<std::string>( "<xmlattr>.method" ),
                                  node.second.get<std::string>( "<xmlattr>.value" ) ) );
         }
         else if ( node.first == WAIT )
         {
            actions.push_back( std::make_shared<CActionWait>(
                                  node.second.get<std::string>( "<xmlattr>.module" ),
                                  node.second.get<std::string>( "<xmlattr>.trigger" ) ) );
         }
         else if ( node.first == WAIT_UNTIL )
         {
            actions.push_back( std::make_shared<CActionWaitUntil>(
                                  node.second.get<std::string>( "<xmlattr>.timeout" ) ) );
         }
         else if ( node.first == WAIT_GROUP )
         {
            std::list<CActionWaitGroup::Condition> conditions;
            for( const auto &i : node.second )
            {
               std::string name;
               boost::property_tree::ptree sub_pt;
               std::tie( name, sub_pt ) = i;

               if ( name == "condition" )
               {
                  conditions.push_back( CActionWaitGroup::Condition(
                                           sub_pt.get<std::string>( "<xmlattr>.module" ),
                                           sub_pt.get<std::string>( "<xmlattr>.trigger" ) ) );
               }
            }
            actions.push_back( std::make_shared<CActionWaitGroup>( conditions ) );
         }
      }
      result = true;
   }
   catch ( const boost::property_tree::xml_parser::xml_parser_error &ex )
   {
      std::cerr << "caught " << ex.message() << std::endl;
   }
   catch ( const boost::property_tree::ptree_bad_path &ex )
   {
      std::cerr << "caught " << ex.what() << std::endl;
   }

   return result;
}

