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
#include "Set.h"
#include "Util.h"
#include <iostream>

Set::Set() {

}

void Set::add(string element){
	elements[element] = 1;
}

void Set::remove(string element){
	elements.erase(element);
}

bool Set::exists(string element){
	return elements.count(element) > 0;
}

string Set::first(){
	return elements.begin()->first;
}

string Set::random(){
	int num = Util::random(elements.size());
	map<string,int>::iterator iterator = elements.begin();
	for(int i=0; i < num; i++,iterator++);
	return iterator->first;
}

bool Set::disjoint(Set* s){
	bool result = true;

	for(map<string,int>::iterator iter = s->elements.begin(); iter != s->elements.end(); iter++)
	{
		if(this->exists(iter->first))
		{
			result = false;
			break;
		}
	}

	return result;
}

Set* Set::unionset(Set* s){

	Set* newSet = new Set();

	for(map<string,int>::iterator iter = this->elements.begin(); iter != this->elements.end(); iter++){
		newSet->add(iter->first);
	}

	for(map<string,int>::iterator iter = s->elements.begin(); iter != s->elements.end(); iter++){
		newSet->add(iter->first);
	}

	return newSet;
}

Set* Set::unionset(vector<Set*>& sets){

	Set* newSet = new Set();

	for(map<string,int>::iterator iter = this->elements.begin(); iter != this->elements.end(); iter++){
		newSet->add(iter->first);
	}

	for(int i=0; i < sets.size(); i++){
		Set* s = sets[i];
		for(map<string,int>::iterator iter = s->elements.begin(); iter != s->elements.end(); iter++){
			newSet->add(iter->first);
		}
	}

	return newSet;
}

Set* Set::difference(Set* s){
	Set* newSet = new Set();

	for(map<string,int>::iterator iter = elements.begin(); iter != elements.end(); iter++){
		if (!s->exists(iter->first)){
			newSet->add(iter->first);
		}
	}

	return newSet;
}

bool Set::equal(Set* s){
	bool result = elements.size() == s->elements.size();

	if (result == true){
		for(map<string,int>::iterator iter = s->elements.begin(); iter != s->elements.end(); iter++){
			result = result && exists(iter->first);

			if (result == false){
				return result;
			}
		}
	}

	return result;
}

int Set::size(){
	return elements.size();
}

void Set::print(){
	for(map<string,int>::iterator iter = elements.begin(); iter != elements.end(); iter++){
		cout << iter->first << ";";
	}
	cout << endl;
}

vector<string> Set::getElements(){
	vector<string> elementsResult;

	for(map<string,int>::iterator iter = elements.begin(); iter != elements.end(); iter++){
		elementsResult.push_back(iter->first);
	}

	return elementsResult;
}

Set::~Set() {

}
