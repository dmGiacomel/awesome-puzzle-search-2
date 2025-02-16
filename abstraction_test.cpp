#include "PDBAbstractedPuzzle.hpp"
#include <iostream>

void testsAbstraction(){
    PDBAbstraction abs({2,1,0,3}, 
                       {0,9,12,14},
                       4, 4);

    abs.printAbstraction();

    std::cout << std::endl;

    abs.makeMove(up);
    abs.makeMove(right);
    abs.makeMove(down);

    abs.printAbstraction();

    std::cout << std::endl;

    abs.makeMove(up);
    abs.makeMove(up);
    abs.makeMove(up);
    abs.makeMove(left);

    abs.printAbstraction();
    std::cout << std::endl;

    abs.makeMove(right);
    abs.printAbstraction();


    PDBAbstraction abs_2({5,4,0,1,2,3}, 
                       {3,6,8,9,11,12},
                       4, 4);

    abs_2.printAbstraction();

    auto temp = abs;
    
    temp = abs_2;   
    temp.printAbstraction();    

    temp = std::move(abs);
    temp.printAbstraction();    
}



int main(){

    //not necessarily valid state
    Puzzle p ({6,7,0,15,13,9,1,2,3,5,4,11,8,10,12,14}, 4, 4);
    PDBAbstraction abs(p, {11, 12, 13});
    
    abs.printAbstraction();
    return 0;
}