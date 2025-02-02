/*
 * This file contains implementations for "libjournal.h" library functions
 *
 */

#include "libjournal.h"


namespace JournalLib {

    std::string JournalManager::urgencyToString(Urgency urgency){
        auto it = urgencyToStringMap.find(urgency);
        if (it != urgencyToStringMap.end()){
            return it->second;
        } else {
            return urgencyToStringMap.begin()->second;
        }
    }

    JournalManager::JournalManager(std::string journalName, Urgency minUrgency){
        this->journalName = journalName;
        this->minUrgency = minUrgency;
    }

    void JournalManager::init(const std::string& journalName, Urgency minUrgency){
        this->journalName = journalName;
        this->minUrgency = minUrgency;
    }

    bool JournalManager::write(const std::string& message, Urgency urgency){
        if (urgency < minUrgency) // the message priority is too low, so don't write anything to the journal
            return false;
        if (message == "") //no message to write down
            return false;

        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        std::filesystem::path currentPath = std::filesystem::current_path();
        try{
            std::filesystem::path journalPath = currentPath.parent_path() / "journals" / (journalName+".txt");
            std::filesystem::create_directories(journalPath.parent_path()); //create necessary directory and/or file, if it doesn't already exist
            std::ofstream journal(journalPath, std::ios::app); //open journal for writing
            if (!journal)
                return false;
            journal << "Time: " << std::put_time(std::localtime(&t_c), "%d-%m-%Y %H:%M:%S") << " Urgency: " << urgencyToString(urgency) << " Message: " << message << std::endl;
            journal.close();
        } catch (const std::exception& e){
            return false;
        }
        return true; // successful, if true. An error occured, if false
    }

    void JournalManager::changeUrgency(Urgency newUrgency){
        this->minUrgency = newUrgency;
    }

    void JournalManager::reset(){
        this->journalName = "";
        this->minUrgency = Urgency::Undefined;
    }

    bool JournalManager::getState(){
        return this->journalName != "" ? true : false;
    }
}
