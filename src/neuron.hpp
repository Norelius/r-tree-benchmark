#include <vector>
#include <boost/math/quaternion.hpp>

namespace rtreebench {

  // One point of a neuron in swc format. Containing coordinates and meta informatuion.
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
    void moveCoordinates(double xd, double yd, double zd);
  };

  struct Neuron {
    std::vector<NeuronPoint> points;

    // Reads a neuron from an swc file.
    Neuron(const char* filename);
    // Copy constructor.
    Neuron(const Neuron& rhs);

    // Rotates the whole neuron given a quaternion.
    void rotate(::boost::math::quaternion<double> quaternion);
    // Move the whole neuron while preserving the rotation.
    void move(double xd, double yd, double zd);
  };

  std::ostream& operator<<(std::ostream& os, const NeuronPoint& npt);
}