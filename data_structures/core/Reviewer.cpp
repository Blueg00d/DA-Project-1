#include "Reviewer.h"
#include <string>
using namespace std;

//contructor
Reviewer::Reviewer(int id, string name, string email, int primary, int secondary) {
    this->id = id;
    this->name = name;
    this->email = email;
    this->primary = primary;
    this->secondary = secondary;
}

//getters
int Reviewer::getId() const { return id; }
string Reviewer::getName() const { return name; }
string Reviewer::getEmail() const { return email; }
int Reviewer::getPrimary() const { return primary; }
int Reviewer::getSecondary() const { return secondary; }

//setters
void Reviewer::setId(int id) { this->id = id; }
void Reviewer::setName(string name) { this->name = name; }
void Reviewer::setEmail(string email) { this->email = email; }
void Reviewer::setPrimary(int primary) { this->primary = primary; }
void Reviewer::setSecondary(int secondary) { this->secondary = secondary; }