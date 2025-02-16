#include "PDB.hpp"

//to compile this test: g++ -Ofast test_pdb.cpp ../PDBAbstractedPuzzle.cpp ../puzzle_n_minus_1.cpp ../Utils/IndexFunctions.cpp ../Utils/MovementsHandler.cpp Heuristics.cpp PDB.cpp -o test_pdb.x

int main(){
    PDB h;

    h.build(Puzzle(4,4), Puzzle(4,4), {9, 10, 11, 12, 13, 14, 15});

    // h.build(Puzzle(4,4), Puzzle(4,4), {9, 10, 11, 12, 13, 14, 15});

    std::cout << "slots not reached: " << h.verify() << std::endl;
    
}