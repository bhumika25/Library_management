# Library Management System

This project is a library management system that allows users to manage books, users, and transactions. It also includes server and client functionalities.

## Prerequisites

- C++ Compiler 


## How to Compile

1. Open a terminal or command prompt.
2. Navigate to the project directory.
3. Compile the project using the following commands:


g++ -o server.exe Server.cpp Library.cpp Book.cpp User.cpp Transaction.cpp -lws2_32
g++ client.cpp -o client -lws2_32
g++ -o main.exe main.cpp Library.cpp Book.cpp User.cpp Transaction.cpp -lws2_32


## To Run the File

1. To run console application
    Run main.exe file  as ./main.exe in terminal and enter 1 for console application

2. To run as client-server application
    Run main.exe file as ./main.exe in terminal and enter 2 to run the server. 
    once server is running, you can connect with client by running
    ./main.exe again in another terminal and entering 3 to run the client.

    