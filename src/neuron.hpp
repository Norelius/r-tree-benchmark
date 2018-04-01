#include <vector>
#include <fstream>
#include <boost/math/quaternion.hpp>

namespace rtreebench {

  struct NeuronPoint {
    int id, type, parent;
    double radius;
    double coordinates[3];

    NeuronPoint();
    NeuronPoint(int pid, int ptype, int pparent, double pradius, double px, double py, double pz);
    NeuronPoint(int pid, int ptype, int pparent, double pradius, const double* pcoordinates);
    NeuronPoint(const NeuronPoint&);
    NeuronPoint(std::string &s);
    // ~NeuronPoint(); arrays are not dynamically allocated


    void setCoordinates(double x, double y, double z);
  };

  struct Neuron {
    std::vector<NeuronPoint> points;

    Neuron(std::ifstream &in);
    Neuron(const Neuron& rhs);

    void rotate(::boost::math::quaternion<double> quaternion);
    void move(double xd, double yd, double zd);
  };

  std::ostream& operator<<(std::ostream& os, const NeuronPoint& npt);
}