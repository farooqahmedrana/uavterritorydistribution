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
#ifndef PATHSTRATEGY_H_
#define PATHSTRATEGY_H_

#include <string>
#include <vector>
#include <map>
#include "GraphNode.h"
#include "Path.h"

using namespace std;


class PathStrategy {
public:
	virtual void execute(GraphNode* node,float maxDistance,vector<Path*>& paths) = 0;
};

class BfsStrategy : public PathStrategy{
public:
	virtual void execute(GraphNode* node,float maxDistance,vector<Path*>& paths);
};

class DfsStrategy : public PathStrategy{
private:
	void generatePaths(GraphNode* node,float maxDistance,float currentDistance,Set& pathEdges,int level);
public:
	virtual void execute(GraphNode* node,float maxDistance,vector<Path*>& paths);
};

class RadialStrategy : public PathStrategy{
private:
	void findNodes(GraphNode* root,GraphNode* node,float range,map<string,GraphNode*>& nodes);
public:
	virtual void execute(GraphNode* node,float maxDistance,vector<Path*>& paths);
};

class LfsStrategy : public PathStrategy{
private:
	void generatePaths(GraphNode* node,float maxDistance,float currentDistance,string path,Set& visitedEdges,vector<Path*>& paths);
public:
	virtual void execute(GraphNode* node,float maxDistance,vector<Path*>& paths);
};

#endif /* PATHSTRATEGY_H_ */
