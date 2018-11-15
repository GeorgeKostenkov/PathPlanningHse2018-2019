#include <iostream>
#include <string>

using namespace std;

int main (int argc, char *argv[]) {
    argc = 1;
    string amount = argv[1];
    for (int i = 0; i < stoi(amount); ++i)
        cout << "Path Planning\n";
    return 0;
}
