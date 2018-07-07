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
#ifndef GRAPHXMLPARSER_H_
#define GRAPHXMLPARSER_H_

#include <libxml/parser.h>

using namespace std;

class Graph;

class GraphXmlParser {

protected:
	char* filename;
	Graph* graph;

	void parseNodes(xmlNodePtr);
	void parseEdges(xmlNodePtr);
	void parseNode(xmlNodePtr);
	void parseEdge(xmlNodePtr);

public:
	GraphXmlParser(Graph* graph,char* file);
	void parse();
	virtual ~GraphXmlParser();
};

#endif /* GRAPHXMLPARSER_H_ */
