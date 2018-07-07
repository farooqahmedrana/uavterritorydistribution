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
#include "Allocation.h"
#include <iostream>

Allocation::Allocation() {
	unionpath = new Path();
}

Allocation::Allocation(Allocation& sol) {

	for(int i=0; i < sol.allocation.size(); i++){
		allocation.push_back(sol.allocation[i]);
	}

	Path empty;
	unionpath = empty.unionset(allocation);

}

void Allocation::addPath(Path* p){
	allocation.push_back(p);
	Path* newUnionPath = unionpath->unionset(p);
	delete unionpath;
	unionpath = newUnionPath;
}

void Allocation::removeLastPath(){
	Path* p = allocation[allocation.size()-1];
	allocation.pop_back();
	Path* newUnionPath = unionpath->difference(p);
	delete unionpath;
	unionpath = newUnionPath;
}

bool Allocation::checkDisjointPathProperty(Path* p){
	return unionpath->disjoint(p);
}

bool Allocation::compare(Allocation& sol){

	if(this->unionpath->size() > sol.unionpath->size() ||						// more edges covered
		(this->unionpath->size() == sol.unionpath->size() && this->allocation.size() < sol.allocation.size()) ){ // or same edges but with fewer paths
			return true; // better solution
	}
	return false;
}


Set Allocation::getVisitedEdges(){
	return unionpath->getIds();
}

void Allocation::print(){

	for(int i=0; i < allocation.size(); i++){
		allocation[i]->print();
	}

}

void Allocation::operator =(Allocation& sol){
	allocation.clear();
	for(int i=0; i < sol.allocation.size(); i++){
		allocation.push_back(sol.allocation[i]);
	}

	Path empty;
	Path* newUnionPath = empty.unionset(allocation);
	delete unionpath;
	unionpath = newUnionPath;
}

int Allocation::size(){
	return allocation.size();
}

void Allocation::merge(Allocation& alloc){
	for(int i=0; i < alloc.allocation.size(); i++){
		allocation.push_back(alloc.allocation[i]);
	}

	Path* newUnionPath = unionpath->unionset(alloc.allocation);
	delete unionpath;
	unionpath = newUnionPath;
}

Allocation::~Allocation() {
	delete unionpath;
}
