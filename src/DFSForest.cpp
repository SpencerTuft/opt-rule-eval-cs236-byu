//
// Created by Spencer Tuft on 2018-12-13.
//

#include "DFSForest.h"
bool includes(std::vector<int> vec, int item) {
  for (auto &element : vec) {
    if (element == item) return true;
  }
  return false;
};

DFSForest::DFSForest(DependencyGraph dependency_graph, std::vector<int> row_list) {
  std::vector<int> visited;
  for (auto &row_num : row_list) {
    if (!includes(visited, row_num)) {
      std::vector<int> connected_component;
      dfs_forest_util(dependency_graph, visited, connected_component, row_num);
      f.emplace_back(connected_component);
    }
  }
}
std::vector<int> DFSForest::generate_topological_ordering() {
  std::stack<int> s;
  std::vector<int> result;

  for (auto &component : f) {
    for (auto i = static_cast<unsigned int>(component.size()); i != 0; i--) {
      s.push(component[i-1]);
    }
  }

  while (!s.empty()) {
    result.push_back(s.top());
    s.pop();
  }

//  std::stringstream ss;
//  ss << "Topological Ordering" << std::endl;
//  for (int i = 0, max = static_cast<int>(result.size()); i < max; i++) {
//    ss << "R" << result[i];
//    if (i != max -1) ss << " -> ";
//  }
//  std::cout << ss.str() << std::endl << std::endl;

  return result;
}
std::vector<std::set<int>> DFSForest::generate_strongly_connected_components() {
  std::vector<std::set<int>> strongly_connected_components;
  for (auto &component : f) {
    std::set<int> strongly_connected_component;
    for (auto &node : component) {
      strongly_connected_component.emplace(node);
    }
//    std::cout << "Number of nodes in component: " << strongly_connected_component.size() << std::endl;
    strongly_connected_components.emplace_back(strongly_connected_component);
  }
//  std::cout << "Number of strongly connected components: " << strongly_connected_components.size() << std::endl;

//  std::stringstream ss;
//  ss << "Strongly Connected Components" << std::endl;
//  for (int i = 0, max = static_cast<int>(strongly_connected_components.size()); i < max; i++) {
//    ss << "{";
//    for (auto &node : strongly_connected_components[i]) {
//      ss << " R" << node << " ";
//    }
//    ss << "}";
//    if (i != max) ss << std::endl;
//  }
//  std::cout << ss.str() << std::endl;

  return strongly_connected_components;
}
std::string DFSForest::str() {
  std::stringstream ss;
  for (auto &component : f) {
    for (int i = 0, max = static_cast<int>(component.size()); i < max; i++) {
      ss << "R" << component[i];
      if (i != max-1) ss << " -> ";
    }
    ss << std::endl;
  }
  return ss.str();
}
void DFSForest::dfs_forest_util(DependencyGraph &dependency_graph,
                                std::vector<int> &visited,
                                std::vector<int> &connected_component,
                                int row_num) {
  if (!includes(visited, row_num)) {
    connected_component.emplace_back(row_num);
    visited.emplace_back(row_num);
    auto dl = dependency_graph.get_dependency_list();
    auto row_found = dl.find(row_num);
    if (row_found != dl.end()) {
      auto row_num_list = row_found->second;
      for (auto &row_num : row_num_list) {
        dfs_forest_util(dependency_graph, visited, connected_component, row_num);
      }
    }
  }
}
