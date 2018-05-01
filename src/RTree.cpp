#include "neuron.hpp"
#include <boost\geometry\index\rtree.hpp>
#include <boost/geometry/geometries/register/point.hpp> 
#include <iostream>
#include <vector>
#include <iomanip> // std::setprecision for double formating
#include <boost/geometry/geometries/box.hpp>
#include <boost/foreach.hpp>
#include <boost/geometry/io/wkt/write.hpp> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <exception>
//using namespace rtreebench;

namespace bg = boost::geometry;
namespace bgm = boost::geometry::model;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 3, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef boost::mt19937 generator_type;

const int default_number_neurons = 1000;
const int elemPerNodes = 80;
const double deviation = 300;
double dev[] = { 260, 140, 350 };


const char* beguinfile = "C:/Users/Anton/source/repos/Project1/Project1/input2.txt";

//using namespace SpatialIndex;


void insertNeuron(rtreebench::Neuron aNeuron, bgi::rtree<point, bgi::quadratic<elemPerNodes>> *tree) {
	std::vector<point> vec;

	for (auto itr = aNeuron.points.begin(); itr != aNeuron.points.end(); ++itr) {
		point a_point(itr->coordinates[0], itr->coordinates[1], itr->coordinates[2]);
		tree->insert(a_point);
	}
}


void insertNeuronToArray(rtreebench::Neuron aNeuron, std::vector<point>* vec) {
	for (int i = 0; i < aNeuron.points.size(); i++) {
		point a_point(aNeuron.points[i].coordinates[0], aNeuron.points[i].coordinates[1], aNeuron.points[i].coordinates[2]);
		vec->push_back(a_point);
	}
}


void shimmyAndAddNeurons(bgi::rtree<point, bgi::quadratic<elemPerNodes>> *tree, rtreebench::Neuron& base, int n, double* d) {

	// Rng double.
	generator_type generator(32);
	boost::uniform_real<> uni_dist_x(-d[0] / 2, d[0] / 2);
	boost::variate_generator<generator_type, boost::uniform_real<>> unix(generator, uni_dist_x);
	boost::uniform_real<> uni_dist_y(-d[1] / 2, d[1] / 2);
	boost::variate_generator<generator_type, boost::uniform_real<>> uniy(generator, uni_dist_y);
	boost::uniform_real<> uni_dist_z(-d[2] / 2, d[2] / 2);
	boost::variate_generator<generator_type, boost::uniform_real<>> uniz(generator, uni_dist_z);

	std::vector<point>* vec = new std::vector<point>();
	for (int i = 0; i < n; ++i) {
		rtreebench::Neuron nd = rtreebench::Neuron(base);
		// TODO: make explicit methods for different distrubutions.
		double xd = unix();
		double yd = uniy();
		double zd = uniz();
		
		nd.move(xd, yd, zd);
		//insertNeuronToArray(nd, vec);
		insertNeuron(nd, tree);
	}

	//tree->insert(vec->begin(), vec->end());
}


int main(int argc, char const *argv[])
{
	int n = default_number_neurons;
	rtreebench::Neuron aNeuron(beguinfile);
	
	// create the rtree
	bgi::rtree< point, bgi::quadratic<elemPerNodes> >* rtree;

	//parameter sets maximum number of elements in  nodes
	rtree = new bgi::rtree< point, bgi::quadratic<elemPerNodes>>();
	//insert points
	insertNeuron(aNeuron, rtree);


	//create box for query
	box query_box(point(-10, -110, -110), point(20, 20,100));
	

	//take time
	//auto start = std::chrono::high_resolution_clock::now();
	//rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));
	//auto finish = std::chrono::high_resolution_clock::now();

	//print result
	//std::chrono::duration<double> elapsed = finish - start;
	//std::cout << std::endl << "Time: "<< elapsed.count()*1000 << "ms" << std::endl;


	for (int i = 1; i < 8; ++i) {

		std::vector<point> result_s;
		shimmyAndAddNeurons(rtree, aNeuron, 100, dev);
		//IStatistics* stats;
		//rtree->getStatistics(&stats);
		//int reads = stats->getReads();

		auto start = std::chrono::high_resolution_clock::now();
		rtree->query(bgi::intersects(query_box), std::back_inserter(result_s));
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;

		//rtree.getStatistics(&stats);
		//reads = stats->getReads() - reads;

		std::cout << i*n*5 << "  size: " << result_s.size() << "  time: " << elapsed.count() * 1000 << "ms "  << std::endl;
	}


	getchar();

	
	return 0;

}