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
#include <sstream>
#include <queue>
#include "PathStrategy.h"
#include "GraphEdge.h"


using namespace std;

void BfsStrategy::execute(GraphNode* node,float maxDistance,vector<Path*>& paths){
	float distance = 0;
	stringstream pathinfo;
	Set visitedEdges;
	GraphNode* current = node;
	queue<GraphNode*> nodeQueue;

	nodeQueue.push(node);

	do{
		current = nodeQueue.front();
		nodeQueue.pop();
		vector<GraphEdge*> edges = current->getEdges();

		for(int i=0; i < edges.size();i++){
			GraphEdge* edge = edges[i];

			if(! edge->existsUndirectedIn(visitedEdges) &&
					distance + 2*edge->distance() <= maxDistance){

						distance += 2*edge->distance();
						edge->addUndirectedIn(visitedEdges);
						pathinfo << edge->getUndirectedPathString();

						nodeQueue.push(edge->getToNode());
			}
		}

	} while(!nodeQueue.empty());

	paths.push_back(new Path(pathinfo.str()));
}

void DfsStrategy::execute(GraphNode* node,float maxDistance,vector<Path*>& paths){
	Set pathEdges;
	generatePaths(node,maxDistance,0,pathEdges,1);

	stringstream pathStr;
	vector<string> edges = pathEdges.getElements();
	for(int i=0; i < edges.size(); i++){
		pathStr << edges[i] << ";";
	}

	paths.push_back(new Path(pathStr.str()));
}

void DfsStrategy::generatePaths(GraphNode* node,float maxDistance,float currentDistance,Set& pathEdges,int level){

	vector<GraphEdge*> edges = node->getEdges();

	for(int i=0; i < edges.size(); i++){

		if( currentDistance + 2*(edges[i]->distance()) <= maxDistance)
		{
			edges[i]->addUndirectedIn(pathEdges);
			generatePaths(edges[i]->getToNode(),maxDistance,currentDistance + 2*(edges[i]->distance()),pathEdges,level+1);
		}
	}

}

void RadialStrategy::findNodes(GraphNode* root,GraphNode* node,float range,map<string,GraphNode*>& nodes){
	nodes[node->getId()] = node;
	vector<GraphEdge*> edges = node->getEdges();
	for(int i=0; i < edges.size(); i++){
		GraphNode* nextNode = edges[i]->getToNode();

		if (nodes.count(nextNode->getId()) == 0 &&
				root->distance(nextNode) <= range ){

			findNodes(root,nextNode,range,nodes);
		}
	}
}

void RadialStrategy::execute(GraphNode* node,float maxDistance,vector<Path*>& paths){
	map<string,GraphNode*> nodes;
	findNodes(node,node,maxDistance/2,nodes);

	stringstream pathStr;
	for(map<string,GraphNode*>::iterator iter = nodes.begin(); iter != nodes.end(); iter++){
		vector<GraphEdge*> edges = iter->second->getEdges();

		for(int i=0; i < edges.size(); i++){
			if (nodes.count(edges[i]->getToNode()->getId()) != 0){
				pathStr << edges[i]->getUndirectedPathString();
			}
		}
	}

	paths.push_back(new Path(pathStr.str()));
}


void LfsStrategy::execute(GraphNode* node,float maxDistance,vector<Path*>& paths){
	Set visitedEdges;
	generatePaths(node,maxDistance,0,"",visitedEdges,paths);
}

void LfsStrategy::generatePaths(GraphNode* node,float maxDistance,float currentDistance,string path,Set& visitedEdges,vector<Path*>& paths){

	float temp = 0;
	stringstream pathTemp;
	vector<GraphEdge*> edges = node->getEdges();

	// looking ahead
	for(int i=0; i < edges.size(); i++){
		if(! edges[i]->existsUndirectedIn(visitedEdges)){
			temp += 2*edges[i]->distance();
			pathTemp << edges[i]->getUndirectedPathString();
		}
	}

	// deciding further action based upon lookahead result
	if (currentDistance + temp <= maxDistance){ // when all edges can be added, recurse further
		currentDistance += temp;
		stringstream revisedPath;
		revisedPath << path << pathTemp.str();

		for (int i=0; i < edges.size(); i++){
			if(! edges[i]->existsUndirectedIn(visitedEdges)){
				edges[i]->addUndirectedIn(visitedEdges);
				generatePaths(edges[i]->getToNode(),maxDistance,currentDistance,revisedPath.str(),visitedEdges,paths);
			}
		}
	}
	else {										// when only some edges can be added

		// edges of the current node not visited yet
		vector<GraphEdge*> unvisitedEdges;
		for(int i=0; i < edges.size(); i++){
			if(! edges[i]->existsUndirectedIn(visitedEdges)){
				unvisitedEdges.push_back(edges[i]);
			}
		}

		// loop 2^n possibilities (all subsets of considered set)
		int ns = 1 << unvisitedEdges.size();
		for(int i=0; i < ns; i++){

			float tempDistance = 0;
			vector<GraphEdge*> subset;

			// create a subset of unvisited edges
			for(int j=0; j < unvisitedEdges.size(); j++){
				// for each bit, if set, consider corresponding edge
				if( (1 << j) & i ) {
					tempDistance += unvisitedEdges[j]->distance();
					subset.push_back(unvisitedEdges[j]);
				}

			}

			// evaluate for the distance criteria
			if(currentDistance + 2*tempDistance <= maxDistance){

				// add a path
				stringstream tempEdges;
				for(int k=0; k < subset.size(); k++){
					subset[k]->addUndirectedIn(visitedEdges);
					tempEdges << subset[k]->getUndirectedPathString();
				}

				stringstream revisedPath;
				revisedPath << path << tempEdges.str();
				paths.push_back(new Path(revisedPath.str()));

			}

		}

	}

}

