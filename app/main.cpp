#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "../libs/journallib.h"

std::queue<std::string> dataQueue; // queue to hold input data
std::mutex mtx; // mutex for thread safety
std::condition_variable cv; // condition variable for synchronization
bool done = false; // flag to indicate whether input is done

// Handles input
void inputData() {
    std::string input;
    while (true) {
        std::cout << "Available commands (type 'exit' to quit): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            {
                std::lock_guard<std::mutex> lock(mtx);
                done = true; // Signal that input is done
            }
            cv.notify_one(); // Notify the writer thread
            break;
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            dataQueue.push(input); // Push data to the queue
        }
        cv.notify_one(); // Notify the writer thread
    }
}

// Handles output
void writeData() {
    std::ofstream outFile("output.txt");

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !dataQueue.empty() || done; }); // Wait for data or done flag

        while (!dataQueue.empty()) {
            outFile << dataQueue.front() << std::endl; // Write data to the file
            dataQueue.pop(); // remove the data from the queue
        }

        if (done && dataQueue.empty()) {
            break; // exit if input is done and queue is empty
        }
    }

    outFile.close();
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
    return JournalLib::Urgency::Undefined; //couldn't find matching urgency status for the input data
}

int main() {
    std::thread inputThread(inputData); // start input thread
    std::thread writeThread(writeData); // start writing thread

    inputThread.join();  // wait for input thread to finish
    writeThread.join();  // wait for writing thread to finish

    return 0;
}
