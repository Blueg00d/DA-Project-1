#ifndef DA_PROJECT_1_REVIEWER_H
#define DA_PROJECT_1_REVIEWER_H
#include <string>
using namespace std;

/**
 * @class Reviewer
 * @brief Represents a reviewer available to evaluate conference submissions.
 */
class Reviewer {
private:
    int id;
    string name;
    string email;
    int primary;
    int secondary;

public:
    /** @brief Default constructor. */
    Reviewer() = default;

    /**
     * @brief Parameterized constructor for Reviewer.
     * @param id The ID of the reviewer.
     * @param name The reviewer's name.
     * @param email The reviewer's email.
     * @param primary The primary domain topic ID they can review.
     * @param secondary The secondary domain topic ID they can review.
     */
    Reviewer(int id, string name, string email, int primary, int secondary);

    /** @brief Gets the reviewer ID. */
    int getId() const;
    /** @brief Gets the reviewer's name. */
    string getName() const;
    /** @brief Gets the reviewer's email. */
    string getEmail() const;
    /** @brief Gets the primary domain topic ID. */
    int getPrimary() const;
    /** @brief Gets the secondary domain topic ID. */
    int getSecondary() const;

    /** @brief Sets the reviewer ID. */
    void setId(int id);
    /** @brief Sets the reviewer's name. */
    void setName(string name);
    /** @brief Sets the reviewer's email. */
    void setEmail(string email);
    /** @brief Sets the primary domain topic ID. */
    void setPrimary(int primary);
    /** @brief Sets the secondary domain topic ID. */
    void setSecondary(int secondary);
};

#endif //DA_PROJECT_1_REVIEWER_H