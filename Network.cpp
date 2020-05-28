#include "Network.h"

/// FOR MARK ///
Network::Mark::~Mark() = default;

Network::Mark::Mark(uint64_t capacity, uint64_t prev_node)
	: capacity(capacity), prev_node(prev_node) { }


/// FOR VERTEX ///
Network::Vertex::~Vertex()
{
	delete routes;
	delete mark;
}

Network::Vertex::Vertex(uint64_t number, Map<uint64_t, uint64_t>* routes, Mark* mark)
	: number(number), mark(mark), routes(routes) { }


/// FOR PUBLIC IN NETWORK ///
Network::Network()
	: maximal_flow(0) { }


Network::Network(std::string& filepath)
	: maximal_flow(0)
{
	readFrom(filepath);
	create();
}


void Network::readFrom(std::string& filepath)
{
	bool start_exists = false, finish_exists = false; //for checking if network has source and runoff
	std::string line; //for storing one file line
	std::ifstream f(filepath);

	if (f.is_open())
	{
		while (std::getline(f, line))
		{
			auto arr = new std::string[INPUT_DATA_AMOUNT];

			if (line[1] != ' ' || line[3] != ' ') throw std::invalid_argument("wrong input format: vertex name");

			for (unsigned i = 0; i < line.length(); ++i)
			{
				if (line[i] == ' ')
					line.erase(i, 1);
			}

			arr[V1] = line[V1];
			arr[V2] = line[V2];
			arr[C] = line.erase(0,2);

			try
			{
				std::stoi(arr[C]);
			}
			catch (std::invalid_argument& e)
			{
				throw std::invalid_argument("wrong input format: bandwidth");
			}
			
			if (arr[V1] == arr[V2]) throw std::logic_error("loops are not allowed");
			if (arr[V2] == "S") throw std::logic_error("ways to the source can not exist");
			if (arr[V1] == "T") throw std::logic_error("ways from the runoff can not exist");

			if (!start_exists)
				if (arr[V1] == "S") start_exists = true;

			if (!finish_exists)
				if (arr[V2] == "T") finish_exists = true;

			input.push_back(arr);
		}

		if (input.getSize() == 0) throw std::invalid_argument("file was empty");

		if (!start_exists) throw std::logic_error("source is missing");

		if (!finish_exists) throw std::logic_error("runoff is missing");
	}
	else
		throw std::exception("could not open the file");

	f.close();
}


List<std::string*> Network::getInputData()
{
	return input;
}


void Network::create()
{
	uint64_t last_added = 1; //number of last added node

	/* creating matches between nodes names and numbers */
	number_name_match.insert("S", 1);
	
	for (size_t i = 0; i < input.getSize(); ++i)
	{
		const auto line = input.at(i);

		for (auto j = 0; j < 2; ++j)
		{
			auto cur = line[j]; //current symbol of input
			
			if(!number_name_match.contains(cur))
			{
				if (cur != "T" && cur != "S")
				{
					number_name_match.insert(cur, last_added + 1);
					last_added++;
				}
			}
		}
	}

	number_name_match.insert("T", last_added + 1);

	vertices = List<Vertex*>(number_name_match.getSize(), nullptr);
	
	/* creating network */
	for (size_t i = 0; i < input.getSize(); ++i)
	{
		const auto line = input.at(i);

		uint64_t first_vertex_num = number_name_match.find(line[V1]);
		uint64_t second_vertex_num = number_name_match.find(line[V2]);
		uint64_t capacity = std::stoi(line[C]);

		/* if vertex is not in the list yet */
		if (vertices.at(first_vertex_num - 1) == nullptr)
			vertices.set(first_vertex_num - 1, createVertex(first_vertex_num, second_vertex_num, capacity));
		else
		{
			Vertex* vertex = vertices.at(first_vertex_num - 1);
			if (vertex->routes->contains(second_vertex_num))
				vertex->routes->update(second_vertex_num, capacity);
			else
				vertex->routes->insert(second_vertex_num, capacity);
		}

		if (vertices.at(second_vertex_num - 1) == nullptr)
			vertices.set(second_vertex_num - 1, createVertex(second_vertex_num, first_vertex_num, 0));
		else
			vertices.at(second_vertex_num - 1)->routes->insert(first_vertex_num, 0);
		
	}
}

uint64_t Network::ford_fulkerson()
{
	uint64_t i = 1; //current vertex number (step 1)

	while(true)
	{
		auto current_vertex = vertices.at(i - 1);
		
		List<Vertex*> S; //list of possible transitions
		auto neighbors = current_vertex->routes->get_keys(); //list of all current's neighbors

		//filling the list of possible transitions (step 2)
		for (size_t j = 0; j < neighbors.getSize(); ++j)
		{
			auto neighbour_vertex = vertices.at(neighbors.at(j) - 1);

			/* if neighbor is not marked and remnant bandwidth is not zero */
			if (neighbour_vertex->mark == nullptr && current_vertex->routes->find(neighbour_vertex->number) != 0)
				S.push_back(neighbour_vertex);
			
		}

		/* counting next vertex (step 3) */
		if (!S.isEmpty())
		{
			uint64_t maximal_capacity = 0;
			Vertex* k = S.at(0); //vertex with maximal capacity in S

			for (size_t j = 0; j < S.getSize(); ++j)
			{
				auto current_capacity = current_vertex->routes->find(S.at(j)->number);

				if (current_capacity > maximal_capacity)
				{
					maximal_capacity = current_capacity;
					k = S.at(j);
				}
			}

			/* marking the vertex and reassign number of current vertex*/
			k->mark = new Mark(maximal_capacity, i);
			i = k->number;

			/* if i is not the runoff */
			if (i != vertices.getSize())
				continue; //goto step 2
			
		}
		else if (S.isEmpty()) // backup or finishing (step 4)
		{
			/*i f there is no way out of the source - maximal flow is found */
			if (i == SOURCE)
				break;

			/* back upping to the previous vertex to change other way*/
			i = current_vertex->mark->prev_node;
			current_vertex->mark->prev_node = 0;	

			continue; //goto step 2
		}

		/*if i is runoff (step 5)*/
		
		List<Vertex*> N; // list of all vertices on the path (all marked vertices excluding source and back upped vertices)

		/* filling N */
		for (size_t j = 0; j < vertices.getSize(); ++j)
			if (vertices.at(j)->mark != nullptr && vertices.at(j)->mark->prev_node != 0)
				N.push_back(vertices.at(j));

		auto current_max_f = INF;

		/* finding minimal capacity */
		for (size_t j = 0; j < N.getSize(); ++j)
			if (N.at(j)->mark->capacity < current_max_f)
				current_max_f = N.at(j)->mark->capacity;

		maximal_flow += current_max_f;

		/* recounting remnant bandwidths*/
		recountBandwidths(current_max_f);

		for (size_t j = 1; j < vertices.getSize(); ++j)
		{
			delete vertices.at(j)->mark;
			vertices.at(j)->mark = nullptr;
		}

		i = 1;
	}

	return maximal_flow;
}


/// FOR PRIVATE IN NETWORK ///

Network::Vertex* Network::createVertex(uint64_t number, uint64_t neighbor, uint64_t capacity)
{
	auto routes = new Map<uint64_t, uint64_t>(neighbor, capacity);
	Mark* mark = nullptr;

	/* the source is being created with [INF; 0] marking*/
	if (number == SOURCE)
		mark = new Mark(INF, 0);

	return new Vertex(number, routes, mark);
}

void Network::recountBandwidths(uint64_t f)
{
	Vertex* current = vertices.at(vertices.getSize() - 1); // getting the runoff

	do
	{
		Vertex* prev = vertices.at(current->mark->prev_node - 1);
		current->routes->update(prev->number, current->routes->find(prev->number) + f);
		
		prev->routes->update(current->number, prev->routes->find(current->number) - f);

		current = vertices.at(prev->number - 1);
	}
	while (current->number != SOURCE);
}
