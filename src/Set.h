/*
 * Set.h
 *
 *  Created on: Jul 23, 2017
 *      Author: farooq
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
