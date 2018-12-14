//
// Created by Spencer Tuft on 2018-12-12.
//

#include "DependencyGraph.h"
DependencyGraph::DependencyGraph(std::vector<Rule> rules) {
  // Save rules
  r = std::move(rules);

  // Create rule-to-row map
  rule_name_to_row_number = create_rule_name_number_list();

  // Create adjacency list
  dependency_list = create_dependency_list();
}
DependencyGraph::DependencyGraph(std::map<int, std::set<int>>d_list) {
  dependency_list = std::move(d_list);
}
std::map<std::string, std::vector<int>> DependencyGraph::create_rule_name_number_list() {
  std::map<std::string, std::vector<int>> list;
  for (int i = 0, max = static_cast<int>(r.size()); i < max; i++) { // Iterate through rules
    std::string rule_name = r[i].getHead().get_id();
    list[rule_name].push_back(i);
  }

  // Testing if the list is right
//  std::stringstream ss;
//  for (auto &row : list) {
//    ss << row.first << ": ";
//    auto ll = row.second;
//    for (size_t i = 0, max = ll.size(); i < max; i++) {
//      ss << ll[i];
//      if (i != max-1) ss << ",";
//    }
//    ss << std::endl;
//  }
//  std::cout << "Rule name to number list" << std::endl << ss.str() << std::endl;

  return list;
}
std::map<int, std::set<int>> DependencyGraph::create_dependency_list() {
  std::map<int, std::set<int>> list;
  for (size_t i = 0, max = r.size(); i < max; i++) { // Iterate through rules
    list[i]; // Create key
    auto predicates = r[i].getList(); // Get predicates
    for (auto &predicate : predicates) { // Iterate through predicates
      auto predicate_name = predicate.get_id();
      auto predicate_found = rule_name_to_row_number.find(predicate_name); // Search for the pred in rule to number map
      if (predicate_found != rule_name_to_row_number.end()) {
        auto predicate_rows_where_defined = predicate_found->second;
        for (auto &num : predicate_rows_where_defined) {
          list[i].emplace(num); // Add row nums to list where predicate is found
        }
      }
    }
  }
  return list;
}
std::vector<int> DependencyGraph::get_numbers() {
  std::vector<int> rule_numbers;
  for (auto &item : dependency_list) {
    rule_numbers.emplace_back(item.first);
  }
  return rule_numbers;
}
DependencyGraph DependencyGraph::invert() {
  std::map<int, std::set<int>> inverted;
  for (auto &item : dependency_list) {
    auto item_list = item.second;
    for (auto &num : item_list) {
      inverted[num].emplace(item.first);
    }
  }
  return DependencyGraph(inverted);
}
std::map<int, std::set<int>> DependencyGraph::get_dependency_list() {
  return dependency_list;
}
std::string DependencyGraph::str() {
  std::stringstream ss;
  for (auto &dependency : dependency_list) {
    ss << "R" << dependency.first << ":";
    auto list = dependency.second;
    for (size_t i = 0, max = list.size(); i < max; i++) {

      ss << "R" << *std::next(list.begin(), i);
      if (i != max-1) ss << ",";
    }
    ss << std::endl;
  }
  return ss.str();
}
