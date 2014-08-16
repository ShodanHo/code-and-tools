#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <stdexcept>

#define STR(x) #x << '=' << x

//template < class T, class Alloc = allocator<T> > class list;
//template < class T, class Alloc = allocator<T> > class vector;
template < template<class,class> class C, class T>
std::ostream& operator<<(std::ostream& os, const C<T, std::allocator<T> >& c)
{
  os << '[';
  for (auto itr = c.cbegin(); itr != c.cend(); ++itr) {
    if (itr != c.cbegin())
      os << ',';
    os << *itr;
  }
  os << ']';
  return os;
}

struct person {
  std::string _name;
  bool _male;
  person() : _name(), _male(false) {}
  person(const std::string name, bool male) : _name(name), _male(male) {}
};

std::ostream&
operator<<(std::ostream& os, const person& p)
{
  os << p._name << ':' << p._male;
  return os;
}

template <class T>
typename std::list<T>::const_iterator
list_index(const std::list<T>& l, std::size_t n)
{
  for (auto itr = l.cbegin(); itr != l.cend(); ++itr) {
    if (!n--) {
      return itr;
    }
  }
  throw std::out_of_range("index not found");
}

int main()
{
  std::list<person> people;
  people.push_back(person("ann", false));
  people.push_back(person("mary", false));
  people.push_back(person("jane", false));
  people.push_back(person("carol", false));
  people.push_back(person("beth", false));

  people.push_back(person("matthew", true));
  people.push_back(person("john", true));
  people.push_back(person("mark", true));
  people.push_back(person("peter", true));
  people.push_back(person("luke", true));

  std::cout << STR(people) << std::endl;

  { // first way. use seperate vectors for male and females
    std::vector<person> males;
    std::vector<person> females;
    for_each(people.begin(), people.end(),
	     [&males, &females](const person& person) {
	       if (person._male) males.push_back(person);
	       else females.push_back(person);
	     }
	     );

    std::cout << STR(males) << std::endl;
    std::cout << STR(females) << std::endl;

    srand (time(NULL));

    int random_male = rand() % males.size();
    person john_doe = males[random_male];

    int random_female = rand() % females.size();
    person jane_doe = females[random_female];

    std::cout << STR(john_doe) << std::endl
	      << STR(jane_doe) << std::endl;
  }

  { // second way. use a vector of everyone
    srand (time(NULL));

    std::vector<person> everyone;
    for_each(people.begin(), people.end(),
	     [&everyone](const person& person)
	     { everyone.push_back(person); }
	     );

    std::cout << STR(everyone) << std::endl;
    person john_doe;
    for (;;) {
      person random_person = everyone[rand() % everyone.size()];
      if (random_person._male == true) {
	john_doe = random_person;
	break;
      }
    }
    person jane_doe;
    for (;;) {
      person random_person = everyone[rand() % everyone.size()];
      if (random_person._male == false) {
	jane_doe = random_person;
	break;
      }
    }
    std::cout << STR(john_doe) << std::endl
	      << STR(jane_doe) << std::endl;
  }

  { // third way. traverse the list
    srand (time(NULL));

    bool male_found = false;
    bool female_found = false;
    person john_doe;
    person jane_doe;
    for (; !male_found || !female_found;) {
      unsigned int random_person_num = rand() % people.size(); // [0,size)
      for (auto &p : people) {
	if (!random_person_num--) {
	  if ((p._male == true) && !male_found) {
	    john_doe = p;
	    male_found = true;
	  }
	  if ((p._male == false) && !female_found) {
	    jane_doe = p;
	    female_found = true;
	  }
	}
      }
    }

    std::cout << STR(john_doe) << std::endl
	      << STR(jane_doe) << std::endl;
  }

  { // fourth way. index the list
    srand (time(NULL));

    bool male_found = false;
    bool female_found = false;
    person john_doe;
    person jane_doe;
    for (; !male_found || !female_found;) {
      unsigned int random_person_num = rand() % people.size(); // [0,size)

      person random_person = *list_index(people, random_person_num);

      if ((random_person._male == true) && !male_found) {
	john_doe = random_person;
	male_found = true;
      }
      if ((random_person._male == false) && !female_found) {
	jane_doe = random_person;
	female_found = true;
      }
    }

    std::cout << STR(john_doe) << std::endl
	      << STR(jane_doe) << std::endl;
  }
  return 0;
}
