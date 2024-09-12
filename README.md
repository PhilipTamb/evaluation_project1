# Evaluation Project 1

## Description

The goal of this project is to assess your programming skills and your understanding of basic Linux system operations. You are required to develop an application consisting of the following components:

### 1. C++ Shared Library for INI File Parsing:
You will create a C++ shared library that can load and parse an INI configuration file into volatile memory from the Linux filesystem. The library must provide the following public interfaces:
* unsigned short load_resource(const std::string &path);
Loads the INI file from the specified path.
* unsigned short get_value(const std::string &key, std::string &value);
Retrieves the value associated with the given key.
* unsigned short set_value(const std::string &key, std::string &value);
Updates the value associated with the given key.
### 2. C++ Server Application:
You will develop a C++ server application that uses the shared library mentioned above and provides a basic API accessible via TCP at localhost:12345. The server should expose the following API endpoints:
* LOAD PATH – Loads the INI file located at the specified path.
* GET KEY – Retrieves the value associated with a given key.
* SET KEY VALUE – Sets or updates the value for a specified key.
### C++ Client Application with CLI:
You will also create a C++ client application with a command-line interface (CLI) that allows users to send requests to the server via the above-mentioned TCP API. The CLI should support the following commands:
* ./client --load /home/user/evaluation_project1/example/example.ini
Loads the specified INI file into the server.
* ./client --get section.foo.bar
Retrieves the value for the key section.foo.bar from the server.
* ./client --set section.color.red "roses are red"
Sets the value of section.color.red to "roses are red" on the server.
### Build Requirements:
The C++ applications must be built using a Makefile. While a basic Makefile is sufficient, using a more sophisticated one would be appreciated.


## How to solve this problem
First, I focused on how to load and manage an INI file in volatile memory within a Linux environment. I determined that the best approach was to use mmap, which allows the mapping of a file (or a memory region) to a range of virtual memory addresses. While implementing this, I had to consider several potential issues, such as: ensuring proper unmapping of the file pointer, avoiding exceeding the size of the allocated memory page, being cautious with memory offsets during write operations, and carefully managing the deallocation of volatile memory.

Once the library for handling volatile memory was completed, I turned my attention to creating the CMakeLists file to automate the compilation and linking process of the shared library. Afterward, I developed the server APIs using simple sockets and finally moved on to writing the client application.

## Result
It is hoped that the result will be appreciated, although it should be considered an intermediate work with some execution issues. The project remains incomplete due to time constraints.


## Getting Started

Download and execute the code.

```
git clone https://github.com/PhilipTamb/evaluation_project1.git
cd evaluation_project1
```


Build code
```
cmake -S . -B build
cmake --build build
```

Server bash
```
./build/server
```

Client bash
```
./build/client --load /home/userver/evaluation_project1/example/example.ini
./build/client --get section.foo.bar
./build/client --set section.color.red “roses are red”
```
