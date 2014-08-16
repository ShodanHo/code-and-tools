#include <iostream>
#include <stdio.h>
#include "employee.h"

static std::string
displayEmployee(const Employee& e)
{
  //std::cout << "Name          ID Number  Department     Position    Years Worked"
  std::string name = e.getName(); name.resize(12, ' ');
  std::string idNumber = e.getIdNumber(); idNumber.resize(9, ' ');
  std::string department = e.getDepartment(); department.resize(13, ' ');
  std::string position = e.getPosition(); position.resize(10, ' ');
  unsigned yearsWorked = e.getYearsWorked();
  char str[20]; sprintf(str, "%d", yearsWorked);
  std::string yearsWorkedStr(str);

  std::string ret =
    name + "  " +
    idNumber + "  " +
    department + "  " +
    position + "  " +
    yearsWorkedStr;

  return ret;
}

int main(int argc, char **argv)
{
  Employee jennyJacobs("Jenny Jacobs", "JJ8990", "Accounting",    "President", 15);
  Employee myronSmith( "Myron Smith",  "MS7571", "IT",            "Programmer", 5);
  Employee chrisRaines("Chris Raines", "CR6873", "Manufacturing", "Engineer", 30);

  //jennyJacobs.setYearsWorked(-1);

  std::cout << "Name          ID Number  Department     Position    Years Worked"
	    << std::endl;
  std::cout << "----          ---------  ----------     --------    ------------"
	    << std::endl;

  std::cout << displayEmployee(jennyJacobs)
	    << std::endl;

  std::cout << displayEmployee(myronSmith)
	    << std::endl;

  std::cout << displayEmployee(chrisRaines)
	    << std::endl;

}
