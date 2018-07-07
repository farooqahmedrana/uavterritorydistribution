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
#ifndef SET_H_
#define SET_H_

#include <map>
#include <string>
#include <vector>

using namespace std;

class Set {

private:
	map<string,int> elements;


public:
	Set();
	void add(string element);
	void remove(string element);
	bool exists(string element);
	string first();
	string random();

	bool disjoint(Set* s);
	Set* unionset(Set* s);
	Set* unionset(vector<Set*>& sets);
	Set* difference(Set* s);
	bool equal(Set* s);
	int size();


	vector<string> getElements();
	void print();

	virtual ~Set();
};

#endif /* SET_H_ */
