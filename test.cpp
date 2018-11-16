#include <iostream>
#include <string>

using namespace std;

int main (int argc, char *argv[]) {
    argc = 1;
    string times = argv[1];
    for (int i = 0; i < stoi(times); ++i)
        cout << "Path Planning\n";
    return 0;
}
