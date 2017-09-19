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
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include "Graph.h"
#include "PathStrategy.h"


using namespace std;

PathStrategy* getPathStrategy(char* name);
PartitionStrategy* getPartitionStrategy(char* name,Graph* graph,PathStrategy* pathStrategy);
int generate(char* graphFile,char* pathStrategyName,char* partitionStrategyName,float distance);
void convert(char* graphFile,char* inputPaths,char* outputDirectory);


/*
int main(){
	Graph graph;
	graph.load("/home/farooq/sumoprojects/cologne/cologne-center-graph.xml");
	PathStrategy* strategy = getPathStrategy("bfs");
	vector<Path*> paths;
	strategy->execute(graph.getNode("1004021481"),2000,paths);
	cout << "bfs:\n----" << endl;
	if (paths.size() > 0){
		for(int i=0; i < paths.size();i++)
			cout << paths[i]->size() << endl;
	}

	delete strategy;

	strategy = getPathStrategy("lfs");
	paths.clear();
	strategy->execute(graph.getNode("1004021481"),2000,paths);
	cout << "lfs:\n----" << endl;
	if (paths.size() > 0){
		for(int i=0; i < paths.size();i++)
			cout << paths[i]->size() << endl;
	}

	delete strategy;

	return 0;
}
*/

int main(int argc,char* argv[]){



	if (argc > 1 && strcmp(argv[1],"generate") == 0 && argc == 6){
		return generate(argv[2],argv[3],argv[4],atof(argv[5]));
	}
	else if (argc > 1 && strcmp(argv[1],"convert") == 0 && argc == 5){
		convert(argv[2],argv[3],argv[4]);
		return 0;
	}
	else {
		cout << "usage:\n (1) " << argv[0] << " generate graph-file path-strategy partition-strategy distance" << endl <<
				         "(2) " << argv[0] << " convert graph-file paths-file output-directory" << endl;
		return 1;
	}

	return 0;
}

void convert(char* graphFile,char* inputPaths,char* outputDirectory){
	Graph graph;
	graph.load(graphFile);

	fstream inputFile(inputPaths);
	string line;
	if(inputFile.is_open()){
		int counter = 0;

		while (getline(inputFile,line)){
			string xml = graph.convertPathToXml(line);

			stringstream outputFilePath;
			outputFilePath << outputDirectory << "g" << counter << ".xml" ;

			fstream outputFile(outputFilePath.str().c_str(),ios::out);
			if (outputFile.is_open()){
				outputFile << xml ;
				outputFile.close();
			}

			counter++;
		}
	}
}

int generate(char* graphFile,char* pathStrategyName,char* partitionStrategyName,float distance){
	Graph graph;
	PartitionStrategy* partitionStrategy = NULL;

	graph.load(graphFile);
	PathStrategy* pathStrategy = getPathStrategy(pathStrategyName);
	if (pathStrategy != NULL){
		partitionStrategy = getPartitionStrategy(partitionStrategyName,&graph,pathStrategy);
	}
	else{
		cout << "path strategy not supported" << endl;
		return 1;
	}

	if (partitionStrategy != NULL){
		graph.partition(distance,partitionStrategy);

		delete pathStrategy;
		delete partitionStrategy;
	}
	else{
		cout << "partition strategy not supported" << endl;
		return 1;
	}

	return 0;
}


PathStrategy* getPathStrategy(char* name){
	if (strcmp(name,"bfs") == 0){
		return new BfsStrategy();
	}
	else if (strcmp(name,"dfs") == 0){
		return new DfsStrategy();
	}
	else if (strcmp(name,"radial") == 0){
			return new RadialStrategy();
	}
	else if (strcmp(name,"lfs") == 0){
		return new LfsStrategy();
	}

	return NULL;
}

PartitionStrategy* getPartitionStrategy(char* name,Graph* graph,PathStrategy* pathStrategy){
	if (strcmp(name,"optimal") == 0){
		return new OptimalStrategy(graph,pathStrategy);
	}
	else if (strcmp(name,"greedy") == 0){
		return new GreedyStrategy(graph,pathStrategy);
	}

	return NULL;
}
