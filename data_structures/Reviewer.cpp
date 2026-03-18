#include "Reviewer.h"
#include <string>
using namespace std;

Reviewer() {
    //necessário?
}

//contructor
Reviewer::Reviewer(int id, string name, string email, int primary, int secondary) {
    this->id = id;
    this->name = name;
    this->email = email;
    this->primary = primary;
    this->secondary = secondary;
}

//getters
int Reviewer::getId() { return id; }
string Reviewer::getName() { return name; }
string Reviewer::getEmail() { return email; }
int Reviewer::getPrimary() { return primary; }
int Reviewer::getSecondary() { return secondary; }

//setters
void Reviewer::setId(int id) { this->id = id; }
void Reviewer::setName(string name) { this->name = name; }
void Reviewer::setEmail(string email) { this->email = email; }
void Reviewer::setPrimary(int primary) { this->primary = primary; }
void Reviewer::setSecondary(int secondary) { this->secondary = secondary; }