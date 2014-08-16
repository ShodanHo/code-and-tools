#include <string>

class Employee {

private:
  std::string name; // first and last name
  std::string idNumber;
  std::string department;
  std::string position;
  int yearsWorked;

public:
  Employee(const std::string& _name,
	   const std::string& _idNumber,
	   const std::string& _department,
	   const std::string& _position,
	   int _yearsWorked);

  Employee(const std::string& _name,
	   const std::string& _idNumber);

  Employee();

  std::string getName() const;
  void setName(const std::string&);

  std::string getIdNumber() const;
  void setIdNumber(const std::string&);

  std::string getDepartment() const;
  void setDepartment(const std::string&);

  std::string getPosition() const;
  void setPosition(const std::string&);

  int getYearsWorked() const;
  void setYearsWorked(int);
};
