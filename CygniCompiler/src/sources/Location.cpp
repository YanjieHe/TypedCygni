#include "Location.hpp"
#include "Exception.hpp"

Location::Location(LocationKind locationKind, int index)
    : locationKind{locationKind}, index{index} {}

String Location::ToString() const {
  switch (locationKind) {
    case LocationKind::Global:
      return String("Global, ") + String::ToString(index);
    case LocationKind::Class:
      return String("Class, ") + String::ToString(index);
    case LocationKind::Module:
      return String("Module, ") + String::ToString(index);
    case LocationKind::Function:
      return String("Function, ") + String::ToString(index);
    default:
      throw NotImplementedException();
  }
}
