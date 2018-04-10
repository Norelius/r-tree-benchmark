#include <iostream>
#include <vector>
#include <iomanip> // std::setprecision for double formating
#include <spatialindex/SpatialIndex.h>
#include "neuron.hpp"


using namespace std;
using namespace rtreebench;
using namespace SpatialIndex;
using namespace SpatialIndex::StorageManager;

const char* beguinfile = "data/control-1-wt.CNG.swc";

int main(int argc, char const *argv[]) {
  // Main memory storage manager
  IStorageManager* store = createNewMemoryStorageManager();
  uint32_t const dimension = 3;
  double const fill = 0.7; // default
  uint32_t indexCapacity = 100;
  uint32_t leafCapacity = 100;
  SpatialIndex::RTree::RTreeVariant rv =  SpatialIndex::RTree::RTreeVariant::RV_RSTAR;
  id_type indexIdentifier;
  SpatialIndex::ISpatialIndex* tree = RTree::createNewRTree(*store, fill, 
    indexCapacity, leafCapacity, dimension, rv, indexIdentifier);

  Neuron beguin(beguinfile);


  return 0;
}