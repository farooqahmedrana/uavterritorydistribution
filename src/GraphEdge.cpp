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

#include "GraphEdge.h"
#include "GraphNode.h"

#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

GraphEdge::GraphEdge(GraphNode* from,GraphNode* to) {
	this->from = from;
	this->to = to;
}

GraphNode* GraphEdge::getFromNode(){
	return from;
}

GraphNode* GraphEdge::getToNode(){
	return to;
}


double GraphEdge::distance(){
	double x = to->getX() - from->getX();
	double y = to->getY() - from->getY();
	return sqrt((x*x) + (y*y));
}

void GraphEdge::print(){
	cout << getId() << endl;
}

bool GraphEdge::existsUndirectedIn(Set& edges){
	return edges.exists(getId()) && edges.exists(getId(true));
}

void GraphEdge::addUndirectedIn(Set& edges){
	edges.add(getId());
	edges.add(getId(true));
}


string GraphEdge::getId(bool reverse){
	stringstream stream;
	if (!reverse){
		stream << from->getId() << "->" << to->getId();
	}
	else {
		stream << to->getId() << "->" << from->getId();
	}

	return stream.str();
}

string GraphEdge::getUndirectedPathString(){
	stringstream stream;
	stream << getId() << ";" << getId(true) << ";";
	return stream.str();
}

GraphEdge::~GraphEdge() {

}
