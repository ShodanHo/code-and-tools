#include "employee.h"
#include <stdexcept>

Employee::Employee(const std::string& _name,
		   const std::string& _idNumber,
		   const std::string& _department,
		   const std::string& _position,
		   int _yearsWorked) :
  name(_name),
  idNumber(_idNumber),
  department(_department),
  position(_position),
  yearsWorked(_yearsWorked)
{}

Employee::Employee(const std::string& _name,
		   const std::string& _idNumber) :
  name(_name),
  idNumber(_idNumber),
  department(""),
  position(""),
  yearsWorked(0)
{}

Employee::Employee() :
  name(""),
  idNumber(""),
  department(""),
  position(""),
  yearsWorked(0)
{}

std::string Employee::getName() const
{
  return name;
}

void Employee::setName(const std::string& _name)
{
  name = _name;
}

std::string Employee::getIdNumber() const
{
  return idNumber;
}

void Employee::setIdNumber(const std::string& _idNumber)
{
  idNumber = _idNumber;
}

std::string Employee::getDepartment() const
{
  return department;
}

void Employee::setDepartment(const std::string& _department)
{
  department = _department;
}

std::string Employee::getPosition() const
{
  return position;
}

void Employee::setPosition(const std::string& _position)
{
  position = _position;
}

int Employee::getYearsWorked() const
{
  return yearsWorked;
}

void Employee::setYearsWorked(int _yearsWorked)
{
  if (_yearsWorked < 0)
    throw std::out_of_range ("years worked cannot be negative");

  yearsWorked = _yearsWorked;
}
