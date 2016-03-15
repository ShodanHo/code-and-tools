#include "string_utils.h"

#include <sstream> // ostringstream

namespace common_string_utils {

std::string toString(unsigned u)
{
  std::ostringstream oss;
  oss << u;
  return oss.str();
}

} // namespace common_string_utils
