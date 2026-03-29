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
        Reviewer() = default;
        Reviewer(int id, string name, string email, int primary, int secondary);

        int getId() const;
        string getName() const;
        string getEmail() const;
        int getPrimary() const;
        int getSecondary() const;

        void setId(int id);
        void setName(string name);
        void setEmail(string email);
        void setPrimary(int primary);
        void setSecondary(int secondary);
};

#endif //DA_PROJECT_1_REVIEWER_H