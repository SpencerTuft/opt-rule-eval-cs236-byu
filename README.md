# opt-rule-eval-cs236-byu

### Introduction
Right now the Datalog Interpreter evaluates all the rules en masse, and it re-evaluates all the rules if any new tuple is created. This brute force iterative evaluation of rules to a fix-point is needlessly expensive when dealing with large relations. The purpose of this project is to derive an optimal order of evaluation on rules sufficient to determine which require iteration and which do not. Facts are generated using those rules and order before answering all the queries.

### Table of Contents
- [Project Description](#project-description)
- [Requirements](#requirements)
  * [Part 1 Strongly connected components](#part-1-strongly-connected-components)
  * [Part 2 Optimizing rule evaluation](#part-2-optimizing-rule-evaluation)
- [Rule Dependency Graph](#rule-dependency-graph)
- [Rule Grouping and Evaluation Order](#rule-grouping-and-evaluation-order)
- [Rule evaluation](#rule-evaluation)
- [FAQ](#faq)


### Project Description
The project is divided into three phases: building a rule dependency graph, finding the strongly connected components of the rule dependency graph to determine order and grouping in the rule evaluation, and then evaluating each group of rules to a fix-point in the specified order. Each of these phases will be treated separately. Once all the facts from the rules have been generated, the queries are answered as before.

### Requirements
#### Part 1 Strongly connected components
Implement a graph class to support the rule optimization algorithm. The class must be able to

- Report a topological ordering of all nodes in the graph (using post-order traversal, see Part 2 for more details)
- Invert the graph (reverse all the edges)
- Compute the set of all strongly connected components

As in previous projects, the graph class must include at least 10 tests to validate functionality; although, more very well may be needed given the complexity of the graph object.

- *Command line*: no command line arguments
- *Input*: none
- *Output*: A pass/fail report for each test, based on comparison of results with expected output

The pass-off is based on the quality of tests and whether or not the solution passes.


#### Part 2 Optimizing rule evaluation
Use the graph from part 1 to evaluate rules in an optimal way. Use the datalog object to build the rule dependency graph, and then analyze that graph for the strongly connected components that define the rule evaluation order (see below). As the graph is a generic object, with no knowledge of rules, it will be necessary to create a mapping between graph nodes and rules.

The output is the same as that in datalog-interpreter with the addition of the rule-graph definition the number of passes on each strongly connect component with the rules in that component:
```
Dependency Graph
 R0:

 Rule Evaluation
 1 passes: R0
```

The input to output relation is given more precisely in the example section. As before, order matters! Be especially careful that when you have multiple rules in a SCC that you output them in least to greatest order numerically. See below for more details.

- *Command line*: a single argument indicating the input file
- *Input*: a valid datalog program
- *Output*: see the output specifications and examples

Part 2 is scored on a set of 10 private tests at submission. *The time limit is absolute*.

### Rule Dependency Graph
Each rule is assigned a unique ID. For this project, each rules is assigned an ID in program order starting from zero. In general, the output displays the rule IDs with a preceding R as in R0 for the zero-th rule in program order, R1 for the one-th rule in program order, etc. The preceding R is for decoration only and is not part of the ID. Each rule ID is a vertex in a graph. The edges between rule IDs come directly from the actual rules. An edge from one rule, Ri, to another rule, Rj, is added to the graph if rule Ri uses a relation that is the named target of rule Rj. Consider the following rules as an example:
```
A(X,Y) :- B(X,Y), C(X,Y). # R0
B(X,Y) :- A(X,Y), D(X,Y). # R1
B(X,Y) :- B(Y,X).         # R2
E(X,Y) :- F(X,Y), G(X,Y). # R3
E(X,Y) :- E(X,Y), F(X,Y).  # R4
```

The vertices for the rule dependency graph are {R0, R1, R2, R3, R4}. Vertex R0 has an edge to R1 and another edge to R2 since both those rules name the relation B as the target, and R0 depends on the relation B. The other dependency, relation C, is not named as the target in any rule, so it does not contribute an edge in the graph: that relation is static. The final dependency graph as an adjacency list is:
```
Dependency Graph
R0:R1,R2
R1:R0
R2:R1,R2
R3:
R4:R3,R4
```

### Rule Grouping and Evaluation Order
The grouping of rules and the order of evaluation is computed using an algorithm to find strongly connected components (SCCs). All of the graph concepts are defined in Chapter 11 of the course text. The full presentation of the algorithm to compute SCCs with its proof is found in section 3.4 of [Algorithms](https://people.eecs.berkeley.edu/~vazirani/algorithms/chap3.pdf) by Dasgupta, C. H. Papadimitriou, and U. V. Vazirani. A simple presentation is included here for convenience, but it is strongly recommended to read the [full presentation](https://people.eecs.berkeley.edu/~vazirani/algorithms/chap3.pdf) in section 3.4.

A strongly connected component is a maximal set of vertices such that every vertex in the set can reach every other vertex in the set through some path that never leaves the set. The SCCs in a graph partition the vertices into disjoint sets. The algorithm to computes SCCs proceeds in three distinct steps

- Create the reverse graph: all the edges are reversed
- Compute the topological ordering for the vertices using the reversed graph (the order of leaving the vertices in the depth-first search)
- Following the topological ordering on vertices from the last vertex the search left to the first vertex the search left, start a depth-first search on the original graph, and any vertex visited in that search belongs to the SCC. Repeat until all the vertices are visited.

Each step merits some further explanation.

Reversing the edges in step 1 is very direct. Returning to the example from the previous section, the reversed graph as an adjacency list is
```
Reverse Dependency Graph
R0: R1
R1: R0,R2
R2: R0,R2
R3: R4
R4: R4
```

The reverse graph is the graph used to compute the topological ordering. It is tempting to compute this ordering on the original graph, and in many cases it works, but such an approach does not work for any arbitrary graph (see section 3.4 of Algorithms for an explanation). It is equally tempting to try to follow the edges backwards on the original graph, but why deal with such added complexity?

The topological ordering is computed by looping through each vertex in the graph and performing a depth-first search starting from that vertex if it has not already been visited in a previous search. During post-order traversal, push each vertex onto a stack when the depth-first search leaves the vertex after visiting it. The topological ordering is based on the order of the vertices in the stack after all nodes have been visited. Be sure that the vertices are looped through in the same order that the rules appear in the Datalog program. For the example, the topological ordering, having started the depth-first search from vertex R0 is
```
POTN(R2) = 0
POTN(R1) = 1
POTN(R0) = 2
POTN(R4) = 3
POTN(R3) = 4
```

The last step to compute SCCs starts a depth-first search from the greatest post-order traversal number (POTN) to the least POTN on the original graph. In the view of a stack, that is the top of the stack to the bottom of the stack. The process is repeated until everything is visited in the graph. This produces a DFS forest where each tree represents one SCC. For the running example, the first depth-first search is started from vertex R3 to create the SCC of {R3}. The R3 vertex is removed from the stack. R4 is now the top, the depth-first search creates the SCC {R4}, and it is then removed from the stack as well. R0 is now the top of the stack, so a depth-first search is started at R0. The depth-first search must not reset the visited information between searches. The visited information is what prevents the search from moving into another already completed SCC. The second depth-first search from R0 yields the SCC {R0, R1, R2}. The vertex is popped from the stack leaving R1 on top. R1 has already been visited so it is popped. The same is true for R2. If you printed out the strongly connected components in the order you discovered them it would look like:
```
Strongly Connected Components:
{R3}
{R4}
{R0, R1, R2}
```

### Rule evaluation
Rules are evaluated in groups defined by the SCCs, and the SCCs are processed in the order of their discovery from the algorithm (FIFO order). A given SCC is called trivial if it only contains a single rule and there is no self-loop on that rule in the dependency graph. A trivial SCC requires no iteration when evaluating the associated rule. A single rule evaluation is sufficient. A given non-trivial SCC must evaluate the associated rules in the SCC until a fix-point is reached. Both SCCs in the example are non-trivial and require iteration. All facts are known once the SCCs are processed. Evaluate rules in an SCC following the order of the Rule IDs.

### FAQ
*How should rules be assigned rule IDs?*

Rule IDs should be assigned following program order starting from zero. Rule IDs should be integers and not strings. The proceeding R in the output is just a decoration.

*What order should be followed when printing output or starting depth-first searches?*

Always follow vertex order from least to greatest according to the rule IDs. Also, since the R is a decoration on the output, vertex 2 is ordered before vertex 10! When printing the dependency graph as an adjacency list, edges should also follow vertex order. In a depth-first search, when there is a choice of two edges to follow, for example vertex R0 can go to either vertex R2 or vertex R4, again follow vertex order and visit R2 first.

*What is the output for this project?*

The output is the dependency graph, followed by the number of iterations on each SCC with the rule IDs in that SCC, followed by the actual query results. The SCC order is the evaluation order as determined by the SCC algorithm: FIFO on the SCCs as they are discovered. See the examples above for further clarification.

*Is there a time-bound for pass-off?*

Yes. Please review the project standards for details. The time-bound is strictly enforced.

*I thought my project is set up correctly, but the test cases are taking forever to run. What is happening?*

Are you running your program in Visual Studio? Visual Studio in debug mode tends to slow down the running time of your project by a huge factor. Try running the test cases either outside of Visual Studio (in the command line) or set up Visual Studio to run in release mode (here are instructions from Microsoft on how to do that).

Other things that may be influencing your run time include what computer you are running your program on (run the program on the lab computers for the best approximation of how it will run on the TA computers) and using a vector to store the tuples in each Relation instead of a set (if you have to sort the vector manually, that can really slow down run time).

[Back to Top](#opt-rule-eval-cs236-byu)