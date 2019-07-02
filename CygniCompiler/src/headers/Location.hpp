#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "String.hpp"

enum class LocationKind { Global, Class, Module, Function };

class Location {
 private:
  LocationKind locationKind;
  int index;

 public:
  Location() : locationKind{LocationKind::Global}, index{INT32_MIN} {}

  Location(LocationKind locationKind, int index);

  String ToString() const;

  LocationKind Kind() const { return locationKind; }

  int Index() const { return index; }
};

#endif  // LOCATION_HPP
