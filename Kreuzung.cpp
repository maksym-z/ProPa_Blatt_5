#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <sstream>

#include "kreuzung.hpp"

class fahrzeugistream {
  std::vector<fahrzeug>& spur;
public:
  fahrzeugistream(std::vector<fahrzeug>& spur) : spur(spur) {}
  fahrzeugistream& operator<<(const fahrzeug& f) {
    spur.push_back(f);
    return *this;
  }
};

fahrzeugistream stream_for(kreuzung& k, const std::string& input) {
  if (input == "A1") {
    return fahrzeugistream(k.spuren[A1]);
  } else if (input == "A2") {
    return fahrzeugistream(k.spuren[A2]);
  } else if (input == "B1") {
    return fahrzeugistream(k.spuren[B1]);
  } else if (input == "B2") {
    return fahrzeugistream(k.spuren[B2]);
  } else if (input == "C") {
    return fahrzeugistream(k.spuren[C]);
  } else if (input == "D1") {
    return fahrzeugistream(k.spuren[D1]);
  } else if (input == "D2") {
    return fahrzeugistream(k.spuren[D2]);
  } else {
    throw std::runtime_error("Unbekannte Spur: " + input);
  }
}

int main(int argc, char* argv[]) {
  std::istream *in;
  if (argc == 2) {
    in = new std::ifstream(argv[1]);
  } else {
    in = &std::cin;
  }
  try {
    kreuzung k;
    std::string line;
    while (!in->eof() && std::getline(*in, line)) {
      std::stringstream ss(line);
      std::string input;
      ss >> input;
      if (input == "") break;
      fahrzeugistream fis = stream_for(k, input);
      while (!ss.eof()) {
        ss >> input;
        if (input == "l") {
          fis << LINKS;
        } else if (input == "r") {
          fis << RECHTS;
        } else if (input == "g") {
          fis << GERADEAUS;
        } else {
          throw std::runtime_error("Unbekannte Fahrtrichtung: " + input);
        }
      }
    }
    std::cout << "Die Kreuzung ist leer nach " << k.simuliere() <<
        " Sekunden." << std::endl;
    return 0;
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
