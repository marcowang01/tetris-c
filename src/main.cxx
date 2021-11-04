#include "controller.hxx"

#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])

try {


    Controller().run();
}


catch (exception const& e) {
    cerr << argv[0] << ": " << e.what() << "\n";
    return 1;
}
