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
const double deviation = 300;
double dev[] = {260, 140, 350};
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

// Adds n copies of the given neuron that will be slightly shifted from the
// base, up to at most +- d in any dimension.
void shimmyAndAddNeurons(SpatialIndex::ISpatialIndex* tree, Neuron& base, int n, double* d) {

  // Rng double.
  generator_type generator(32);
  boost::uniform_real<> uni_dist_x(-d[0]/2,d[0]/2);
  boost::variate_generator<generator_type, boost::uniform_real<>> unix(generator, uni_dist_x);
  boost::uniform_real<> uni_dist_y(-d[1]/2,d[1]/2);
  boost::variate_generator<generator_type, boost::uniform_real<>> uniy(generator, uni_dist_y);
  boost::uniform_real<> uni_dist_z(-d[2]/2,d[2]/2);
  boost::variate_generator<generator_type, boost::uniform_real<>> uniz(generator, uni_dist_z);

  if (verbose) {
    std::cout << "Adding 1+" << n << " neurons with a deviation of [" << d[0] << ", " << d[1] << ", " << d[2] << "]" << std::endl;
  }
  for (int i = 0; i < n; ++i) {
    Neuron nd = Neuron(base);
    // TODO: make explicit methods for different distrubutions.
    double xd = unix();
    double yd = uniy();
    double zd = uniz();
    if (verbose) {
      std::cout << i+1 << ": radomized deviation [" << xd << ", " << yd << ", " << zd << "]" << std::endl;
    }
    nd.move(xd, yd, zd);
    addNeuron(tree, nd);
  }
}

int main(int argc, char const *argv[]) {
  int n = default_number_neurons;
  if (argc >= 2) {
    n = std::atoi(argv[1]);
  }
  if (argc >= 3) {

  }

  // Create main memory storage manager and tree.
  IStorageManager* store = createNewMemoryStorageManager();
  uint32_t const dimension = 3;
  double const fill = 0.7; // default
  uint32_t indexCapacity = 80;
  uint32_t leafCapacity = 80;
  SpatialIndex::RTree::RTreeVariant rv =  SpatialIndex::RTree::RTreeVariant::RV_RSTAR;
  id_type indexIdentifier;
  SpatialIndex::ISpatialIndex* tree = RTree::createNewRTree(*store, fill, 
    indexCapacity, leafCapacity, dimension, rv, indexIdentifier);

  // Populate tree.
  Neuron beguin(beguinfile);
    // Perform range queries.
  double ac[3] = {-11.2, -30.3, -102};
  Point a = Point(&ac[0], 3);
  double bc[3] = {1.3, 8, 1};
  Point b = Point(&bc[0], 3);
  Region region(a, b);
  MyVisitor visitor;
  


  if (!verbose) {
    std::cout << "# Neurons Points Time Reads" << std::endl;
  }
  for (int i = 1; i < 8; ++i) {
    shimmyAndAddNeurons(tree, beguin, 5*n, dev);
    IStatistics* stats;
    tree->getStatistics(&stats); 
    int reads = stats->getReads();

    auto start = std::chrono::high_resolution_clock::now();
    tree->intersectsWithQuery(region, visitor);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    tree->getStatistics(&stats);
    reads = stats->getReads() - reads;

    if (verbose) {
      std::cout << std::endl << *tree <<  "Query time: " << elapsed.count()*1000 << "ms" << std::endl;
    } else {
      std::cout << i*n*5 << " " << stats->getNumberOfData() << " " << elapsed.count()*1000 << " " << reads << std::endl;
    }
  }
  return 0;
}
