#include <iostream>
#include "Go.hpp"
using namespace std;

int main(){
    Go g;
    /*cout << Go::sgfToStd("B[pd]") << endl;
    cout << Go::sgfToStd("W[dd]") << endl;
    cout << Go::sgfToStd("B[pp]") << endl;
    cout << Go::sgfToStd("W[dp];") << endl;
    cout << Go::sgfToStd("B[cq];W[dq]") << endl;
    cout << Go::sgfToStd("W[dq]") << endl;
    g.play("Q16");
    cout << g.toString() << endl;
    g.play("D16");
    g.play("Q4");
    g.play("D4");
    g.play("C3");
    g.play("D3");
    cout << g.toString() << endl;
    g.saveSgf();*/
    cout << g.toString() << endl;
    g.saveSgf();
    return 0;
}