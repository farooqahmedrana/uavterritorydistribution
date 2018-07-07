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
#include "Path.h"
#include "Graph.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <map>

Path::Path() {

}

Path::Path(string p){

	path = p;

	char* temp = new char[path.length()+1];
	strcpy(temp,path.c_str());

	char* token = strtok(temp,";");
	while(token != NULL){
		string id (token);
		ids.add(id);
		token = strtok(NULL,";");
	}

	delete [] temp;
}

Path::Path(vector<string>& ids){
	for(int i=0; i < ids.size() ; i++){
		this->ids.add(ids[i]);
	}
}


Path::Path(vector<GraphEdge*>& edges){
	for(int i=0; i < edges.size() ; i++){
		ids.add(edges[i]->getId());
	}
}

Set Path::getIds(){
	return ids;
}

void Path::print(){
	ids.print();
}

bool Path::disjoint(Path* p){
	return ids.disjoint(&p->ids);
}

Path* Path::unionset(Path* p){
	Set* unionSet = ids.unionset(&p->ids);
	vector<string> elements = unionSet->getElements();
	Path* newPath = new Path(elements);
	delete unionSet;
	return newPath;
}

Path* Path::unionset(vector<Path*>& p){

	vector<Set*> sets;
	for (int i=0; i < p.size(); i++){
		sets.push_back(&p[i]->ids);
	}
	Set* unionSet = ids.unionset(sets);
	vector<string> elements = unionSet->getElements();
	Path* newPath = new Path(elements);
	delete unionSet;
	return newPath;
}

Path* Path::difference(Path* p){

	Set* differenceSet = ids.difference(&p->ids);
	vector<string> elements = differenceSet->getElements();
	Path* newPath = new Path(elements);
	delete differenceSet;
	return newPath;
}

bool Path::equal(Path* p){
	return ids.equal(&p->ids);
}

string Path::toString(){
	return path;
}

int Path::size(){
	return ids.size();
}

Path::~Path() {
	// TODO Auto-generated destructor stub
}
