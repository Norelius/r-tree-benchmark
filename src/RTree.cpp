#include "neuron.hpp"
#include <boost\geometry\index\rtree.hpp>
#include <boost/geometry/geometries/register/point.hpp> 
#include <iostream>
#include <vector>
#include <boost/geometry/geometries/box.hpp>
#include <boost/foreach.hpp>
#include <boost/geometry/io/wkt/write.hpp> 
#include <chrono>
//using namespace rtreebench;

namespace bg = boost::geometry;
namespace bgm = boost::geometry::model;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 3, bg::cs::cartesian> point;
typedef bg::model::box<point> box;


const char* beguinfile = "C:/Users/Anton/source/repos/Project1/Project1/input2.txt";


void insertNeuron(rtreebench::Neuron aNeuron, bgi::rtree<point, bgi::linear<16>> *tree) {
	std::vector<point> vec;

	//create points from Neuron
	for (int i = 0; i < aNeuron.points.size(); i++) {
		point a_point(aNeuron.points[i].coordinates[0], aNeuron.points[i].coordinates[1], aNeuron.points[i].coordinates[2]);
		vec.push_back(a_point);
	}
	tree->insert(vec.begin(), vec.end());

}


int main(int argc, char const *argv[])
{
	
	rtreebench::Neuron aNeuron(beguinfile);
	
	// create the rtree
	bgi::rtree< point, bgi::linear<16> > rtree;

	//insert points
	insertNeuron(aNeuron, &rtree);


	//create box for query
	box query_box(point(-10, -110, -110), point(20, 20,100));
	std::vector<point> result_s;

	//take time
	auto start = std::chrono::high_resolution_clock::now();
	rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));
	auto finish = std::chrono::high_resolution_clock::now();

	//print result
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << std::endl << "Time: "<< elapsed.count()*1000 << "ms" << std::endl;


	getchar();

	
	return 0;

}