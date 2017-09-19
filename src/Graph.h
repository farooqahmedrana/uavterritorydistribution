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
#ifndef GRAPH_H_
#define GRAPH_H_

#include "GraphNode.h"

#include <map>
#include <vector>
#include <string>
#include <stack>
#include "Path.h"
#include "PathStrategy.h"
#include "PartitionStrategy.h"
#include "Allocation.h"

using namespace std;

class Graph {

	friend class PartitionStrategy;

private:
	map<string,GraphNode*> nodes;
	map<string,GraphEdge*> edges;

	void ensureUndirectedGraph();

public:
	Graph();
	void load(char* file);
	void print();
	void partition(float maxDistance,PartitionStrategy* partitionStrategy);
	void addNode(GraphNode* node);
	void addEdge(string from,string to);
	GraphNode* getNode(string id);
	GraphEdge* getEdge(string id);

	vector<GraphEdge*> filterNodeEdges(string nodefile);
	string convertPathToXml(string path);

	virtual ~Graph();
};

#endif /* GRAPH_H_ */

