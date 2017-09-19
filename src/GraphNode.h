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
#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

class GraphEdge;

class GraphNode {
private:
	string id;
	double x;
	double y;
	vector<GraphEdge*> edges;

	int random();

public:
	GraphNode(string i,double x,double y);
	GraphEdge* addNode(GraphNode* node);
	void print();
	GraphNode* randomWalk();
	vector<GraphEdge*> getEdges();
	double getX();
	double getY();
	string getId();
	double distance(GraphNode* o);
	virtual ~GraphNode();
};



#endif /* GRAPHNODE_H_ */
