/*
 * The application allowes the user to write messages with different urgency levels to different text-files (aka journals).
 *
 */


#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "../libs/libjournal.h"

std::queue<std::pair<std::string,JournalLib::Urgency>> dataQueue; // queue to hold input data
std::mutex mtx; // mutex for thread safety
std::condition_variable cv; // condition variable for synchronization
bool done = false; // flag to indicate whether input is done
JournalLib::JournalManager journal; // journal to interact with

//Converts string to urgency object
JournalLib::Urgency stringToUrgency(const std::string str){
    if (str == "") //urgency is undefined
        return JournalLib::Urgency::Undefined;

    for (auto it = JournalLib::urgencyToStringMap.begin(); it != JournalLib::urgencyToStringMap.end(); ++it)
    {
        if (it->second == str)
            return it->first;
    }
    return JournalLib::Urgency::Undefined; //couldn't find matching urgency level for the input string
}

//print available urgency levels
void urgencyHelper(){

    std::cout << "Available urgency levels:\n";

    for (auto it = JournalLib::urgencyToStringMap.begin(); it != JournalLib::urgencyToStringMap.end(); ++it)
    {
        std::cout << it->second << std::endl;
    }
    try {
        std::string defaultUrgency = JournalLib::urgencyToStringMap.begin()->second;
        std::cout << "\nThe urgency levels that don't match the strings above, will be treated as " << defaultUrgency << "\n"
        << "This is the lowest urgency level available for this program" << std::endl;
    }
    catch (std::exception e){
        std::cout << "No urgencies are available" << std::endl;
    }

}

// Handles input
void inputData() {
    std::string inputCommand;
    while (true) {
        std::getline(std::cin, inputCommand);

        if (inputCommand == "-exit") {
            std::lock_guard<std::mutex> lock(mtx);
            done = true; // signal that input is done
            cv.notify_one(); // notify the writer thread
            break;
        }
        else if (inputCommand == "-h") {
            std::cout << "List of available commands:" << "\n"
            << "-h          #Provides user with a list of available commands" << "\n"
            << "-init       #Initialize the journal" << "\n"
            << "-write      #Write to the journal" << "\n"
            << "-change     #Change minimum urgency level for incoming messages" << "\n"
            << "-reset      #Deinitializes the journal" << "\n"
            << "-exit       #Exit the application" << std::endl;
        }
        else if (inputCommand == "-init") {
            std::string name, urgency;
            std::cout << "Journal name: "; std::getline(std::cin, name);
            std::cout << "Set minimum urgency for incoming messages (print * for help): "; std::getline(std::cin, urgency);
            if (urgency == "*")
            {
                urgencyHelper();
                std::cout << "Awaiting input: "; std::getline(std::cin, urgency);
            }
            journal.init(name,stringToUrgency(urgency));
            if (journal.getState())
                std::cout << "Success" << std::endl;
            else
                std::cout << "Error" << std::endl;


        }
        else if (inputCommand == "-write") {
            if (journal.getState() == 0) {
                std::cout << "The journal is not initialized" << std::endl;
                continue;
            }
            std::string message, urgency;
            std::cout << "Message: "; std::getline(std::cin, message);
            std::cout << "Urgency (print * for help): "; std::getline(std::cin, urgency);
            if (urgency == "*")
            {
                urgencyHelper();
                std::cout << "Awaiting input: "; std::getline(std::cin, urgency);
            }
            std::lock_guard<std::mutex> lock(mtx);
            dataQueue.push({message,stringToUrgency(urgency)});
            cv.notify_one(); // notify the writer thread
        }
        else if (inputCommand == "-change") {
            if (journal.getState() == 0) {
                std::cout << "The journal is not initialized" << std::endl;
                continue;
            }
            std::string urgency;
            std::cout << "New urgency (print * for help): "; std::getline(std::cin, urgency);
            if (urgency == "*")
            {
                urgencyHelper();
                std::cout << "Awaiting input: "; std::getline(std::cin, urgency);
            }
            journal.changeUrgency(stringToUrgency(urgency));
        }
        else if (inputCommand == "-reset") {
            journal.reset();
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
}

// Handles output
void writeData() {

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !dataQueue.empty() || done; }); // wait for data or done flag

        while (!dataQueue.empty()) {
            journal.write(dataQueue.front().first,dataQueue.front().second);
            dataQueue.pop();
        }

        if (done && dataQueue.empty()) {
            break; // exit if input is done and the queue is empty
        }
    }
}

int main() {
    std::cout << "Welcome to the journal writer application.\nHint: type -h to list available commands." << std::endl;
    std::thread inputThread(inputData); // start input thread
    std::thread writeThread(writeData); // start writing thread

    inputThread.join();  // wait for the input thread to finish
    writeThread.join();  // wait for the writing thread to finish

    return 0;
}
