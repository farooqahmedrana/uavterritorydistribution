/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Computer Science Department, FAST-NU, Lahore.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Farooq Ahmed <farooq.ahmad@nu.edu.pk>
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
	void postprocess(Allocation& solution,float maxDistance, Allocation& output);
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

class MonteCarloStrategy : public GreedyStrategy {
private:
	int iterations;

protected:
	virtual void execute(float maxDistance);
public:
	MonteCarloStrategy(Graph* graph,PathStrategy* strategy,int iter) : GreedyStrategy(graph,strategy) { iterations = iter; }
};

class SimulatedAnnealingStrategy : public PartitionStrategy {
private:
	void initsol(GraphNode* node,float maxDistance,Allocation& sol);
	Allocation neighbor(Allocation& current,float maxDistance);

protected:
	virtual void solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final);
public:
	SimulatedAnnealingStrategy(Graph* graph,PathStrategy* strategy) : PartitionStrategy(graph,strategy) { }

};

class GeneticAlgorithmStrategy : public PartitionStrategy {
private:
	void initsol(GraphNode* node,float maxDistance,Allocation& sol);
	void initpopulation(GraphNode* node,float maxDistance,Allocation** population);
	void crossover(Allocation& parentA,Allocation& parentB,Allocation& childA,Allocation& childB);
	void mutate(Allocation& chromosome,float maxDistance);
	vector<Allocation*> selection(vector<Allocation*> initGen, vector<Allocation*> newGen );

	static bool compare(Allocation* a,Allocation* b);

	static const int SIZE = 100;
	static const int ITERATIONS = 10;
	static const int ELITISM_PERCENTAGE = 10;
	static const float MUTATION_PROBABILITY = 0.3;

protected:
	virtual void solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final);
public:
	GeneticAlgorithmStrategy(Graph* graph,PathStrategy* strategy) : PartitionStrategy(graph,strategy) { }


};


#endif /* PARTITIONSTRATEGY_H_ */

