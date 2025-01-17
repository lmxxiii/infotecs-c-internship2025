/*
 * This file contains implementations for "journallib.h" library functions
 *
 */

#include "journallib.h"


namespace JournalLib {

    void JournalManager::init(const std::string& journalName, Urgency minUrgency){
        this->journalName = journalName;
        this->minUrgency = minUrgency;
    }

    bool JournalManager::write(const std::string& message, Urgency urgency){
        if (urgency < minUrgency) // this message priority is too low, so don't write anything to the journal
            return false;

        const auto now = std::chrono::system_clock::now();
        const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
        std::filesystem::path currentPath = std::filesystem::current_path();
        try{
            std::filesystem::path journalPath = currentPath.parent_path() / "journals" / (journalName+".txt");
            std::filesystem::create_directories(journalPath.parent_path()); //create necessary directory and/or file, if it doesn't already exist
            std::ofstream journal(journalPath); //open journal for writing
            if (!journal)
                return false;
            journal << "Time: " << std::ctime(&t_c) << " Urgency: " << urgencyToString(urgency) << " Message: " << message << std::endl;
            journal.close();
        } catch (const std::exception& e){
            return false;
        }
        return true;
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

    std::string urgencyToString(Urgency urgency){
        auto it = urgencyToStringMap.find(urgency);
        if (it != urgencyToStringMap.end()){
            return it->second;
        } else {
            return urgencyToStringMap.begin()->second;
        }
    }
}
