#pragma once
#include "utility.h"

class Recipe {
 private:
  std::string recipe_name_;
  int num_servings_;
  std::vector<Ingredient> ingredients_;
  std::vector<std::string> instructions_;

 public:
  Recipe(std::string const& r_name, int const& num_servings) {
    std::string nows_r_name = RemoveWhitespace(r_name);
    recipe_name_ = nows_r_name;
    num_servings_ = num_servings;
  }
  void AddIngredient(std::string const& ingredient) {
    std::string nows_ingredient = RemoveWhitespace(ingredient);
    Ingredient new_ingredient(nows_ingredient);
    ingredients_.push_back(new_ingredient);
  }
  void SetInstructions(std::string const& instr) {
    // Uses an istringstream to create the instructions string and eliminate
    // extra whitespace and tab characters. Pushes back instructions into a
    // vector.
    std::istringstream in;
    in.str(instr);
    std::string t;
    while (std::getline(in, t, '\n')) {
      std::string instructions, word;
      std::istringstream iss(t);
      while (iss >> word) {
        instructions += word + " ";
      }
      instructions = instructions.substr(0, instructions.size() - 1);
      if (instructions.size() > 1) {
        instructions_.push_back(instructions);
      }
    }
  }
  std::string IngredientInOneServing(std::string const ingredient) {
    // Takes an ingredient (as a string) as a parameter and returns the
    // ingredient (still as a string type) with a changed portion equal to the
    // amount needed for a single serving
    std::string return_string = "";
    std::pair<int, int> portion_pair;
    try {
      int ingredient_found_check = 0;
      for (int i = 0; i < static_cast<int>(ingredients_.size()); i++) {
        if (ingredient == ingredients_[i].name_) {
          portion_pair =
              std::make_pair(ingredients_[i].portion.first,
                             (ingredients_[i].portion.second * num_servings_));
          return_string = pair_to_fraction(portion_pair) + " " +
                          ingredients_[i].unit_ + " " + ingredient;
          ingredient_found_check = 1;
        }
      }
      if (ingredient_found_check == 0) {
        throw std::invalid_argument("Ingredient Not Found");
      }
    } catch (std::domain_error) {
    }
    return return_string;
  }
  void ChangeServings(int servings) {
    // Used in UseUpIngredients function
    for (int i = 0; i < static_cast<int>(ingredients_.size()); i++) {
      int numerator = ingredients_.at(i).portion.first;
      int denominator = ingredients_.at(i).portion.second;
      ingredients_.at(i).portion.first = numerator * servings;
      ingredients_.at(i).portion.second = denominator * num_servings_;
    }
    num_servings_ = servings;
  }

  bool HasIngredient(Ingredient const& ingredient) {
    // Used to check if the ingredient exists in the ingredient vector (if its
    // in the recipe)
    // Used in function UseUpIngredient() in the RecipeApp class
    for (int i = 0; i < static_cast<int>(ingredients_.size()); i++) {
      if (ingredient.name_ == ingredients_.at(i).name_) {
        return true;
      }
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& out, const Recipe& r) {
    out << "Recipe for: " << r.recipe_name_ << std::endl;
    out << "Serves " << r.num_servings_ << std::endl;
    out << "Ingredients:" << std::endl;
    for (int i = 0; i < static_cast<int>(r.ingredients_.size()); i++) {
      out << r.ingredients_.at(i);
    }
    out << std::endl << "Instructions:" << std::endl;
    std::vector<std::string> instructions_vec = r.instructions_;
    for (int i = 0; i < static_cast<int>(instructions_vec.size()); i++) {
      out << instructions_vec.at(i) << "\n";
    }
    out << "\n";
    return out;
  }
  friend bool operator<(Recipe const& r1, Recipe const& r2) {
    // Used for sorting Recipe objects in AddRecipe() function in RecipeApp
    // class
    return (r1.recipe_name_ < r2.recipe_name_);
  }
};
