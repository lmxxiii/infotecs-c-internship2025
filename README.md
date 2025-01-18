# Journal writer application

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

##FEATURES

The application allowes the user to write messages with different urgency levels to different text-files (aka journals).

##INSTALLATION

1. Clone the repository:
```bash
   git clone https://github.com/lmxxiii/infotecs-c-internship2025.git
```
   
2. Navigate into the project directory
```bash
   git path-to-the-project
```
   
3. Build the project (if applicable)
```bash
   mkdir build & cd build
   make
```

##USAGE

To begin using the application, type one of the following commands to the commands line.

List of available commands
-h          #Provides user with a list of available commands
-init       #Initialize the journal
-write      #Write to the journal
-change     #Change minimum urgency level for incoming messages
-reset      #Deinitializes the journal
-exit       #Exit the application

Hint: -write and -change commands are softlocked by default. To enable them, you need to first initialize the journal with the -init command.
