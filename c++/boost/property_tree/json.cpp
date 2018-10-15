#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

using namespace boost::property_tree;

const char *json_str = R"(
    {
      "animals": [
        {
          "name": "cat",
          "legs": 4,
          "has_tail": true
        },
        {
          "name": "spider",
          "legs": 8,
          "has_tail": false
        }
      ],
      "log": {
        "all": true
      }
    }
)";

int main()
{
    {
        ptree pt;
        pt.put( "C:.Windows.System", "20 files" );
        pt.put( "C:.Windows.Cursors", "50 files" );

        json_parser::write_json( "file.json", pt );

        ptree pt2;
        json_parser::read_json( "file.json", pt2 );

        std::cout << std::boolalpha << ( pt == pt2 ) << '\n';
    }

    {
        ptree pt;
        std::stringstream input(json_str);
        json_parser::read_json( input , pt );

        std::stringstream output;
        json_parser::write_json( output, pt, false /*no-pretty (i.e. on one line)*/ );

        std::cout << output.str() << std::endl;
    }
}
