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
  { // key:value
    boost::property_tree::ptree pt;
    std::string json ("{ \"time\": \"values\",\"date\":\"1234\" }");
    std::istringstream input (json);

    try
    {
      boost::property_tree::read_json (input, pt);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error " << e.message() << "\n";
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
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
  }

  { // object
    boost::property_tree::ptree pt;
    std::string json ("{ \"root\": { \"values\": [ 1, 2, 3, 4, 5 ] } }");
    std::istringstream input (json);

    try
    {
      boost::property_tree::read_json (input, pt);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error " << e.message() << "\n";
      return -1;
    }

    try
    {
      std::cout << "array:\n";
      for (auto& child : pt.get_child("root.values"))
      {
        std::cout << child.first << "\n";
        std::cout << child.second.data() << "\n";
      }
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
  }

  {
    boost::property_tree::ptree pt;
    std::string json ("{ \"object\": { \"name\": \"John\", \"age\": 22 } }");
    std::istringstream input (json);

    try
    {
      boost::property_tree::read_json (input, pt);
    }
    catch (boost::property_tree::json_parser_error& e)
    {
      std::cerr << HERE() << ':' << "error " << e.message() << "\n";
      return -1;
    }

    try
    {
      std::cout << "object:\n";
      for (auto& child : pt.get_child("object"))
      {
        std::cout << child.first << ":" << child.second.data() << "\n";
      }
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
  }
  {
    std::string x =
    "{"
      "\"firstName\": \"John\"," // key:value
      "\"lastName\": \"Smith\"," // key:value
      "\"isAlive\": true," // key:value
      "\"age\": 25," // key:value
      "\"address\": {" // object
        "\"streetAddress\": \"21 2nd Street\","
        "\"city\": \"New York\","
        "\"state\": \"NY\","
        "\"postalCode\": \"10021-3100\""
      "},"
      "\"phoneNumbers\": [" // array
        "{"
          "\"type\": \"home\","
          "\"number\": \"212 555-1234\""
        "},"
        "{"
          "\"type\": \"office\","
          "\"number\": \"646 555-4567\""
        "}"
      "],"
      "\"children\": []," // key:value
      "\"spouse\": null" // key:value
    "}";

    boost::property_tree::ptree pt;
    std::string json (x);
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

    // key:value
    try
    {
      const std::string x[] = {"firstName","lastName","isAlive","age","spouse"};
      for (auto key:x) {
        std::string value = pt.get<std::string> (key);
        std::cout << STR(key) << ':' << STR(value) << '\n';
      }
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }

    // objects
    try
    {
      std::cout << "object:\n";
      for (auto& child : pt.get_child("address"))
      {
        std::cout << child.first << ":" << child.second.data() << "\n";
      }
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }

    // arrays of objects
    try
    {
      std::cout << "array:\n";
      for (auto& child : pt.get_child("phoneNumbers"))
      {
        for (auto& childschild : child.second.get_child(""))
        {
          std::cout << childschild.first << ":" << childschild.second.data() << "\n";
        }
      }
    }
    catch (boost::property_tree::ptree_bad_path& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
    catch (boost::property_tree::ptree_bad_data& e)
    {
      std::cerr << HERE() << ':' << "error " << e.what() << "\n";
      return -1;
    }
  }

  return 0;
}
