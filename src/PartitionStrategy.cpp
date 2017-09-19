/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Computer Science Department, FAST-NU, Lahore.
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
#include <iostream>
#include <queue>
#include <sstream>
#include "PartitionStrategy.h"
#include "Graph.h"

PartitionStrategy::PartitionStrategy(Graph* graph,PathStrategy* pathStrategy) {
	this->graph = graph;
	this->nodes = &graph->nodes;
	this->edges = &graph->edges;
	this->pathStrategy = pathStrategy;
}

void PartitionStrategy::filterFeasiblePaths(Allocation& allocation, vector<Path*>& allPaths , vector<Path*> & consideredPaths)
{
    for(int i = 0;i < allPaths.size();i++){
        if(allocation.checkDisjointPathProperty(allPaths[i])){
            consideredPaths.push_back(allPaths[i]);
        }
    }
}

vector<GraphEdge*> PartitionStrategy::findUnvisitedEdges(Allocation& allocation) {

	vector<GraphEdge*> unvisitedEdges;
	Set visitedEdges = allocation.getVisitedEdges();

	for(map<string,GraphEdge*>::iterator iter = edges->begin(); iter != edges->end(); iter++){
		if(!visitedEdges.exists(iter->first)){
			unvisitedEdges.push_back(iter->second);
		}
	}

	return unvisitedEdges;
}

vector<GraphNode*> PartitionStrategy::findNodesOfUnvisitedEdges(Allocation& allocation) {

	vector<GraphNode*> unvisitedEdgesNodes;

	vector<GraphEdge*> unvisitedEdges = findUnvisitedEdges(allocation);
	map<string,GraphNode*> unvisitedEdgesNodesMap;

	for (int i=0; i < unvisitedEdges.size(); i++){
		unvisitedEdgesNodesMap[unvisitedEdges[i]->getFromNode()->getId()] = unvisitedEdges[i]->getFromNode();
		unvisitedEdgesNodesMap[unvisitedEdges[i]->getToNode()->getId()] = unvisitedEdges[i]->getToNode();
	}

	for (map<string,GraphNode*>::iterator iter = unvisitedEdgesNodesMap.begin();iter != unvisitedEdgesNodesMap.end(); iter++){
		unvisitedEdgesNodes.push_back(iter->second);
	}

	return unvisitedEdgesNodes;
}


vector<Path*>* PartitionStrategy::getAllPathsOfNode(GraphNode* node,float maxDistance){
	if(nodePaths.count(node->getId()) == 0)
	{
		vector<Path*>* allPathsOfThisNode = new vector<Path*>();

		if (pathStrategy != NULL){
			pathStrategy->execute(node,maxDistance,*allPathsOfThisNode);
		}
		nodePaths[node->getId()] = allPathsOfThisNode;
	}

	return nodePaths[node->getId()];
}


void PartitionStrategy::postprocess(Allocation& allocation,float maxDistance){

	vector<GraphNode*> unvisitedEdgesNodes = findNodesOfUnvisitedEdges(allocation);
	Set visitedEdges = allocation.getVisitedEdges();

	for(int i=0; i < unvisitedEdgesNodes.size(); i++){
		string pathString = bfsPath(unvisitedEdgesNodes[i],maxDistance,visitedEdges);
		if (!pathString.empty()){
			cout << pathString << endl;
		}
	}

}

string PartitionStrategy::bfsPath(GraphNode* node,float maxDistance,Set& visitedEdges){
	double currentDistance = 0;
	queue<GraphNode*> nodeQueue;
	GraphNode* current;
	stringstream pathBuffer;

	nodeQueue.push(node);

	do {
		current = nodeQueue.front();
		nodeQueue.pop();
		vector<GraphEdge*> nodeEdges = current->getEdges();

		for(int i=0; i < nodeEdges.size(); i++){
			GraphEdge* edge = nodeEdges[i];
			if ( ! edge->existsUndirectedIn(visitedEdges) &&
					(currentDistance + 2*edge->distance()) <= maxDistance){

				edge->addUndirectedIn(visitedEdges);
				currentDistance += 2*edge->distance();
				pathBuffer << edge->getUndirectedPathString();

				nodeQueue.push(edge->getToNode());
			}
		}

	} while(!nodeQueue.empty());

	return pathBuffer.str();
}

void PartitionStrategy::execute(float maxDistance){

	Allocation currentSolution;
	Allocation solution;

	cout << "*** finding disjoint paths ***" << endl;

	solve(nodes->begin()->second,maxDistance,solution,currentSolution);

	cout << "*** printing solution ***" << endl;
	currentSolution.print();

	cout << "*** post processing ***" << endl;
	postprocess(currentSolution,maxDistance);

}

PartitionStrategy::~PartitionStrategy() {

	for(map<string,vector<Path*>*>::iterator iter = nodePaths.begin();iter != nodePaths.end();iter++){
		vector<Path*>* pathVector = iter->second;
		for(int i=0; i < pathVector->size(); i++){
			if ((*pathVector)[i] != NULL){
				delete (*pathVector)[i];
			}
		}
		delete iter->second;
	}

}


void OptimalStrategy::solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final){
	findOptimalDisjointPathsSolution(node,maxDistance,temp,final,1);
}

void OptimalStrategy::findOptimalDisjointPathsSolution(GraphNode* node,float maxDistance,Allocation temp,Allocation& final,int level){

	vector<Path*>* allPathsOfThisNode = getAllPathsOfNode(node,maxDistance);
	vector<Path*> consideredPaths;  // that satisfy the bound - for saving space

    filterFeasiblePaths(temp, *allPathsOfThisNode , consideredPaths);

	for(int i=0; i < consideredPaths.size(); i++){

		temp.addPath(consideredPaths[i]);
		vector<GraphNode*> unvisitedNodes = findNodesOfUnvisitedEdges(temp);

		if(unvisitedNodes.size() > 0){
			for(int j=0; j < unvisitedNodes.size(); j++){

				findOptimalDisjointPathsSolution(unvisitedNodes[j],maxDistance,temp,final,level+1);

				if (temp.compare(final)){  // new allocation (temp) is better than current allocation (final), increasing the coverage with fewer or equal uavs
					final = temp;
				}
			}
		}
		else{
			if (temp.compare(final)){  // new allocation (temp) is better than current allocation (final), increasing the coverage with fewer or equal uavs
				final = temp;
			}
		}

		temp.removeLastPath(); // using allocation as a stack
	}

}

void GreedyStrategy::solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final){
	findGreedyDisjointPathsSolution(node,maxDistance,temp,final);
}

void GreedyStrategy::findGreedyDisjointPathsSolution(GraphNode* node,float maxDistance,Allocation temp,Allocation& final){

	Set visitedNodesId;
	for (map<string,GraphNode*>::iterator nodeIterator = nodes->begin(); nodeIterator != nodes->end(); nodeIterator++){
		visitedNodesId.add(nodeIterator->first);
	}


	do {

		vector<Path*>* allPathsOfThisNode = getAllPathsOfNode(node,maxDistance);
		vector<Path*> consideredPaths;  // that satisfy the bound - for saving space

		filterFeasiblePaths(temp, *allPathsOfThisNode, consideredPaths);

		// find path with max edge size
		int maxEdgeSize = 0;
		Path* consideredPath = NULL;
		for(int i=0; i < consideredPaths.size(); i++){
			if (consideredPaths[i]->size() > maxEdgeSize){
				consideredPath = consideredPaths[i];
				maxEdgeSize = consideredPaths[i]->size();
			}
		}

		if (consideredPath != NULL){
			temp.addPath(consideredPath);
		}


		// reclaim memory
		for(int i=0; i < allPathsOfThisNode->size(); i++){
			if ( consideredPath == NULL || (consideredPath != NULL && !allPathsOfThisNode->at(i)->equal(consideredPath)) ){
				delete (*allPathsOfThisNode)[i];
				(*allPathsOfThisNode)[i] = NULL;
			}
		}

		visitedNodesId.remove(node->getId());
		if (visitedNodesId.size() > 0){
			node = (*nodes)[visitedNodesId.random()];
		}

	} while(visitedNodesId.size() > 0);

	final = temp;
}

