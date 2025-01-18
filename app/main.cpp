/*
 * The application allowes the user to write messages with different urgency levels to different text-files (aka journals).
 *
 */


#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "../libs/journallib.h"

std::queue<std::pair<std::string,JournalLib::Urgency>> dataQueue; // queue to hold input data
std::mutex mtx; // mutex for thread safety
std::condition_variable cv; // condition variable for synchronization
bool done = false; // flag to indicate whether input is done
JournalLib::JournalManager journal; // journal to interact with

// Handles input
void inputData() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);

        if (input == "-exit") {
            {
                std::lock_guard<std::mutex> lock(mtx);
                done = true; // signal that input is done
            }
            cv.notify_one(); // notify the writer thread
        }
        else if (input == "-h") {
            std::cout << "List of available commands:" << std::endl
            << "-h          #Provides user with a list of available commands" << std::endl
            << "-init       #Initialize the journal" << std::endl
            << "-write      #Write to the journal" << std::endl
            << "-change     #Change minimum urgency level for incoming messages" << std::endl
            << "-reset      #Deinitializes the journal" << std::endl
            << "-exit       #Exit the application" << std::endl;
        }
        else if (input == "-init") { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        }
        else if (input == "-write") { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            std::cout << "Message: ";
            std::cout << "Urgency (HIGH/MEDIUM/LOW): ";
            std::lock_guard<std::mutex> lock(mtx);
            //dataQueue.push(input); // push data to the queue
            cv.notify_one(); // notify the writer thread
        }
        else if (input == "-change") { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        }
        else if (input == "-reset") {
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
            journal.write(dataQueue.front().first,dataQueue.front().second); // write data to the file
            dataQueue.pop(); // remove the data from the queue
        }

        if (done && dataQueue.empty()) {
            break; // exit if input is done and queue is empty
        }
    }
}

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

int main() {
    std::cout << "Welcome to the journal writer application.\nHint: type -h to list available commands." << std::endl;
    std::thread inputThread(inputData); // start input thread
    std::thread writeThread(writeData); // start writing thread

    inputThread.join();  // wait for the input thread to finish
    writeThread.join();  // wait for the writing thread to finish

    return 0;
}
