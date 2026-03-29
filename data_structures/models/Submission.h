#ifndef DA_PROJECT_1_SUBMISSION_H
#define DA_PROJECT_1_SUBMISSION_H
#include <string>
using namespace std;

class Submission {
    private:
        int id;
        string title;
        string author;
        string email;
        int primary;
        int secondary;
    public:

    Submission() = default;
    Submission(int id, string title, string author, string email, int primary, int secondary);

    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    string getEmail() const;
    int getPrimary() const;
    int getSecondary() const;

    void setId(int id);
    void setTitle(string title);
    void setAuthor(string author);
    void setEmail(string email);
    void setPrimary(int primary);
    void setSecondary(int secondary);
};


#endif //DA_PROJECT_1_SUBMISSION_H