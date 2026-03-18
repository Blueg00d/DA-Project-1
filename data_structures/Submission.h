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

    Submission();
    Submission(int id, string title, string author, string email, int primary, int secondary);

    int getId();
    string getTitle();
    string getAuthor();
    string getEmail();
    int getPrimary();
    int getSecondary();

    void setId(int id);
    void setTitle(string title);
    void setAuthor(string author);
    void setEmail(string email);
    void setPrimary(int primary);
    void setSecondary(int secondary);
};


#endif //DA_PROJECT_1_SUBMISSION_H