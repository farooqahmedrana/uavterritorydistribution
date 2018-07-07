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
#include "GraphNode.h"
#include "GraphEdge.h"
#include "Util.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>


using namespace std;


GraphNode::GraphNode(string i,double x,double y) {
	this->id = i;
	this->x = x;
	this->y = y;
}

void GraphNode::print(){
	stringstream distances;
	for(int i=0; i < edges.size(); i++){
		distances << edges[i]->distance() << ",";
	}
	cout << id << ":" << x <<	"," << y << ":" << edges.size() << ":" << distances.str() << endl;
}

GraphEdge* GraphNode::addNode(GraphNode* node){
	GraphEdge* edge = new GraphEdge(this,node);
	edges.push_back(edge);
	return edge;
}

double GraphNode::getX(){
	return x;
}

double GraphNode::getY(){
	return y;
}

GraphNode* GraphNode::randomWalk(){
	return edges[random()]->getToNode();
}

int GraphNode::random(){
	Util::random(edges.size());
}

string GraphNode::getId(){
	return id;
}

vector<GraphEdge*> GraphNode::getEdges(){
	return edges;
}

double GraphNode::distance(GraphNode* o){
	double xd = o->x - this->x;
	double yd = o->y - this->y;

	return sqrt((xd*xd) + (yd*yd));
}


GraphNode::~GraphNode() {
	// TODO Auto-generated destructor stub
}
