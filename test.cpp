#include <iostream>
#include "Go.hpp"
using namespace std;

int main(){
    Go g;
    cout << Go::sgfToStd("B[pd]") << endl;
    cout << Go::sgfToStd("W[dd]") << endl;
    cout << Go::sgfToStd("B[pp]") << endl;
    cout << Go::sgfToStd("W[dp];") << endl;
    cout << Go::sgfToStd("B[cq];W[dq]") << endl;
    cout << Go::sgfToStd("W[dq]") << endl;
    g.saveSgf(".");
    return 0;
}