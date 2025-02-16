#ifndef puzzle_n_minus_1_cpp
#define puzzle_n_minus_1_cpp
#include "puzzle_n_minus_1.hpp"

#define DEBUGGING_MODE_PUZZLE

//Puzzle::Puzzle(){};

//default constuctor
Puzzle::Puzzle(unsigned char rows, unsigned char columns)
    :board(rows, columns)
{

    unsigned char tile_fill{0};
    
    for(unsigned char i = 0; i < rows; i++)
        for(unsigned char j = 0; j < columns; j++)
            board.setValueAt(i, j, tile_fill++);

    position_of_empty = std::make_tuple(0, 0);

    //std::cout << "Puzzle created!\n";
    
}

Puzzle::Puzzle(const std::vector<unsigned char>& tiles, int rows, int columns)
    :board(rows,columns)
{
    size_t aux = 0;
    for (int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(tiles[aux] == 0)
                position_of_empty = std::make_tuple(i,j);
            board.setValueAt(i, j , tiles[aux++]);
        }
    }
}

//copy constructor
Puzzle::Puzzle(const Puzzle& other)
    :board(other.board), position_of_empty(other.position_of_empty)
{
    //std::cout << "Puzzle copied!\n";
}

//copy assignment constructor
Puzzle& Puzzle::operator=(const Puzzle& other){

    //std::cout << "in puzzle copy assignement operator\n";
    if(this != &other){
        board = other.board;
        position_of_empty = other.position_of_empty;
    }

    //std::cout << "Puzzle copied!" << std::endl;
    return *this;
}

//move constructor
Puzzle::Puzzle(Puzzle&& other) noexcept
    :board(std::move(other.board)), position_of_empty(std::move(other.position_of_empty))
{
   // std::cout << "Puzzle moved!\n";
}

//move assignment operator
Puzzle& Puzzle::operator=(Puzzle&& other) noexcept{
    if(this != &other){
        board = std::move(other.board);
        position_of_empty = std::move(other.position_of_empty);
    }

    //std::cout << "Puzzle moved!\n";
    return *this;
}

Puzzle::~Puzzle(){}

std::set<moves> Puzzle::availableMoves() const{
    std::set<moves> available_moves;
    
    //if empty is at top row
    if(std::get<0>(position_of_empty) == 0){

        //if empty the left-top corner
        if(std::get<1>(position_of_empty) == 0){
            available_moves.insert(right);
            available_moves.insert(down);

            return available_moves;
        }

        //if empty is in the right-top corner
        if(std::get<1>(position_of_empty) == board.getColumns() - 1){
            available_moves.insert(left);
            available_moves.insert(down);

            return available_moves;
        }

        //empty is in the top, but not in a corner
        available_moves.insert(left);
        available_moves.insert(down);
        available_moves.insert(right);

        return available_moves;
    }

    //if empty is at bottom row
    if(std::get<0>(position_of_empty) == board.getRows() - 1){
        
        //if empty is at bottom-left corner
        if(std::get<1>(position_of_empty) == 0){
            available_moves.insert(right);
            available_moves.insert(up);

            return available_moves;
        }

        //if empty is at bottom-right corner
        if(std::get<1>(position_of_empty) == board.getColumns() - 1){
            available_moves.insert(left);
            available_moves.insert(up);

            return available_moves;
        }

        //empty is at bottom row, but not in a corner
        available_moves.insert(left);
        available_moves.insert(up);
        available_moves.insert(right);

        return available_moves;
    }

    //corner cases already checked
    //if empty is at left column
    if(std::get<1>(position_of_empty) == 0){

        available_moves.insert(up);
        available_moves.insert(right);
        available_moves.insert(down);

        return available_moves;
    }

    //if empty is at right column 
    if(std::get<1>(position_of_empty) == board.getColumns() - 1){

        available_moves.insert(up);
        available_moves.insert(left);
        available_moves.insert(down);

        return available_moves;
    }

    //not a edge_case at this point
    //could use for loop, but it would be overkill
    available_moves.insert(up);
    available_moves.insert(left);
    available_moves.insert(right);
    available_moves.insert(down);

    return available_moves;
}

//--------------------------------------------------------------
//at the point of calling these, nothing will go wrong
void Puzzle::moveUp(){
    auto to_move = board.getValueAt(std::get<0>(position_of_empty) - 1, std::get<1>(position_of_empty));
    board.setValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty), to_move);
    board.setValueAt(std::get<0>(position_of_empty) - 1, std::get<1>(position_of_empty), 0);

    //update postion_of_empty
    position_of_empty = std::make_tuple(std::get<0>(position_of_empty) - 1, std::get<1>(position_of_empty));
}

void Puzzle::moveLeft(){
    auto to_move = board.getValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty) - 1);
    board.setValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty), to_move);
    board.setValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty) - 1, 0);

    //update postion_of_empty
    position_of_empty = std::make_tuple(std::get<0>(position_of_empty), std::get<1>(position_of_empty) - 1);
}

void Puzzle::moveRight(){
    auto to_move = board.getValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty) + 1);
    board.setValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty), to_move);
    board.setValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty) + 1, 0);

    //update postion_of_empty
    position_of_empty = std::make_tuple(std::get<0>(position_of_empty), std::get<1>(position_of_empty) + 1);
}
void Puzzle::moveDown(){
    auto to_move = board.getValueAt(std::get<0>(position_of_empty) + 1, std::get<1>(position_of_empty));
    board.setValueAt(std::get<0>(position_of_empty), std::get<1>(position_of_empty), to_move);
    board.setValueAt(std::get<0>(position_of_empty) + 1, std::get<1>(position_of_empty), 0);

    //update postion_of_empty
    position_of_empty = std::make_tuple(std::get<0>(position_of_empty) + 1, std::get<1>(position_of_empty));
}
//---------------------------------------------------------------

bool Puzzle::makeMove(moves move){

    std::set<moves> available_moves = availableMoves();

    if(available_moves.find(move) != available_moves.end()){
        switch (move){
        case up:
            moveUp();
            break;
        
        case left:
            moveLeft();
            break;

        case right:
            moveRight();
            break;

        case down:
            moveDown();
            break;
        }

        return true;
    }else{
        //the move is not possible
        return false;
    }

}

unsigned char Puzzle::getTileAt(unsigned char row, unsigned char column) const{
    return board.getValueAt(row, column);
}

void Puzzle::printBoard() const{
    board.printMatrix();
}

const Matrix<unsigned char>& Puzzle::getBoard() const {
    return board;
}

bool Puzzle::operator== (const Puzzle& other) const{
    return (this->board == other.board);
}

bool Puzzle::isSolved() const{
    Puzzle p(board.getRows(), board.getColumns());
    return p == *this;
}

std::vector<unsigned char> Puzzle::getPuzzleAsString () const{
    auto buffer = board.getRawPointer();
    std::vector <unsigned char> string(buffer[0], buffer[0] + board.getRows() * board.getColumns()); 
    return string;
}

std::vector<char> Puzzle::getPuzzleAsSignedString () const{
    char** buffer = (char**)board.getRawPointer();
    std::vector <char> string(buffer[0], buffer[0] + board.getRows() * board.getColumns()); 
    return string;
}

std::tuple<unsigned char, unsigned char> Puzzle::getPositionOfEmpty() const{
    return position_of_empty;
}

#endif