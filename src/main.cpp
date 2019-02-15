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
PartitionStrategy* getPartitionStrategy(char* name,Graph* graph,PathStrategy* pathStrategy,map<string,string> options);
int generate(char* graphFile,char* pathStrategyName,char* partitionStrategyName,float distance,map<string,string> options);
void convert(char* graphFile,char* inputPaths,char* outputDirectory);
void createFleetFile(map<GraphNode*,vector<string> >& outputMap, char* outputDirectory);
map<string,string> getOptions(char* options);


int main(int argc,char* argv[]){



	if (argc > 1 && strcmp(argv[1],"generate") == 0 && argc >= 6){
		map<string,string> options;
		if (argc > 6)
			options = getOptions(argv[6]);
		return generate(argv[2],argv[3],argv[4],atof(argv[5]),options);
	}
	else if (argc > 1 && strcmp(argv[1],"convert") == 0 && argc == 5){
		convert(argv[2],argv[3],argv[4]);
		return 0;
	}
	else {
		cout << "usage:\n (1) " << argv[0] << " generate graph-file path-strategy partition-strategy distance [options]" << endl <<
				         "(2) " << argv[0] << " convert graph-file paths-file output-directory" << endl;
		return 1;
	}

	return 0;
}

void convert(char* graphFile,char* inputPaths,char* outputDirectory){
	Graph graph;
	graph.load(graphFile);
	map<GraphNode*,vector<string> > outputMap; // output files mapped to base

	fstream inputFile(inputPaths);
	string line;
	if(inputFile.is_open()){
		int counter = 0;

		while (getline(inputFile,line)){
			GraphNode* base = graph.findBase(line);
			string xml = graph.convertPathToXml(line,base->getId());

			stringstream outputFilePath;
			outputFilePath << outputDirectory << "g" << counter << ".xml" ;

			outputMap[base].push_back(outputFilePath.str());

			fstream outputFile(outputFilePath.str().c_str(),ios::out);
			if (outputFile.is_open()){
				outputFile << xml ;
				outputFile.close();
			}

			counter++;
		}
	}

	createFleetFile(outputMap,outputDirectory);
}

void createFleetFile(map<GraphNode*,vector<string> >& outputMap, char* outputDirectory){
	stringstream outputFilePath;
	outputFilePath << outputDirectory << "fleet.xml" ;

	stringstream content;
	content << "<fleet>" << endl;
	for(map<GraphNode*,vector<string> >::iterator iter = outputMap.begin(); iter != outputMap.end(); iter++){
		content << "<team>" << endl;
		content << "<base x='" << iter->first->getX() << "' y='" << iter->first->getY() << "' ip='' />" << endl;
		for(int i=0; i < iter->second.size(); i++){
			content << "<uav graph='" << (iter->second)[i] << "' ip='' />" << endl;
		}
		content << "</team>" << endl;
	}
	content << "</fleet>" << endl;

	fstream outputFile(outputFilePath.str().c_str(),ios::out);
	if (outputFile.is_open()){
		outputFile << content.str();
		outputFile.close();
	}
}

int generate(char* graphFile,char* pathStrategyName,char* partitionStrategyName,float distance,map<string,string> options){
	Graph graph;
	PartitionStrategy* partitionStrategy = NULL;

	graph.load(graphFile);
	PathStrategy* pathStrategy = getPathStrategy(pathStrategyName);
	if (pathStrategy != NULL){
		partitionStrategy = getPartitionStrategy(partitionStrategyName,&graph,pathStrategy,options);
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

PartitionStrategy* getPartitionStrategy(char* name,Graph* graph,PathStrategy* pathStrategy,map<string,string> options){
	if (strcmp(name,"optimal") == 0){
		return new OptimalStrategy(graph,pathStrategy);
	}
	else if (strcmp(name,"greedy") == 0){
		return new GreedyStrategy(graph,pathStrategy);
	}
	else if (strcmp(name,"montecarlo") == 0){
		string iterations = options["iterations"];
		if (!iterations.empty()){
			return new MonteCarloStrategy(graph,pathStrategy,atoi(iterations.c_str()));
		}
		else return new MonteCarloStrategy(graph,pathStrategy,10);
	}
	else if (strcmp(name,"simulatedannealing") == 0){
		return new SimulatedAnnealingStrategy(graph,pathStrategy);
	}
	else if (strcmp(name,"geneticalgorithm") == 0){
		return new GeneticAlgorithmStrategy(graph,pathStrategy);
	}

	return NULL;
}

map<string,string> getOptions(char* options){
	map<string,string> optionsRecord;
	vector<char*> tokens;

	char* token = strtok(options,";");
	while (token != NULL){
		tokens.push_back(token);
		token = strtok(NULL,";");
	}

	for(int i=0; i < tokens.size(); i++){
		char* key = strtok(tokens[i],"=");
		char* value = strtok(NULL,"=");

		optionsRecord[string(key)] = string(value);
	}

	return optionsRecord;
}
