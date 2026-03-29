#ifndef DA_PROJECT_1_SUBMISSION_H
#define DA_PROJECT_1_SUBMISSION_H
#include <string>
using namespace std;

/**
 * @class Submission
 * @brief Represents a paper or article submitted to the conference.
 */
class Submission {
private:
    int id;
    string title;
    string author;
    string email;
    int primary;
    int secondary;
public:

    /**
     * @brief Default constructor.
     */
    Submission() = default;

    /**
     * @brief Parameterized constructor for Submission.
     * @param id The ID of the submission.
     * @param title The title of the submission.
     * @param author The author's name.
     * @param email The author's email.
     * @param primary The primary domain topic ID.
     * @param secondary The secondary domain topic ID.
     */
    Submission(int id, string title, string author, string email, int primary, int secondary);

    /** @brief Gets the submission ID. */
    int getId() const;
    /** @brief Gets the submission title. */
    string getTitle() const;
    /** @brief Gets the author's name. */
    string getAuthor() const;
    /** @brief Gets the author's email. */
    string getEmail() const;
    /** @brief Gets the primary domain topic ID. */
    int getPrimary() const;
    /** @brief Gets the secondary domain topic ID. */
    int getSecondary() const;

    /** @brief Sets the submission ID. */
    void setId(int id);
    /** @brief Sets the submission title. */
    void setTitle(string title);
    /** @brief Sets the author's name. */
    void setAuthor(string author);
    /** @brief Sets the author's email. */
    void setEmail(string email);
    /** @brief Sets the primary domain topic ID. */
    void setPrimary(int primary);
    /** @brief Sets the secondary domain topic ID. */
    void setSecondary(int secondary);
};

#endif //DA_PROJECT_1_SUBMISSION_H