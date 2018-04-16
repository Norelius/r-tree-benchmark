#include <iostream>
#include <vector>
#include <iomanip> // std::setprecision for double formating
#include <spatialindex/SpatialIndex.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include "neuron.hpp"


using namespace rtreebench;
using namespace SpatialIndex;
using namespace SpatialIndex::StorageManager;

const char* beguinfile = "../data/control-1-wt.CNG.swc";
const int default_number_neurons = 1000;
const int deviation = 300;
const bool verbose = false;

typedef boost::mt19937 generator_type;

SpatialIndex::id_type uid = 1;


struct MyVisitor : public SpatialIndex::IVisitor {
  size_t m_indexIO;
  size_t m_leafIO;
  MyVisitor() :  m_indexIO(0), m_leafIO(0) {}

  void visitNode(const INode& n)
  {
    n.isLeaf() ? ++m_leafIO : ++m_indexIO;
  }

  void visitData(const IData& d) {
    if (verbose) {
      IShape* ps;
      d.getShape(&ps);
      SpatialIndex::Point p;
      ps->getCenter(p);

      std::cout << std::setprecision(3) << d.getIdentifier() << " [" << p << "]" << std::endl;
      delete ps;
    }
  }

  void visitData(std::vector<const IData*>& v) {
    std::cout << v[0]->getIdentifier() << " " << v[1]->getIdentifier() << std::endl;
  }
};

void addNeuron(SpatialIndex::ISpatialIndex* tree, Neuron &n) {
  for (auto itr = n.points.begin(); itr != n.points.end(); ++itr) {
    SpatialIndex::IShape* p = 0;
    p = new SpatialIndex::Point(&itr->coordinates[0], 3);
    uint8_t* pData = 0;
    uint32_t pDataLength = 0; //sizeof(*np)
    // const NeuronPoint *np = new NeuronPoint(*itr);
    tree->insertData(pDataLength, pData, *p, uid++);
  }
}

// Adds the given neuron and also adds n copies of it that will be slightly shifted from the
// base, up to at most +- d in any dimension.
void shimmyAndAddNeurons(SpatialIndex::ISpatialIndex* tree, Neuron& base, int n, double d) {
  addNeuron(tree, base);

  // Rng double.
  generator_type generator(32);
  boost::uniform_real<> uni_dist(-deviation,deviation);
  boost::variate_generator<generator_type, boost::uniform_real<>> uni(generator, uni_dist);

  if (verbose) {
    std::cout << "Adding 1+" << n << " neurons with a deviation of " << deviation << std::endl;
  }
  for (int i = 0; i < n; ++i) {
    Neuron nd = Neuron(base);
    // TODO: make explicit methods for different distrubutions.
    double xd = uni();
    double yd = uni();
    double zd = uni();
    if (verbose) {
      std::cout << i+1 << ": radomized deviation [" << xd << ", " << yd << ", " << zd << "]" << std::endl;
    }
    nd.move(xd, yd, zd);
    addNeuron(tree, nd);
  }
}

int main(int argc, char const *argv[]) {
  int n = default_number_neurons;
  if (argc == 2) {
    n = std::atoi(argv[1]);
  }

  // Create main memory storage manager and tree.
  IStorageManager* store = createNewMemoryStorageManager();
  uint32_t const dimension = 3;
  double const fill = 0.7; // default
  uint32_t indexCapacity = 100;
  uint32_t leafCapacity = 100;
  SpatialIndex::RTree::RTreeVariant rv =  SpatialIndex::RTree::RTreeVariant::RV_RSTAR;
  id_type indexIdentifier;
  SpatialIndex::ISpatialIndex* tree = RTree::createNewRTree(*store, fill, 
    indexCapacity, leafCapacity, dimension, rv, indexIdentifier);

  // Populate tree.
  Neuron beguin(beguinfile);
  shimmyAndAddNeurons(tree, beguin, n, deviation);

  // Perform range queries.
  double ac[3] = {-11.2, -30.3, -102};
  Point a = Point(&ac[0], 3);
  double bc[3] = {1.3, 8, 1};
  Point b = Point(&bc[0], 3);
  Region region(a, b);
  MyVisitor visitor;

  auto start = std::chrono::high_resolution_clock::now();
  tree->intersectsWithQuery(region, visitor);
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  std::cout << std::endl << *tree <<  "Query time: " << elapsed.count()*1000 << "ms" << std::endl;

  return 0;
}

