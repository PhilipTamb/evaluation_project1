
// #include <stdio.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include "include/ini.hpp"
//  #include <stdint.h>
//  #include <string.h>

using namespace std;
string path;
string default_path = "/home/userver/evaluation_project1/example/example.ini";

int main(int argc, char *argv[])
{

    if (argc > 1)
    {
        path = argv[1];
        cout << "path: " << path << " " << endl;
    }
    else
    {
        path = default_path;
    }

    Ini *ini = new Ini();
    unsigned short res;
    res = ini->Ini::load_resurce(path);
    cout << "load_resurce return: " << res << endl;

    string buffer;
    res = ini->Ini::get_value("section.foo", buffer);
    cout << "get_value return: " << buffer << endl;

    string str1 = "section.color.red";
    string str2 = "roses are red and blue";
    res = ini->Ini::set_value(str1, str2);
    cout << "set_value return: " << res << endl;

    str1 = "section.foo.bar";
    str2 = "2048";
    res = ini->Ini::set_value(str1, str2);
    cout << "set_value return: " << res << endl;

    str1 = "section.foo.bar.baz";
    str2 = "20-KJ";
    cout << "before 3th call: " << endl;
    res = ini->Ini::set_value(str1, str2);
    cout << "set_value return 3th call: " << res << endl;
    // res =

    // load_resurce(path);

    // delete ini;

    // if(!path(path))
    // {

    //     throw domain_error("Error path is not defined.");
    // }

    return EXIT_SUCCESS;
}

// int main(int argc, char *argv[])
// {

//    std::cout << "hello" << std::endl;

//     return EXIT_SUCCESS;
// }