#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include <stdexcept>

int FindGCF(int a, int b) {
  // Finds the greatest common factor of 2 integers. Used for reducing
  // fractions.
  // Used in pair_to_fraction() function
  int gcf;
  if (a == 0) return b;
  if (b == 0) return a;
  if (a == b) {
    gcf = a;
  }
  while (a != b) {
    if (a > b) {
      a = a - b;
    }
    if (b > a) {
      b = b - a;
    }
    if (a < 0 || b < 0) {
      gcf = 1;
    }
    if (a == b) {
      gcf = a;
    }
  }
  return gcf;
}

std::string pair_to_fraction(std::pair<int, int> const& p) {
  // Takes a pair of two integers and returns a string representative of those
  // integers in a fraction.
  // Reduces fractions using their gcf, also converts fractions to mixed or
  // whole numbers if necessary
  int gcf = FindGCF(p.first, p.second);
  int int_numer = p.first / gcf;
  int int_denom = p.second / gcf;
  std::string numerator = std::to_string(int_numer);
  if (int_denom == 1) {
    return numerator;
  }
  std::string denominator = std::to_string(int_denom);
  if (int_numer > int_denom) {
    std::string whole_num = std::to_string(int_numer / int_denom);
    std::string numerator = std::to_string(int_numer % int_denom);
    if (numerator == "0") {
      return whole_num;
    } else {
      return whole_num + "-" + numerator + "/" + denominator;
    }
  } else {
    return numerator + "/" + denominator;
  }
}

std::string RemoveWhitespace(std::string const& str) {
  // Removes whitespace and tab characters from beginning and ending of a string
  int i = 0;
  int j = str.length() - 1;
  std::string astr = str;
  while (str.at(i) == ' ' || str.at(i) == '\t') {
    i++;
  }
  while (astr.back() == ' ' || astr.back() == '\t') {
    astr.erase(j);
    j--;
  }
  std::string nowhitespace = astr.substr(i, -1);
  return nowhitespace;
}

std::string RemoveSpaces(std::string const& str) {
  // Removes spaces and tabs from only the beginning of a string
  std::string spaces_removed;
  for (int i = 0; i < static_cast<int>(str.length()); i++) {
    if (str[i] != ' ' && str[i] != '\t') {
      spaces_removed = str.substr(i, -1);
      break;
    }
  }
  return spaces_removed;
}

class Ingredient {
  // Ingredient class that is capable of storing ingredients in a relevant way
  // where their portion, unit, and name are accessable separately as member
  // objects
  // Iterates through ingredient strings and removes whitespace to store only
  // the relevant information in each object. Most of the work is done in the
  // constructor
 public:
  std::pair<int, int> portion;
  std::string unit_;
  std::string name_;
  Ingredient(std::string str) {
    int space1 = str.find(" ");
    std::string portion_str = str.substr(0, space1);
    if (portion_str.find("/") != std::string::npos) {
      if (portion_str.find("-") != std::string::npos) {
        int hyphen = portion_str.find("-");
        int whole_num = stoi(portion_str.substr(0, hyphen));
        int numerator_index = portion_str.find("/");
        int numerator = stoi(portion_str.substr(hyphen + 1, numerator_index));
        int divisor = stoi(portion_str.substr(numerator_index + 1, -1));
        int new_numer = numerator + whole_num * divisor;
        portion = std::make_pair(new_numer, divisor);
      } else {
        int numerator_index = portion_str.find("/");
        int numerator = stoi(portion_str.substr(0, numerator_index));
        int divisor = stoi(portion_str.substr(numerator_index + 1, -1));
        portion = std::make_pair(numerator, divisor);
      }
    } else {
      portion.first = stoi(portion_str);
      portion.second = 1;
    }
    std::string no_portion_spaces = str.substr(space1, -1);
    std::string no_portion = RemoveSpaces(no_portion_spaces);
    int space2 = no_portion.find(" ");
    unit_ = no_portion.substr(0, space2);
    std::string no_unit = no_portion.substr(space2, -1);
    name_ = RemoveSpaces(no_unit);
  }
  friend std::ostream& operator<<(std::ostream& out, Ingredient ingredient) {
    // Defines output behavior for Ingredient types
    out << pair_to_fraction(ingredient.portion) << " " << ingredient.unit_
        << " " << ingredient.name_ << std::endl;
    return out;
  }
  // Used for comparison of ingredient names for sorting ingredients when adding
  // to pantry in RecipeApp
  friend bool operator<(Ingredient const& i1, Ingredient const& i2) {
    return (i1.name_ < i2.name_);
  }
};
