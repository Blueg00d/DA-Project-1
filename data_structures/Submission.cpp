#include "Submission.h"
#include <string>
using namespace std;

Submission() {
    //necessário?
}

//constructor
Submission::Submission(int id, string title, string author, string email, int primary, int secondary) {
    this->id = id;
    this->title = title;
    this->author = author;
    this->email = email;
    this->primary = primary;
    this->secondary = secondary;
}

//getters
int Submission::getId() { return id; }
string Submission::getTitle() { return title; }
string Submission::getAuthor() { return author; }
string Submission::getEmail() { return email; }
int Submission::getPrimary() { return primary; }
int Submission::getSecondary() { return secondary; }

//setters
void Submission::setId(int id) { this->id = id; }
void Submission::setTitle(string title) { this->title = title; }
void Submission::setAuthor(string author) { this->author = author; }
void Submission::setEmail(string email) { this->email = email; }
void Submission::setPrimary(int primary) { this->primary = primary; }
void Submission::setSecondary(int secondary) { this->secondary = secondary; }
