#ifndef DA_PROJECT_1_DEBUGGER_H
#define DA_PROJECT_1_DEBUGGER_H

#include "Brainer.h"

/**
 * @class Debugger
 * @brief Utility class to handle debugging outputs for the ConferenceManager
 */
class Debugger {
public:
    /**
     * @brief Prints the graph edges and their weights
     */
    static void debugGraph(const Brainer& manager);

    /**
     * @brief Prints the graph nodes and the flows between them
     */
    static void debugGraphFlow(const Brainer& manager);

    /**
     * @brief Prints the interpretation results including matches and risks
     */
    static void debugInterpretationResults(const Brainer& manager);
};

#endif //DA_PROJECT_1_DEBUGGER_H