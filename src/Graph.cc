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
#include "Graph.h"
#include "GraphNode.h"
#include "GraphEdge.h"
#include "GraphXmlParser.h"
#include "Util.h"
#include "PartitionStrategy.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <fstream>
#include <sstream>


using namespace std;

Graph::Graph() {

}

void Graph::load(char* file){
	GraphXmlParser parser(this,file);
	parser.parse();
    ensureUndirectedGraph();
}

void Graph::ensureUndirectedGraph()
{
    // ensuring bi-directional edges
    for(map<string,GraphEdge*>::iterator iter = edges.begin();iter != edges.end();iter++){
        if(edges.count(iter->second->getId(true)) == 0){
            GraphEdge *edge = iter->second->getToNode()->addNode(iter->second->getFromNode());
            edges[edge->getId()] = edge;
        }
    }

}


void Graph::addNode(GraphNode* node){
	nodes[node->getId()] = node;
}

void Graph::addEdge(string from,string to){

	if(nodes.count(from) > 0 && nodes.count(to) > 0){
		GraphEdge* edge = nodes[from]->addNode(nodes[to]);
		edges[edge->getId()] = edge;
	}
}

GraphNode* Graph::getNode(string id){
	return nodes[id];
}

GraphEdge* Graph::getEdge(string id){
	return edges[id];
}

void Graph::print(){
	for(map<string,GraphNode*>::iterator iter = nodes.begin(); iter != nodes.end(); iter++){
		iter->second->print();
	}
}

void Graph::partition(float maxDistance,PartitionStrategy* strategy){
	strategy->execute(maxDistance);
}

vector<GraphEdge*> Graph::filterNodeEdges(string nodefile){
	map<string,int> selectedNodes;
	fstream file(nodefile.c_str());
	string line;
	if (file.is_open())
	{
		while ( getline (file,line) )
		{
			selectedNodes[line] = 1;
		}
		file.close();
	}

	vector<GraphEdge*> selectedEdges;
	for(map<string,GraphEdge*>::iterator iter = edges.begin(); iter != edges.end(); iter++){
		if(selectedNodes.count(iter->second->getFromNode()->getId()) > 0
				|| selectedNodes.count(iter->second->getToNode()->getId()) > 0){
			selectedEdges.push_back(iter->second);
		}
	}

	return selectedEdges;
}

GraphNode* Graph::findBase(string path){
	GraphNode* base = NULL;

	Path p(path);
	vector<string> ids = p.getIds().getElements();

	map<string,GraphNode*> pathNodes;

	for(int i=0; i < ids.size(); i++){
		GraphEdge* edge = edges[ids[i]];
		if (edge != NULL){
			GraphNode* pNode = edge->getFromNode();
			pathNodes[pNode->getId()] = pNode;

			pNode = edge->getToNode();
			pathNodes[pNode->getId()] = pNode;
		}
	}
	// determine base node
	int randomNumber = Util::random(pathNodes.size());
	int count = 0;

	for(map<string,GraphNode*>::iterator iter = pathNodes.begin(); iter != pathNodes.end(); iter++){
		if(count == randomNumber){
			base = iter->second;
			break;
		}
		count++;
	}

	return base;
}

string Graph::convertPathToXml(string path,string base){
	Path p(path);
	vector<string> ids = p.getIds().getElements();

	map<string,GraphNode*> pathNodes;

	for(int i=0; i < ids.size(); i++){
		GraphEdge* edge = edges[ids[i]];
		if (edge != NULL){
			GraphNode* pNode = edge->getFromNode();
			pathNodes[pNode->getId()] = pNode;

			pNode = edge->getToNode();
			pathNodes[pNode->getId()] = pNode;
		}
	}

	stringstream xmlContent;
	xmlContent << "<graph>\n<nodes>\n" ;
	for(map<string,GraphNode*>::iterator iter = pathNodes.begin(); iter != pathNodes.end(); iter++){
		if (iter->second->getId().compare(base) == 0){
			xmlContent << "<node id='" << iter->first << "' x='" << iter->second->getX() << "' y='" << iter->second->getY() << "' type='base' />\n";
		}
		else {
			xmlContent << "<node id='" << iter->first << "' x='" << iter->second->getX() << "' y='" << iter->second->getY() << "' type='node' />\n";
		}
	}
	xmlContent << "</nodes>\n<edges>\n" ;

	for(int i=0; i < ids.size(); i++){
		GraphEdge* edge = edges[ids[i]];
		if (edge != NULL){
			xmlContent << "<edge from='" << edge->getFromNode()->getId() << "' to='" << edge->getToNode()->getId() << "' />\n" ;
		}
	}
	xmlContent << "</edges>\n</graph>";

	return xmlContent.str();
}


Graph::~Graph() {

	for(map<string,GraphNode*>::iterator iter = nodes.begin();iter != nodes.end();iter++){
	    delete iter->second;
	}

    for(map<string,GraphEdge*>::iterator iter = edges.begin();iter != edges.end();iter++){
		delete iter->second;
    }

}


