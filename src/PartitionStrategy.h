/*
 * PartitionStrategy.h
 *
 *  Created on: Jul 24, 2017
 *      Author: farooq
 */

#ifndef PARTITIONSTRATEGY_H_
#define PARTITIONSTRATEGY_H_

#include <vector>
#include <map>
#include <string>

#include "GraphNode.h"
#include "GraphEdge.h"
#include "Path.h"
#include "PathStrategy.h"
#include "Allocation.h"

using namespace std;

class Graph;

class PartitionStrategy {
protected:
	Graph* graph;
	map<string,GraphNode*>* nodes;
	map<string,GraphEdge*>* edges;
	map<string,vector<Path*>*> nodePaths;
	PathStrategy* pathStrategy;

	vector<Path*>* getAllPathsOfNode(GraphNode* node,float maxDistance);
	void filterFeasiblePaths(Allocation& allocation, vector<Path*>& allPaths, vector<Path*> & consideredPaths);
	vector<GraphEdge*> findUnvisitedEdges(Allocation& allocation);
	vector<GraphNode*> findNodesOfUnvisitedEdges(Allocation& allocation);
	void postprocess(Allocation& solution,float maxDistance);
	string bfsPath(GraphNode* node,float maxDistance,Set& edgeVisitMap);

	virtual void solve(GraphNode* node,float maxDistance,Allocation solution,Allocation& currentSolution) = 0;

public:
	PartitionStrategy(Graph* graph,PathStrategy* pathStrategy);
	virtual void execute(float maxDistance);
	virtual ~PartitionStrategy();
};

class OptimalStrategy : public PartitionStrategy {
private:
	void findOptimalDisjointPathsSolution(GraphNode* node,float maxDistance,Allocation temp,Allocation& final,int level);
protected:
	virtual void solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final);
public:
	OptimalStrategy(Graph* graph,PathStrategy* strategy) : PartitionStrategy(graph,strategy) { }
};

class GreedyStrategy : public PartitionStrategy {
private:
	void findGreedyDisjointPathsSolution(GraphNode* node,float maxDistance,Allocation temo,Allocation& final);
protected:
	virtual void solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final);
public:
	GreedyStrategy(Graph* graph,PathStrategy* strategy) : PartitionStrategy(graph,strategy) { }
};

#endif /* PARTITIONSTRATEGY_H_ */

