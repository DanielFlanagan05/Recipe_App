#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "recipe.h"

bool CompareRecipe(Recipe const& r1, Recipe const& r2) { return r1 < r2; }
bool CompareIngredient(Ingredient const& i1, Ingredient const& i2) {
  return i1 < i2;
}

class RecipeApp {
 private:
  std::vector<Recipe> recipe_list_;
  std::vector<Ingredient> pantry_;

 public:
  RecipeApp() = default;
  void AddRecipe(Recipe const& r) {
    // Adds ingredient to Recipe Vector recipe_list_
    // Sorts while inserting to vector
    std::vector<Recipe>::iterator it = recipe_list_.begin();
    int i = 0;
    while (i < recipe_list_.size()) {
      if (r < recipe_list_.at(i)) {
        recipe_list_.insert(it + i, r);
        it = recipe_list_.begin();
        break;
      }
      i++;
    }
    if (i == recipe_list_.size()) {
      recipe_list_.push_back(r);
    }
  }
  void AddIngredientToPantry(std::string const ingredient) {
    // Adds ingredient to Ingredient Vector pantry_
    // Sorts while inserting to vector
    std::string nows_ingredient = RemoveWhitespace(ingredient);
    Ingredient new_ingredient(nows_ingredient);
    std::vector<Ingredient>::iterator it = pantry_.begin();
    int i = 0;
    while (i < pantry_.size()) {
      if (new_ingredient < pantry_.at(i)) {
        pantry_.insert(it + i, new_ingredient);
        it = pantry_.begin();
        break;
      }
      i++;
    }
    if (i == pantry_.size()) {
      pantry_.push_back(new_ingredient);
    }
  }

  Recipe UseUpIngredient(std::string const str) {
    Ingredient ingredient(str);
    try {
      for (int i = 0; i < recipe_list_.size(); i++) {
        if (recipe_list_.at(i).HasIngredient(ingredient)) {
          Recipe recipe = recipe_list_.at(i);
          Ingredient ingredient_one_serving(
              recipe.IngredientInOneServing(ingredient.name_));
          std::pair<int, int> portion = ingredient.portion;
          std::pair<int, int> portion_one_serving =
              ingredient_one_serving.portion;
          int denominator = portion.second * portion_one_serving.first;
          int numerator = portion.first * portion_one_serving.second;
          int servings = numerator / denominator;
          recipe.ChangeServings(servings);
          return recipe;
        }
      }

      throw std::invalid_argument("Ingredient Not Found");
    } catch (std::domain_error) {
    }
  }
  friend std::ostream& operator<<(std::ostream& out, const RecipeApp& r) {
    out << "Recipes in the app (ordered by name):\n";
    for (int i = 0; i < r.recipe_list_.size(); i++) {
      out << r.recipe_list_.at(i);
    }
    out << "Ingredients in pantry (ordered by name):\n";
    for (int i = 0; i < r.pantry_.size(); i++) {
      out << r.pantry_.at(i);
    }
    return out;
  }
};
