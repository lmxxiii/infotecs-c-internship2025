/*
 * This header file describes the journal writer library interface.
 * The library is made to enable writing messages with different urgency levels to different journals.
 *
 *
 */

#ifndef JOURNALLIB_H
#define JOURNALLIB_H

#include <string>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <filesystem>

namespace JournalLib {

    enum class Urgency { //enumerates possible urgency statuses
        Undefined,
        Low,
        Medium,
        High
    };

    const std::unordered_map<Urgency, std::string> urgencyToStringMap = {
        {Urgency::Undefined, "UNDEFINED"},
        {Urgency::Low, "LOW"},
        {Urgency::Medium, "MEDIUM"},
        {Urgency::High, "HIGH"}
    };

    class JournalManager {
    public:
        // The JournalManager has two initialization parameters: name of the journal and minimum urgency for incoming messages.
        // Unless the parameters are defined, the JournalManager is constructed with default values
        JournalManager(std::string journalName = "", Urgency minUrgency = Urgency::Undefined);
        // Sets the name of the journal to write to, and minimum urgency for incoming messages
        void init(const std::string& journalName, Urgency minUrgency);
        // Write a message with a set urgency to the journal
        // Return true, if writing was successful
        bool write(const std::string& message, Urgency urgency);
        void changeUrgency(Urgency newUrgency); //change the minimum urgency for incoming messages for the current journal
        void reset(); //reset to default parameters' values
        bool getState(); //return true if the journal is initialized
    private:
        std::string journalName;
        Urgency minUrgency;
        std::string urgencyToString(Urgency urgency); //converts urgency object to string
    };
}

#endif // JOURNALLIB_H
