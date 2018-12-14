//
// Created by Spencer Tuft on 2018-12-12.
//

#ifndef OPT_RULE_EVAL_CS236_BYU_GRAPH_H
#define OPT_RULE_EVAL_CS236_BYU_GRAPH_H

#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "Rule.h"

class DependencyGraph {
 private:
  std::vector<Rule> r;
  std::map<std::string, std::vector<int>> rule_name_to_row_number;
  std::map<int, std::set<int>> dependency_list;

  std::map<std::string, std::vector<int>> create_rule_name_number_list();
  std::map<int, std::set<int>> create_dependency_list();
 public:
  explicit DependencyGraph(std::vector<Rule>);
  explicit DependencyGraph(std::map<int, std::set<int>>);
  std::vector<int> get_numbers();
  DependencyGraph invert();
  std::map<int, std::set<int>> get_dependency_list();
  std::string str();
};

#endif //OPT_RULE_EVAL_CS236_BYU_GRAPH_H
