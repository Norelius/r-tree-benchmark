#include "neuron.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <boost/math/quaternion.hpp>

namespace rtreebench {

  Neuron::Neuron(const char* filename) {
    std::ifstream in(filename);
    points.reserve(1500);
    std::string line;
    if (in.is_open()) {
        // skip comments
      while (true) {
        getline (in,line);
        if (!(line[0] == '#')) {
          break;
        }
      }
      NeuronPoint first(line);
      points.push_back(first);

      int id, structure, parent;
      double radius;
      double cords[3];
      while (in >> id >> structure >> cords[0] >> cords[1] >> cords[2] >> radius >> parent) {
        NeuronPoint np = {id, structure, parent, radius, &cords[0]};
        points.push_back(np);
      }
      in.close();
    } else {
      throw std::invalid_argument("No valid inputfile");
    }
  }

  Neuron::Neuron(const Neuron& rhs) {
    points = rhs.points;
  }

  void Neuron::rotate(::boost::math::quaternion<double> quaternion) {
    // TODO
  }

  void Neuron::move(double xd, double yd, double zd){
    for (auto itr = points.begin(); itr != points.end(); ++itr) {
      itr->moveCoordinates(xd, yd, zd);
    }
  }

  NeuronPoint::NeuronPoint() {}

  NeuronPoint::NeuronPoint(int pid, int ptype, int pparent, double pradius, double px, double py, double pz) :
    id(pid), type(ptype), parent(pparent), radius(pradius), coordinates{px, py, pz} {}


  NeuronPoint::NeuronPoint(int pid, int ptype, int pparent, double pradius, const double* pcoordinates) :
    id(pid), type(ptype), parent(pparent), radius(pradius), coordinates{pcoordinates[0], 
    pcoordinates[1], pcoordinates[2]} {}

  NeuronPoint::NeuronPoint(const NeuronPoint& rhs) : id(rhs.id), type(rhs.type), 
    parent(rhs.parent), radius(rhs.radius), coordinates{rhs.coordinates[0], 
    rhs.coordinates[1], rhs.coordinates[2]} {}

  NeuronPoint::NeuronPoint(std::string &s) {
    std::istringstream ss(s);
    ss >> id >> type >> coordinates[0] >> coordinates[1] >> coordinates[2] >> radius >> parent;
  }

  void NeuronPoint::setCoordinates(double x, double y, double z) {
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
  }

  void NeuronPoint::moveCoordinates(double xd, double yd, double zd) {
    coordinates[0] += xd;
    coordinates[1] += yd;
    coordinates[2] += zd;
  }

  std::ostream& operator<<(std::ostream& os, const NeuronPoint& npt) {
    os << "{id=" << npt.id << " type=" << npt.type << " parent=" << npt.parent
    << " radius=" << npt.radius << " point=[" << npt.coordinates[0] << ", " 
    << npt.coordinates[1] << ", " << npt.coordinates[2] << "]}";
    return os;
  }
}
