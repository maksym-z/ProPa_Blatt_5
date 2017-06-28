#include <array>
#include <vector>
enum fahrzeug {
  LINKS, RECHTS, GERADEAUS
};

enum spur {
  A1 = 0, A2 = 1, B1 = 2, B2 = 3, C = 4, D1 = 5, D2 = 6, SPUR_ANZAHL = 7 // keine Spur, sondern ein Meta-Wert
};

// ermöglicht es, auf einen Wert vom Typ spur den ++ Operator anzuwenden.
// wird zB in leer() benutzt.
spur operator++(spur& s) {
  s = static_cast<spur>(static_cast<int>(s) + 1);
  return s;
}

typedef int sekunden;

struct traffic_light_state {
  // gruen[B2] gibt zB an, ob die Spur B2 grün ist
  std::array<bool, SPUR_ANZAHL> gruen;
  // Anzahl Sekunden, die diese Ampelphase andauert
  sekunden laenge;
};

class kreuzung {
  // Ampelzustände, übertragen aus der Aufgabe.
  const std::vector<traffic_light_state> light_states = { { { true, false, true,
      false, false, false, false }, 25 }, { { false, false, false, false, false,
      false, false }, 5 }, { { false, true, false, true, false, false, false },
      15 }, { { false, false, false, false, false, false, false }, 5 }, { {
      false, false, false, false, true, true, true }, 20 }, { { false, false,
      false, false, false, false, false }, 5 } };

  // Ordnet jeder Spur eine Gegenverkehr-Spur zu, auf die geachtet werden muss,
  // falls ein Auto links abbiegt. Ist der Wert einer Spur SPUR_ANZAHL, dann
  // hat diese Spur keine Gegenverkehrspur.
  const std::array<spur, SPUR_ANZAHL> gegenverkehr { { SPUR_ANZAHL, SPUR_ANZAHL,
      SPUR_ANZAHL, SPUR_ANZAHL, D1, SPUR_ANZAHL, C } };

  // Ordnet jedem Fahrzeugtyp die Dauer zu, die dieser braucht, um die
  // Kreuzung zu überqueren.
  const std::array<sekunden, 3> dauer_richtung { { 4, 3, 2 } };

  // Anzahl Sekunden, die eine Spur noch belegt ist in dem Fall, dass ein
  // Fahrzeug von dieser Spur aus gerade über die Kreuzung fährt. Dies ist zB
  // wichtig für die leer() Überprüfung, kann aber auch im Algorithmus benutzt
  // werden.
  std::array<sekunden, SPUR_ANZAHL> belegt;

  /* Prüft, ob die Kreuzung leer ist (keine Fahrzeuge fahren mehr darüber und
   * keine Fahrzeuge warten noch auf einer der Spuren).
   */
  bool leer() {
    for (spur s = A1; s < SPUR_ANZAHL; ++s) {
      if (belegt[s] > 0 || spuren[s].size() > 0) {
        return false;
      }
    }
    return true;
  }

  // testet, ob das aktuelle Fahrzeug auf der gegeneben Spur ein Linksabbieger
  // ist und Gegenverkehr zu erwarten hat. Es wird nicht getestet, ob auf der
  // gegenspur auch tatsächlich Fahrzeuge fahren!
  bool linksabbieger_mit_gegenverkehr(spur s) {
    return gegenverkehr[s] != SPUR_ANZAHL && spuren[s].back() == LINKS;
  }

  // Falls Sie weitere Hilfsfunktionen brauchen, können Sie diese hier einfügen.
public:
  // Konstruktor. Setzt alle belegt-Werte auf 0.
  kreuzung() :
      belegt { 0, 0, 0, 0, 0, 0, 0 } {
  }

  // Fahrzeuge, die auf den Spuren stehen. Das letzte (!) Fahrzeug steht vorne.
  // Dies ist so definiert, damit bei der Implementierung mit pop_back() einfach
  // das aktuelle Fahrzeug entfernt werden kann.
  std::array<std::vector<fahrzeug>, SPUR_ANZAHL> spuren;
  sekunden simuliere();
};

// implementiert die oben deklarierte Methode. Mit dieser Syntax lassen sich
// Methoden außerhalb der Klassendeklaration definieren.
sekunden kreuzung::simuliere() {
  // aktueller Ampelzustand. Dieser Wert ist ein Iterator; man kann mit dem ->
  // operator auf die darunterliegenden Werte zugreifen.
  // light_state->gruen[A1] gewährt zum Beispiel Zugriff auf den Grün-Zustand
  // von Spur A1.
  auto light_state = light_states.begin();

  // insgesamt verbrauchte Sekunden
  int secs = 0;

  // im aktuellen Zustand verbrauchte Sekunden
  int state_secs = 0;

  // Hauptschleife: Iteriere, bis die Kreuzung leer ist.
  while (!leer()) {
    // Fügen Sie Ihre Implementierung an dieser Stelle ein!
    for (spur s = A1; s < SPUR_ANZAHL; ++s) {
      bool got_cars = spuren[s].size() > 0;
      bool can_go = (light_state->gruen[s]) && belegt[s] == 0;
      bool got_traffic = spuren[gegenverkehr[s]].size()>0 && spuren[gegenverkehr[s]].back()!=LINKS;
      bool left_blocked = spuren[s].size()>0 && linksabbieger_mit_gegenverkehr(s) && got_traffic;
      bool last_travelling = spuren[s].size()>0 &&  linksabbieger_mit_gegenverkehr(s) && belegt[gegenverkehr[s]] > 1;
      if (got_cars && can_go && !left_blocked && !last_travelling) {
        fahrzeug current_car = spuren[s].back();
        spuren[s].pop_back();
        belegt[s]=dauer_richtung[current_car];
        belegt[gegenverkehr[s]]=dauer_richtung[current_car];
      }
    }
    for (spur s = A1; s < SPUR_ANZAHL; ++s) {
      if (belegt[s]>0) {
        --belegt[s];
      }
    }
    // Erhöhe Sekunden und gehe wenn nötig in den nächsten Ampelzustand über.
    secs++, state_secs++;
    if (state_secs == light_state->laenge) {
      ++light_state;
      state_secs = 0;
      if (light_state == light_states.end()) {
        light_state = light_states.begin();
      }
    }

  }
  return secs;
}
