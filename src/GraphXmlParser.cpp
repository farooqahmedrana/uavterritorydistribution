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
#include "GraphXmlParser.h"
#include "Graph.h"

#include <iostream>
#include <string>

using namespace std;

GraphXmlParser::GraphXmlParser(Graph* graph,char* file) {
	this->graph = graph;
	this->filename = file;
}

void GraphXmlParser::parse(){
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(filename);

	if (doc == NULL ) {
		cout << "Not parsed" << endl;
		return;
	}

	cur = xmlDocGetRootElement(doc);


	if (cur == NULL) {
		cout << "Empty document" << endl;
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "graph") != 0) {
		cout << "document of the wrong type, root node != graph" << endl;
		xmlFreeDoc(doc);
		return;
	}
	else {
		xmlNodePtr node = cur->xmlChildrenNode;

		while (node != NULL){

			if (xmlStrcmp(node->name, (const xmlChar *) "nodes") == 0) {
				cout << "parse nodes" << endl;
				parseNodes(node);
			}

			if (xmlStrcmp(node->name, (const xmlChar *) "edges") == 0) {
				cout << "parse edges" << endl;
				parseEdges(node);
			}

			node = node->next;
		}

		xmlFreeDoc(doc);
	}

	cout << "parsed" << endl;
}

void GraphXmlParser::parseNodes(xmlNodePtr root){

	xmlNodePtr node = root->xmlChildrenNode;
	while (node != NULL){
		if (xmlStrcmp(node->name, (const xmlChar *) "node") == 0) {
			parseNode(node);
		}
		node = node->next;
	}
}

void GraphXmlParser::parseEdges(xmlNodePtr root){
	xmlNodePtr edge = root->xmlChildrenNode;

	while (edge != NULL){
		if (xmlStrcmp(edge->name, (const xmlChar *) "edge") == 0) {
			parseEdge(edge);
		}
		edge = edge->next;
	}
}

void GraphXmlParser::parseNode(xmlNodePtr node){
	string id ((const char*) xmlGetProp(node,(const xmlChar*) "id"));
	double x = atof((const char*) xmlGetProp(node,(const xmlChar*) "x"));
	double y = atof((const char*)  xmlGetProp(node,(const xmlChar*) "y"));

	graph->addNode(new GraphNode(id,x,y));
}

void GraphXmlParser::parseEdge(xmlNodePtr node){

	string from((const char*) xmlGetProp(node,(const xmlChar*) "from"));
	string to((const char*) xmlGetProp(node,(const xmlChar*) "to"));

	graph->addEdge(from,to);
}

GraphXmlParser::~GraphXmlParser() {

}
