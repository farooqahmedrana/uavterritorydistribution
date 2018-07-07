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
#ifndef PATH_H_
#define PATH_H_

#include <map>
#include <string>
#include <vector>
#include "GraphEdge.h"
#include "Set.h"

using namespace std;

class Graph;

class Path {
private:
	string path;
	Set ids;

public:
	Path();
	Path(string p);
	Path(vector<string>& ids);
	Path(vector<GraphEdge*>&);

	bool disjoint(Path* p);
	Path* unionset(Path* p);
	Path* unionset(vector<Path*>& p);
	Path* difference(Path* p);
	bool equal(Path*);
	void print();
	string toString();
	int size();
	Set getIds();

	virtual ~Path();
};

#endif /* PATH_H_ */
