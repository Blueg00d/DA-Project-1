#ifndef DA_PROJECT_1_REVIEWER_H
#define DA_PROJECT_1_REVIEWER_H
#include <string>
using namespace std;

class Reviewer {
    private:
        int id;
        string name;
        string email;
        int primary;
        int secondary;

    public:
        Reviewer();
        Reviewer(int id, string name, string email, int primary, int secondary);

        int getId();
        string getName();
        string getEmail();
        int getPrimary();
        int getSecondary();

        void setId(int id);
        void setName(string name);
        void setEmail(string email);
        void setPrimary(int primary);
        void setSecondary(int secondary);
};

#endif //DA_PROJECT_1_REVIEWER_H