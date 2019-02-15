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
#include <iostream>
#include <queue>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "PartitionStrategy.h"
#include "Graph.h"
#include "Util.h"


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


void PartitionStrategy::postprocess(Allocation& allocatedPaths,float maxDistance, Allocation& output){

	vector<GraphNode*> unvisitedEdgesNodes = findNodesOfUnvisitedEdges(allocatedPaths);
	Set visitedEdges = allocatedPaths.getVisitedEdges();

	for(int i=0; i < unvisitedEdgesNodes.size(); i++){
		string pathString = bfsPath(unvisitedEdgesNodes[i],maxDistance,visitedEdges);
		if (!pathString.empty()){
			Path* p = new Path(pathString);
			output.addPath(p);
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
	Allocation postprocessOutput;

//	cout << "*** finding disjoint paths ***" << endl;

	solve(nodes->begin()->second,maxDistance,solution,currentSolution);

//	cout << "*** printing solution ***" << endl;
	currentSolution.print();

	cout << "*** post processing ***" << endl;
	postprocess(currentSolution,maxDistance,postprocessOutput);
	postprocessOutput.print();


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
	Util::randomseed();
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

void MonteCarloStrategy::execute(float maxDistance){

	Allocation finalMergedResults;
	Allocation finalDistribution;
	Allocation finalPostProcessingOutput;

	for (int i=0; i < iterations; i++){

		Allocation mergedResults;
		Allocation currentDistribution;
		Allocation tempDistribution;
		Allocation postprocessOutput;

		// find a random root node
		Util::randomseed();
		int randomNumber = Util::random(nodes->size());
		map<string,GraphNode*>::iterator iterator = nodes->begin();
		GraphNode* root = iterator->second;
		for(int i=1; i != randomNumber && i < nodes->size(); i++,iterator++){
			root = iterator->second;
		}

		// find a solution
		solve(root,maxDistance,tempDistribution,currentDistribution);
		postprocess(currentDistribution,maxDistance,postprocessOutput);

		// compare and update if better
		mergedResults = currentDistribution;
		mergedResults.merge(postprocessOutput);
		if (mergedResults.compare(finalMergedResults)){
			finalMergedResults = mergedResults;
			finalDistribution = currentDistribution;
			finalPostProcessingOutput = postprocessOutput;
		}

		// cleanup essential
		nodePaths.clear();
	}

	finalDistribution.print();
	cout << "*** post process ***" << endl;
	finalPostProcessingOutput.print();
}


void SimulatedAnnealingStrategy::initsol(GraphNode* node,float maxDistance,Allocation& sol){

	Set visitedNodesId;
	for (map<string,GraphNode*>::iterator nodeIterator = nodes->begin(); nodeIterator != nodes->end(); nodeIterator++){
		visitedNodesId.add(nodeIterator->first);
	}

	do {

		vector<Path*>* allPathsOfThisNode = getAllPathsOfNode(node,maxDistance);
		vector<Path*> consideredPaths;  // that satisfy the bound - for saving space

		filterFeasiblePaths(sol, *allPathsOfThisNode, consideredPaths);

		Path* consideredPath = NULL;

		// select a random path
		if(!consideredPaths.empty()){
			int randomNumber = Util::random(consideredPaths.size());
			consideredPath = consideredPaths[randomNumber];
			sol.addPath(consideredPath);
		}

/*		// reclaim memory
		for(int i=0; i < allPathsOfThisNode->size(); i++){
			if ( consideredPath == NULL || (consideredPath != NULL && !allPathsOfThisNode->at(i)->equal(consideredPath)) ){
				delete (*allPathsOfThisNode)[i];
				(*allPathsOfThisNode)[i] = NULL;
			}
		}
*/
		visitedNodesId.remove(node->getId());
		if (visitedNodesId.size() > 0){
			node = (*nodes)[visitedNodesId.random()];
		}

	} while(visitedNodesId.size() > 0);
}

Allocation SimulatedAnnealingStrategy::neighbor(Allocation& current,float maxDistance){
	Allocation next = current;
	next.removeRandomPath();

	vector<GraphNode*> unvisitedNodes = findNodesOfUnvisitedEdges(next);

	Set unvisitedNodesId;
	for(int i=0; i < unvisitedNodes.size(); i++){
		unvisitedNodesId.add(unvisitedNodes[i]->getId());
	}

	GraphNode* node = (*nodes)[unvisitedNodesId.random()];

	do {

		vector<Path*>* allPathsOfThisNode = getAllPathsOfNode(node,maxDistance);
		vector<Path*> consideredPaths;  // that satisfy the bound - for saving space

		filterFeasiblePaths(next, *allPathsOfThisNode, consideredPaths);


		Path* consideredPath = NULL;

		// select a random path
		if(!consideredPaths.empty()){
			int randomNumber = Util::random(consideredPaths.size());
			consideredPath = consideredPaths[randomNumber];
			next.addPath(consideredPath);
		}
/*
		// reclaim memory
		for(int i=0; i < allPathsOfThisNode->size(); i++){
			if ( consideredPath == NULL || (consideredPath != NULL && !allPathsOfThisNode->at(i)->equal(consideredPath)) ){
				delete (*allPathsOfThisNode)[i];
				(*allPathsOfThisNode)[i] = NULL;
			}
		}
*/
		unvisitedNodesId.remove(node->getId());

		if (unvisitedNodesId.size() > 0){
			node = (*nodes)[unvisitedNodesId.random()];
		}

	} while(unvisitedNodesId.size() > 0);

	return next;
}

void SimulatedAnnealingStrategy::solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final){
	Util::randomseed();

	Allocation current;
	initsol(node,maxDistance,current);

	double T = 100;
	int t = 1;
	while(true){
		T *= (1 - (0.001*t) );
		if (T == 0) break;

		Allocation next = neighbor(current,maxDistance);

		float delta = (current.getVisitedEdges().size() - next.getVisitedEdges().size())*0.5 + (current.size() - next.size())*0.5 ;

		if (delta < 0){
			current = next;
		}
		else{
			float probability = exp(- delta/(t*T));
			if(rand() < probability){
				current = next;
			}
		}

		t++;
	}

	final = current;
}


void GeneticAlgorithmStrategy::initsol(GraphNode* node,float maxDistance,Allocation& sol){

	Set visitedNodesId;
	for (map<string,GraphNode*>::iterator nodeIterator = nodes->begin(); nodeIterator != nodes->end(); nodeIterator++){
		visitedNodesId.add(nodeIterator->first);
	}

	do {

		vector<Path*>* allPathsOfThisNode = getAllPathsOfNode(node,maxDistance);
		vector<Path*> consideredPaths;  // that satisfy the bound - for saving space

		filterFeasiblePaths(sol, *allPathsOfThisNode, consideredPaths);

		Path* consideredPath = NULL;

		// select a random path
		if(!consideredPaths.empty()){
			int randomNumber = Util::random(consideredPaths.size());
			consideredPath = consideredPaths[randomNumber];
			sol.addPath(consideredPath);
		}

		visitedNodesId.remove(node->getId());
		if (visitedNodesId.size() > 0){
			node = (*nodes)[visitedNodesId.random()];
		}

	} while(visitedNodesId.size() > 0);
}

void GeneticAlgorithmStrategy::initpopulation(GraphNode* node,float maxDistance,Allocation** population){

	for(int i=0; i < SIZE; i++){
		population[i] = new Allocation();
		initsol(node,maxDistance,*population[i]);
	}

}

void GeneticAlgorithmStrategy::solve(GraphNode* node,float maxDistance,Allocation temp,Allocation& final){
	Util::randomseed();

	Allocation** population = new Allocation*[SIZE];
	initpopulation(node,maxDistance,population);

	vector<Allocation*> populationVector;
	for (int i = 0; i < SIZE; i++){
		populationVector.push_back(population[i]);
	}

	vector<Allocation*> generationVector;

	for(int i=0; i < ITERATIONS; i++){

		while(populationVector.size() > 0){
			int a = Util::random(populationVector.size());
			Allocation* parentA = populationVector[a];
			populationVector.erase(populationVector.begin() + a);

			int b = Util::random(populationVector.size());
			Allocation* parentB = populationVector[a];
			populationVector.erase(populationVector.begin() + b);

			Allocation* childA = new Allocation();
			Allocation* childB = new Allocation();

			crossover(*parentA,*parentB,*childA,*childB);

			if(rand() < MUTATION_PROBABILITY){
				mutate(*childA,maxDistance);
			}

			if(rand() < MUTATION_PROBABILITY){
				mutate(*childB,maxDistance);
			}

			generationVector.push_back(childA);
			generationVector.push_back(childB);
		}

		populationVector = selection(populationVector,generationVector);

		Allocation* current = populationVector[0];
		for (int i=1; i < SIZE; i++){
			Allocation* next = populationVector[i];

			float delta = (current->getVisitedEdges().size() - next->getVisitedEdges().size())*0.5 + (current->size() - next->size())*0.5 ;

			if (delta < 0){
				current = next;
			}
		}

		if(current->compare(final)){
			final = *current;
		}

		generationVector.clear();
	}
}

void GeneticAlgorithmStrategy::crossover(Allocation& parentA,Allocation& parentB,Allocation& childA,Allocation& childB){
	for(int i=0; i < parentA.size() ; i++){
		if (i % 2 == 0){
			childA.addPath(parentA.getPath(i));

		}
		else{
			childB.addPath(parentA.getPath(i));
		}
	}

	for(int i=0; i < parentB.size() ; i++){
		if (i % 2 == 0){
			if (childA.checkDisjointPathProperty(parentB.getPath(i))){
				childA.addPath(parentB.getPath(i));
			}

		}
		else{
			if (childB.checkDisjointPathProperty(parentB.getPath(i))){
				childB.addPath(parentB.getPath(i));
			}
		}
	}
}

void GeneticAlgorithmStrategy::mutate(Allocation& chromosome,float maxDistance){

	chromosome.removeRandomPath();

	vector<GraphNode*> unvisitedNodes = findNodesOfUnvisitedEdges(chromosome);

	Set unvisitedNodesId;
	for(int i=0; i < unvisitedNodes.size(); i++){
		unvisitedNodesId.add(unvisitedNodes[i]->getId());
	}

	GraphNode* node = (*nodes)[unvisitedNodesId.random()];

	do {

		vector<Path*>* allPathsOfThisNode = getAllPathsOfNode(node,maxDistance);
		vector<Path*> consideredPaths;  // that satisfy the bound - for saving space

		filterFeasiblePaths(chromosome, *allPathsOfThisNode, consideredPaths);


		Path* consideredPath = NULL;

		// select a random path
		if(!consideredPaths.empty()){
			int randomNumber = Util::random(consideredPaths.size());
			consideredPath = consideredPaths[randomNumber];
			chromosome.addPath(consideredPath);
		}

		unvisitedNodesId.remove(node->getId());

		if (unvisitedNodesId.size() > 0){
			node = (*nodes)[unvisitedNodesId.random()];
		}

	} while(unvisitedNodesId.size() > 0);

}

vector<Allocation*> GeneticAlgorithmStrategy::selection(vector<Allocation*> initGen, vector<Allocation*> newGen ){
	vector<Allocation*> result;

	sort(initGen.begin(),initGen.end(),compare);
	sort(newGen.begin(),newGen.end(),compare);

	int initNum = ELITISM_PERCENTAGE / 100 * SIZE;
	int count = 0;
	for(int i=initGen.size()-1; i >= 0; i--, count++ ){
		if(count < initNum){
			result.push_back(initGen[i]);
		}
		else {
			delete initGen[i];
		}
	}

	for(int i=newGen.size()-1; i >= 0; i--, count++ ){
		if (count < SIZE){
			result.push_back(newGen[i]);
		}
		else {
			delete newGen[i];
		}
	}

	return result;
}

bool GeneticAlgorithmStrategy::compare(Allocation* a,Allocation* b){
	float delta = (a->getVisitedEdges().size() - b->getVisitedEdges().size())*0.5 + (a->size() - b->size())*0.5;
	if(delta < 0){
		return true;
	}
	return false;
}
