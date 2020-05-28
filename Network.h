#pragma once
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "List.h"
#include "Queue.h"
#include "Map.h"
constexpr short INPUT_DATA_AMOUNT = 3;
constexpr uint64_t SOURCE = 1;
constexpr uint64_t INF = UINT64_MAX;

class Network
{
private:
	enum {V1, V2, C}; //start vertex, end vertex, flow capacity
	
	//represents node's marking for the ford-fulkerson algorithm
	struct Mark
	{
		uint64_t capacity; 
		uint64_t prev_node;

		Mark(uint64_t capacity, uint64_t prev_node);

		~Mark();
	};

	//represents each vertex of the network
	class Vertex
	{
	public:
		uint64_t number; //vertex number
		Mark* mark;
		Map<uint64_t, uint64_t>* routes; //represents links with other nodes and remnant bandwidth on the edge between them

		Vertex(uint64_t number, Map<uint64_t, uint64_t>* routes, Mark* mark = nullptr);

		~Vertex();
	};

	uint64_t maximal_flow; //value of maximal flow in the network
	Map<std::string, uint64_t> number_name_match; //represents match between node's name and number
	List<Vertex*> vertices; //stores all network's vertices; vertex number is always equals (list index + 1)
	List<std::string*> input; //store's input lines

	Vertex* createVertex(uint64_t number, uint64_t neighbor, uint64_t capacity);
	void recountBandwidths(uint64_t f);
	
public:

	Network();

	Network(std::string& filepath);

	void readFrom(std::string& filepath); //reads file and interpets it as queue of char arrays

	List<std::string*> getInputData(); //returns input data converted to List of string Arrays

	void create(); //builds the network from input data

	uint64_t ford_fulkerson(); //counts maximal flow in the network using Ford-Fulkerson's algorithm
};