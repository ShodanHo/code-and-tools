/*
 * test_json.cpp
 *
 *  Created on: 18 Feb 2016
 *      Author: stuartf
 */

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define STR(x) #x << '=' << x
#define HERE() __FUNCTION__ << '(' << __LINE__ << ')'

int
main ()
{
  {
    boost::property_tree::ptree pt;
    std::string json ("{ \"time\": \"values\",\"date\":\"1234\" }");
    std::istringstream input (json);

    try
    {
      boost::property_tree::read_json (input, pt);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error\n";
      return -1;
    }

    try
    {
      std::string str = pt.get<std::string> ("time");
      std::cout << STR(str) << '\n';

      str = pt.get<std::string> ("date");
      std::cout << STR(str) << '\n';
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
      std::cerr << HERE() << ':' << "error\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error\n";
      return -1;
    }
  }

  {
    boost::property_tree::ptree pt;
    std::string json ("{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }");
    //std::string json ("{ \"root\": { \"values\": \"1\" } }");
    std::istringstream input (json);

    try
    {
      boost::property_tree::read_json (input, pt);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error\n";
      return -1;
    }

    try
    {
      for (auto& child : pt.get_child("root.values"))
      {
        std::cout << child.first << "\n";
        std::cout << child.second.data() << "\n";
      }
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error " << e.message() << "\n";
      return -1;
    }
  }

  {
    boost::property_tree::ptree pt;
    std::string json ("{ \"root\": { \"name\": \"John\", \"age\": 22 } }");
    //std::string json ("{ \"root\": { \"values\": \"1\" } }");
    std::istringstream input (json);

    try
    {
      boost::property_tree::read_json (input, pt);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error\n";
      return -1;
    }

    try
    {
      for (auto& child : pt.get_child("root"))
      {
        std::cout << child.first << "\n";
        std::cout << child.second.data() << "\n";
      }
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error " << e.message() << "\n";
      return -1;
    }
  }

  return 0;
}
