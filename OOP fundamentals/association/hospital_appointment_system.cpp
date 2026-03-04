#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Room {
private:
  string number;
  int floor;
public:
  Room(const string& number, int floor) : number(number), floor(floor) {}
  string getNumber() const { return number; }
  int getFloor() const { return floor; }
};

class Doctor;
class Patient;

class Appointment {
private:
  Doctor* doctor;
  Patient* patient;
  Room* room;
  string time;

public:
  Appointment(Doctor* doctor, Patient* patient, Room* room, const string& time);
  Doctor* getDoctor() const { return doctor; }
  Patient* getPatient() const { return patient; }
  Room* getRoom() const { return room; }
  string getTime() const { return time; }
};

class Doctor {
private:
  string name;
  string specialization;
  vector<Appointment*> appointments;
public:
  Doctor(const string& name, const string& specialization) : name(name), specialization(specialization) {}

  void addAppointment(Appointment* appt) {
    appointments.push_back(appt);
  }

  vector<Patient*> getPatients() const;

  string getName() const { return name; }
  string getSpecialization() const { return specialization; }
  vector<Appointment*> getAppointments() const { return appointments; }
};

class Patient {
private:
  string name;
  vector<Appointment*> appointments;
public:
  Patient(const string& name) : name(name) {}

  void addAppointment(Appointment* appt) {
    appointments.push_back(appt);
  }

  vector<Doctor*> getDoctors() const;
  string getName() const { return name; }
  vector<Appointment*> getAppointments() const { return appointments; }
};

Appointment::Appointment(Doctor* doctor, Patient* patient, Room* room, const string& time) 
  : doctor(doctor), patient(patient), room(room), time(time) {
    doctor->addAppointment(this);
    patient->addAppointment(this);
}

vector<Patient*> Doctor::getPatients() const {
  vector<Patient*> result;
  for(auto* appt: appointments) {
    auto* p = appt->getPatient();
    if(find(result.begin(), result.end(), p) == result.end()) {
      result.push_back(p);
    }
  }

  return result;
}

vector<Doctor*> Patient::getDoctors() const {
  vector<Doctor*> result;
  for(auto* appt : appointments) {
    auto* d = appt->getDoctor();
    if(find(result.begin(), result.end(), d) == result.end()) {
      result.push_back(d);
    }
  }

  return result;
}

int main() {
  Doctor drSmith("Dr. Smith", "Cardiology");
  Doctor drPatel("Dr. Patel", "Neurology");

  Patient alice("Alice");
  Patient bob("Bob");

  Room room101("101", 1);
  Room room205("205", 2);

  Appointment a1(&drSmith, &alice, &room101, "9:00 AM");
  Appointment a2(&drSmith, &bob, &room101, "10:00 AM");
  Appointment a3(&drPatel, &alice, &room205, "2:00 PM");

  cout << drSmith.getName() << "'s patients:" << endl;
    for (auto* p : drSmith.getPatients())
        cout << "  - " << p->getName() << endl;

    cout << alice.getName() << "'s doctors:" << endl;
    for (auto* d : alice.getDoctors())
        cout << "  - " << d->getName() << " (" << d->getSpecialization() << ")" << endl;

    cout << drSmith.getName() << "'s schedule:" << endl;
    for (auto* a : drSmith.getAppointments())
        cout << "  - " << a->getTime() << " with " << a->getPatient()->getName()
             << " in Room " << a->getRoom()->getNumber() << endl;

    return 0;
}


