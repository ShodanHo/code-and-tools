#include <iostream>
#include <fstream>
#include <stdio.h>

#define STR(x) #x << '=' << x
#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

using namespace std;

#define ROWS 15
#define COLUMNS 20

struct theatre_seats
{
private:
  bool seats[ROWS][COLUMNS]; // true if sold
  double rowPrice[ROWS];

public:
  theatre_seats(const double row_price[ROWS]);
  std::string displaySeating();
  std::string displayPrices();
  bool getSeat(int row, int column);
  bool setSeat(int row, int column, bool val);
  unsigned getSeatsSold();
  double getTotalRevenue();
};

theatre_seats::theatre_seats(const double row_price[ROWS])
{
  for (unsigned row = 0; row < LENGTH(rowPrice); row++)
    rowPrice[row] = row_price[row];

  for (int row = 0; row < ROWS; ++row)
    for (int column = 0; column < COLUMNS; ++column)
      seats[row][column] = false;
}

std::string theatre_seats::displaySeating()
{
  std::string ret("Seats: ");
  for (int column = 0; column < COLUMNS; ++column) {
    char str[10];
    sprintf(str, "%2d ", column);
    ret += str;
  }
  ret += '\n';

  for (int row = 0; row < ROWS; ++row) {
    char str[20];
    sprintf(str, "Row %-2d ", row);
    ret += str;
    for (int column = 0; column < COLUMNS; ++column)
      if (seats[row][column] == true)
	ret += " # ";
      else
	ret += " * ";
    ret += '\n';
  }
  return ret;
}

std::string theatre_seats::displayPrices()
{
  std::string ret("Prices: ");
  ret += '\n';

  for (int row = 0; row < ROWS; ++row) {
    char str[20];
    sprintf(str, "Row %-2d $ %.2lf", row, rowPrice[row]);
    ret += str;
    ret += '\n';
  }
  return ret;
}

bool
theatre_seats::getSeat(int row, int column)
{
  if (row < ROWS)
    if (column < COLUMNS)
      return seats[row][column];
  return false;
}

bool
theatre_seats::setSeat(int row, int column, bool val)
{
  if (row < ROWS)
    if (column < COLUMNS) {
      bool ret = seats[row][column];
      seats[row][column] = val;
      return ret;
    }
  return false;
}

unsigned
theatre_seats::getSeatsSold()
{
  unsigned total = 0;
  for (int row = 0; row < ROWS; ++row)
    for (int column = 0; column < COLUMNS; ++column)
      if (seats[row][column] == true)
	++total;

    return total;
}

double theatre_seats::getTotalRevenue()
{
  double total = 0.0;
  for (int row = 0; row < ROWS; ++row)
    for (int column = 0; column < COLUMNS; ++column)
      if (seats[row][column] == true)
	total += rowPrice[row];

    return total;
}

static bool
initialiseRowPrices(double* prices, int num)
{
  for (int i = 0; i < num; ++i)
    prices[i] = 0.0;

  string filename("prices.dat");
  std::ifstream pricesFile(filename.c_str());
  if (pricesFile.is_open())
  {
    for (int lineCount = 0; lineCount < num; ++lineCount) {
      if (pricesFile.good()) {
	string line;
	getline (pricesFile, line);
	//std::cout << line << endl;
	if ((line.size() > 1) && (line[0] >= '0') && (line[0] <= '9')) {
	  double d;
	  sscanf(line.c_str(), "%lf", &d);
	  //std::cout << "d=" << d << std::endl;
	  prices[lineCount] = d;
	} else {
	  std::cout << "Bad data at line " << lineCount + 1 << std::endl;
	  return false;
	}
      } else {
	std::cout << "Insufficient data at line " << lineCount + 1 << std::endl;
	return false;
      }
    }
    return true;
  }
  else {
    cout << "Unable to open file " << filename << std::endl; 
    return false;
  }
}

std::string
dollar(double x)
{
  char str[50];
  sprintf(str, "%.2lf", x);
  return str;
}

std::string
anotherTicket()
{
  std::cout << "Would you like to purchase another ticket (y/n)?:";
  std::string line;
  getline(std::cin, line);
  return line;
}

unsigned
enterRow()
{
  std::cout << "Enter the row (0-" << ROWS - 1 << ")?:";
  std::string line;
  getline(std::cin, line);
  if ((line[0] >= '0') && (line[0] <= '9')) {
    unsigned row;
    sscanf(line.c_str(), "%u", &row);
    if (row < ROWS)
      return row;
  }
  return ROWS; // invalid row
}

unsigned
enterColumn()
{
  std::cout << "Enter the seat (0-" << COLUMNS - 1 << ")?:";
  std::string line;
  getline(std::cin, line);
  if ((line[0] >= '0') && (line[0] <= '9')) {
    unsigned column;
    sscanf(line.c_str(), "%u", &column);
    if (column < COLUMNS)
      return column;
  }
  return COLUMNS; // invalid column
}

int
main(int argc, char **argv)
{
  std::cout << argv[0] << " start" << std::endl;
  double row_prices[ROWS];
  if (initialiseRowPrices(row_prices, LENGTH(row_prices)) == false)
    return -1;

  theatre_seats theatreSeats(row_prices);

  while (true) {
    std::cout << theatreSeats.displaySeating() << std::endl
	      << theatreSeats.displayPrices() << std::endl
	      << "TOTAL TICKETS SOLD: " << theatreSeats.getSeatsSold() << std::endl
	      << "TOTAL REVENUE: $" << dollar(theatreSeats.getTotalRevenue()) << std::endl;

    std::string another = anotherTicket();
    std::cout << STR(another) << std::endl;
    if ((another == "n") || 
	(another == "N") || 
	(another == "no") || 
	(another == "NO"))
      break;

    if ((another == "y") || 
	(another == "Y") || 
	(another == "yes") || 
	(another == "YES")) {

      unsigned row = enterRow();
      if (row < ROWS) {
	unsigned column = enterColumn();
	if (column < COLUMNS) {
	  if (theatreSeats.getSeat(row, column) == true) {
	    std::cout << "Seat has been sold, select another" << std::endl;
	  } else {
	    std::cout << "You have bought seat " << column
		      << " in row " << row << " for $ " << row_prices[row] << std::endl;
	    theatreSeats.setSeat(row, column, true);
	  }
	} else {
	  std::cout << "Invalid seat" << std::endl;
	}
      } else {
	std::cout << "Invalid row" << std::endl;
      }
    }

  }
  //theatreSeats.setSeat(1, 1, true);
  //theatreSeats.setSeat(14, 19, true);

  std::cout << theatreSeats.displaySeating() << std::endl
	    << theatreSeats.displayPrices() << std::endl
	    << "TOTAL TICKETS SOLD: " << theatreSeats.getSeatsSold() << std::endl
	    << "TOTAL REVENUE: $" << dollar(theatreSeats.getTotalRevenue()) << std::endl;

  std::cout << argv[0] << " stop" << std::endl;
}
