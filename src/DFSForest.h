//
// Created by Spencer Tuft on 2018-12-13.
//

#ifndef OPT_RULE_EVAL_CS236_BYU_DFSFOREST_H
#define OPT_RULE_EVAL_CS236_BYU_DFSFOREST_H

#include <stack>
#include <sstream>
#include "./DependencyGraph.h"

class DFSForest {
 private:
  std::vector<std::vector<int>> f; //forest
  void dfs_forest_util(DependencyGraph &, std::vector<int> &, std::vector<int> &, int);
 public:
  explicit DFSForest(DependencyGraph, std::vector<int>);
  std::vector<int> generate_topological_ordering();
  std::vector<std::set<int>> generate_strongly_connected_components();
  std::string str();
};

#endif //OPT_RULE_EVAL_CS236_BYU_DFSFOREST_H
