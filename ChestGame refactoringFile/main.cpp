//
//  main.cpp
//  chestGame
//
//  Created by kevin kurashewich on 10/17/16.
//  Copyright © 2016 Kevin Kurashewich. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

int main() {
    
    enum Pieces { PAWN, CASTLE, HORSE, BISHOP, QUEEN, KING };
    
    struct BoardGame {
        
        int currentRow, currentColumn, newRow, newColumn;
        int playerOneKingRow = 0;
        int playerOneKingColumn = 4;
        int playerTwoKingRow = 7;
        int playerTwoKingColumn = 4;
        int pathLength;
        int enemyKingRow, enemyKingColumn;
        int kingRow, kingColumn;
        
        bool isGameGoing = true;
        bool isPlayerOnesTurn = true;
        bool isInCheck = false;
        string someString = "hello";
        
        string playersTurn, otherPlayerString;
        
        vector<vector<vector<char>>> pieces;
        int path[8][2];
        
        
        
        //////////////////Sets up a Default Board////////////////////////
        void newGame() {
            this->pieces = {
                { { '1', '-', 'C' }, { '1', '-', 'H' }, { '1', '-', 'B' }, { '1', '-', 'Q' }, { '1', '-', 'K' }, { '1', '-', 'B' },
                    { '1', '-', 'H' }, { '1', '-', 'C' } },
                { { '1', '-', 'P' }, { '1', '-', 'P' }, { '1', '-', 'P' }, { ' ', ' ', ' ' }, { '1', '-', 'P' }, { '1', '-', 'P' },
                    { '1', '-', 'P' }, { '1', '-', 'P' } },
                { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' },
                    { ' ', ' ', ' ' }, { ' ', ' ', ' ' } },
                { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { '2', '-', 'Q' }, { '1', '-', 'Q' }, { ' ', ' ', ' ' },
                    { ' ', ' ', ' ' }, { ' ', ' ', ' ' } },
                { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' },
                    { ' ', ' ', ' ' }, { ' ', ' ', ' ' } },
                { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' },
                    { ' ', ' ', ' ' }, { ' ', ' ', ' ' } },
                { { '2', '-', 'P' }, { '2', '-', 'P' }, { '2', '-', 'P' }, { '2', '-', 'P' }, { '2', '-', 'P' }, { '2', '-', 'P' },
                    { '2', '-', 'P' }, { '2', '-', 'P' } },
                { { '2', '-', 'C' }, { '2', '-', 'H' }, { '2', '-', 'B' }, { '2', '-', 'Q' }, { '2', '-', 'K' }, { '2', '-', 'B' },
                    { '2', '-', 'H' }, { '2', '-', 'C' } },
            };
        }
        
        ////////////////////////Creats a GameBoard around array and prints//////////////////////////
        void printBoard() {
            cout << endl << endl;
            cout << "       | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |" << endl;
            for (int x = 0; x < pieces.size(); x++) {
                cout << "    ___|___|___|___|___|___|___|___|___|" << endl;
                cout << "    " + to_string(x + 1) + "  ";
                for (int y = 0; y < pieces.size(); y++) {
                    cout << "|" << this->pieces[x][y][0] << this->pieces[x][y][1] << this->pieces[x][y][2] << "";
                }
                cout << "|" << endl;
            }
            cout << "    ___|___|___|___|___|___|___|___|___|" << endl;
        }
        
        ///////////////////////////Moves piece to new spot and replaces old spot//////////////////
        void makeMove() {
            this->pieces[this->newRow][this->newColumn] = this->pieces[this->currentRow][this->currentColumn];
            this->pieces[this->currentRow][this->currentColumn] = { ' ', ' ', ' ' };
        }
        
        //////////////////////create straight path between two points//////////////////////////////
        void createStraightPath(int startRow = -1, int startColumn = -1, int endRow = -1, int endColumn = -1) {
            
            startRow = (startRow == -1)? this->currentRow: startRow;
            startColumn = (startColumn == -1)? this->currentColumn: startColumn;
            endRow = (endRow == -1)? this->newRow: endRow;
            endColumn = (endColumn == -1)? this->newColumn: endColumn;
            
            int columnDifference = abs(startColumn - endColumn);
            int rowDifference = abs(startRow - endRow);
            if (startRow == endRow) {
                if (startColumn - endColumn > 0) {
                    //deal with making path for column moves-- left or right
                    for (int x = 0; x <= columnDifference; x++) {
                        this->path[x][1] = startColumn - x;//moving left on map
                        this->path[x][0] = startRow;
                        this->pathLength = columnDifference;
                    }
                }
                else {
                    for (int x = 0; x <= columnDifference; x++) {
                        this->path[x][1] = startColumn + x;//moving right on map
                        this->path[x][0] = startRow;
                        this->pathLength = columnDifference;
                    }
                }
            }
            else if (startColumn == endColumn) {
                //making path for row moves-- up or down
                if (this->currentRow - this->newRow > 0) {
                    for (int x = 0; x <= rowDifference; x++) {
                        this->path[x][0] = startRow - x; //moving up on map
                        this->path[x][1] = startColumn;
                        this->pathLength = rowDifference;
                    }
                }
                else {
                    for (int x = 0; x <= rowDifference; x++) {
                        this->path[x][0] = startRow + x;//moving down on map
                        this->path[x][1] = startColumn;
                        this->pathLength = rowDifference;
                    }
                }
            }
        }
        
        /////////////////create a Diagnal Path between two points////////////////////////////////
        void createDiagnalPath(int startRow = -1, int startColumn = -1, int endRow = -1, int endColumn =-1) {

            startRow = (startRow == -1)? this->currentRow: startRow;
            startColumn = (startColumn == -1)? this->currentColumn: startColumn;
            endRow = (endRow == -1)? this->newRow: endRow;
            endColumn = (endColumn == -1)? this->newColumn: endColumn;
            
            
            
            int columnDifference = abs(startColumn - endColumn);
            int rowDifference = abs(startRow - endRow);
            
            //making path for diagnals part A
            if (startRow - endRow > 0) {
                for (int x = 0; x <= rowDifference; x++) {
                    this->path[x][0] = startRow - x;// going up on map
                    this->pathLength = rowDifference;
                }
            }
            else {
                for (int x = 0; x <= rowDifference; x++) {
                    this->path[x][0] = startRow + x;//going down on map
                    this->pathLength = rowDifference;
                }
            }
            
            //makeing pathing for diagnols part B
            if (startColumn - endColumn > 0) {
                for (int x = 0; x <= columnDifference; x++) {
                    this->path[x][1] = startColumn - x;//going left on map
                    this->pathLength = columnDifference;
                }
            }
            else {
                for (int x = 0; x <= columnDifference; x++) {
                    this->path[x][1] = startColumn + x;//going right on map
                    this->pathLength = columnDifference;
                }
            }
        }
        
        /////////////////////check if path is clear////////////////////////////////
        bool noObjectsInPath(){
            
            //checks if path has objects in the way
            for (int x = 0; x < this->pathLength - 1; x++) {
                if (this->pieces[path[x + 1][0]][path[x + 1][1]][0] == '1' || this->pieces[path[x + 1][0]][path[x + 1][1]][0] == '2') {
                    return false;
                }
            }
            return true;
        }
        
        //////////////////sees if its enemys piece that is being attacked//////////////////////
        bool canAttack(bool newToKing = false) {
            
            char enemysPiece = (this->isPlayerOnesTurn) ? '2' : '1';
            if (!newToKing){
                return(this->pieces[this->newRow][this->newColumn][0] == enemysPiece) ? true : false;
            }
            else{
                return(this->pieces[this->enemyKingRow][this->enemyKingColumn][0] == enemysPiece) ? true : false;
            }
        }
        
        //////////////////////sees if moveing to a empty space///////////////////////
        bool emptySpace() {
            return(this->pieces[this->newRow][this->newColumn][0] == ' ') ? true : false;
        }
        
        /////////checks if piece can do Move requested///////////////////////
        bool checkMove(Pieces whatType, int startRow = -1, int startColumn = -1, int endRow = -1, int endColumn = -1){
            
            startRow = (startRow == -1)? this->currentRow: startRow;
            startColumn = (startColumn == -1)? this->currentColumn: startColumn;
            endRow = (endRow == -1)? this->newRow: endRow;
            endColumn = (endColumn == -1)? this->newColumn: endColumn;
            
            switch (whatType){
                case PAWN:
                    if (this->isPlayerOnesTurn){
                        if (emptySpace() && startRow == 1 && endRow - startRow <= 2 &&
                            endRow - startRow > 0 && endColumn == startColumn){
                            return true;
                        }
                        else if (emptySpace() && endRow - startRow == 1 && endColumn == startColumn){
                            return true;
                        }
                        else if (startRow + 1 == endRow && abs(startColumn - endColumn) == 1 && canAttack()){
                            return true;
                        }
                    }
                    else{
                        if (emptySpace() && !this->isPlayerOnesTurn && startRow == 6 && startRow - endRow <= 2 &&
                            startRow - endRow > 0 && endColumn == startColumn){
                            return true;
                        }
                        else if (emptySpace() && startRow - endRow == 1 && endColumn == startColumn){
                            return true;
                        }
                        else if (endRow + 1 == startRow && abs(startColumn - endColumn) == 1 && canAttack()){
                            return true;
                        }
                    }
                    break;
                    
                case CASTLE:
                        return (startRow == endRow || startColumn == endColumn);
                    break;
                    
                case HORSE:
                        return((abs(startRow - endRow) == 2 && abs(startColumn - endColumn) == 1) ||
                               (abs(startRow - endRow) == 1 && abs(startColumn - endColumn) == 2));
                    break;
                    
                case BISHOP:
                        return (abs(abs(startRow) - abs(endRow)) == abs(abs(startColumn) - abs(startColumn)));
                    break;
                    
                case QUEEN:
                    return((abs(startRow - endRow) == abs(startColumn - endColumn))
                            || (startRow == endRow || startColumn == endColumn));
                    break;
                    
                case KING:
                    return(abs(startRow - endRow) <= 1 && abs(startColumn - endColumn) <= 1);
                    break;
            }
            return false;
        }
        
        ////////////switches turn//////////////////////////
        void switchTurn(){
            this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
        }
        
        ///////// print player in check////////
        void playerInCheck(){
            cout << endl << "----" << this->otherPlayerString << "is in Check----" << endl;
            this->isInCheck = true;
        }
        
        /////////check your King to see if hes in check/////////
        bool isKingInCheck(){
            char enemyChar = (isPlayerOnesTurn)? '2': '1';
            
            
            kingRow = (!isPlayerOnesTurn) ? this->playerTwoKingRow : this->playerOneKingRow;
            kingColumn = (!isPlayerOnesTurn) ? this->playerTwoKingColumn : this->playerOneKingColumn;
            
            
            for(int x = 0; x < this->pieces.size(); x++){
                for(int y = 0; y < this->pieces[x].size(); y++){
                    if(this->pieces[x][y][0] == enemyChar){
                        switch(this->pieces[x][y][2]){
                                
                            case 'P':
                                this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                if(this->checkMove(PAWN, x, y, kingRow, kingColumn)){
                                    this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                    return true;
                                }
                                break;
                            case 'C':
                                this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                if(this->checkMove(CASTLE, x, y, kingRow, kingColumn)){
                                    
                                    this->createStraightPath(x ,y, kingRow, kingColumn);
                                    
                                    if(this->noObjectsInPath()){
                                        this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                        return true;
                                    }
                                }
                                break;
                            case 'H':
                                this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                if(this->checkMove(HORSE, x, y, kingRow, kingColumn)){
                                    this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                    return true;
                                }
                                break;
                            case 'B':
                                this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                if(this->checkMove(BISHOP, x, y, kingRow, kingColumn)){
                                    
                                    this->createDiagnalPath(x ,y, kingRow, kingColumn);
                                    
                                    if(this->noObjectsInPath()){
                                        this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                        return true;
                                    }
                                }
                                break;
                            case 'Q':
                                this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                if(this->checkMove(QUEEN, x, y, kingRow, kingColumn)){
                                    
                                    if(x == kingRow || y == kingColumn){
                                        createStraightPath(x ,y, kingRow, kingColumn);
                                    } else {
                                        createDiagnalPath(x, y, kingRow, kingColumn);
                                    }
                                    
                                    if(this->noObjectsInPath()){
                                        this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                        return true;
                                    }
                                }
                                break;
                            case 'K':
                                this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                if(this->checkMove(KING, x, y, kingRow, kingColumn)){
                                    this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                                    return true;
                                }
                                break;
                        }
                        this->isPlayerOnesTurn = !this->isPlayerOnesTurn;
                    }
                }
            }
            
            return false;
        }
        
    };/// end of GameBoard Struct
    
    
    BoardGame boardGame;
    
    //Create newGame
    boardGame.newGame();
    
    ////////// Game Loop////////////
    while (boardGame.isGameGoing) {
        
        boardGame.printBoard();
        boardGame.isInCheck = boardGame.isKingInCheck();
        
        if(boardGame.isInCheck){
            cout<<"Your in check"<<endl;
        }
        
        //provides string for whos turn
        boardGame.playersTurn = (boardGame.isPlayerOnesTurn) ? "PlayerOne: " : "PlayerTwo: ";
        
        //provides string for other
        boardGame.otherPlayerString = (boardGame.isPlayerOnesTurn) ? "PlayerTwo: " : "PlayerOne: ";
        
        // asks which piece they wish to move
        cout << boardGame.playersTurn << " where is the item you wish to move-> row 'space' column: ";
        cin >> boardGame.currentRow >> boardGame.currentColumn;
        boardGame.currentRow -= 1;//start at 0 for array
        boardGame.currentColumn -= 1;//start at 0 for array
        
        // checks if it is the users piece
        if ((boardGame.pieces[boardGame.currentRow][boardGame.currentColumn][0] == '1' &&
             boardGame.isPlayerOnesTurn) ||
            (boardGame.pieces[boardGame.currentRow][boardGame.currentColumn][0] == '2' &&
             !boardGame.isPlayerOnesTurn)) {
                
                //asks where the user would like to move
                cout << "where would you like to move-> row 'space' column: ";
                cin >> boardGame.newRow >> boardGame.newColumn;
                boardGame.newRow -= 1;//start at 0 for array
                boardGame.newColumn -= 1;//start at 0 for array
                
                //sees which peice is being moved
                switch (boardGame.pieces[boardGame.currentRow][boardGame.currentColumn][2]) {
                    case 'C':cout << "moving C" << endl;
                        //see if its a move a castle can do
                        if (boardGame.checkMove(CASTLE)) {
                            //makes sure your not targeting your own || its a open space
                            if (boardGame.canAttack() || boardGame.emptySpace()) {
                                //creates a path for move
                                boardGame.createStraightPath();
                                //checks if units are in the path
                                if (boardGame.noObjectsInPath()) {
                                    //moves unit
                                    boardGame.makeMove();//<----- need to add in checkCheck
                                    
                                    //switch Players Turn
                                    boardGame.switchTurn();
                                }
                            }
                        }
                        break;
                    case 'H':cout << "moving H" << endl;
                        //checks if its a move a horse can do
                        if (boardGame.checkMove(HORSE)) {
                            //makes sure your not targeting your own || its a open space
                            if (boardGame.canAttack() || boardGame.emptySpace()) {
                                //moves unit
                                boardGame.makeMove();//<----- need to add in checkCheck
                                
                                
                                //switch Players Turn
                                boardGame.switchTurn();
                            }
                        }
                        break;
                    case 'B':cout << "moving B" << endl;
                        //checks if its a move a bishop can do
                        if (boardGame.checkMove(BISHOP)) {
                            //makes sure your not targeting your own || its a open space
                            if (boardGame.canAttack() || boardGame.emptySpace()) {
                                //creates a path
                                boardGame.createDiagnalPath();
                                //checks if units are in the path
                                if (boardGame.noObjectsInPath()) {
                                    //moves unit and switchs players turn
                                    boardGame.makeMove();//<----- need to add in checkCheck
                                    
                                    //switch Players Turn
                                    boardGame.switchTurn();
                                }
                            }
                        }
                        break;
                    case 'Q':cout << "moving Q" << endl;
                        //checks if its a move a queen can do
                        if (boardGame.checkMove(QUEEN)) {
                            //makes sure your not targeting your own || its a open space
                            if (boardGame.canAttack() || boardGame.emptySpace()) {
                                //creates a path
                                (boardGame.checkMove(CASTLE)) ? boardGame.createStraightPath():boardGame.createDiagnalPath();
                                //checks if units are in the path
                                if (boardGame.noObjectsInPath()) {
                                    //moves unit
                                    boardGame.makeMove();//<----- need to add in checkCheck

                                }
                                //switch Players Turn
                                boardGame.switchTurn();
                            }
                        }
                        break;
                    case 'K':cout << "moving K" << endl;//<--------- we need to add in see if check
                        //checks if its a move a King can do
                        if (boardGame.checkMove(KING)) {
                            //makes sure your not targeting your own || its a open space
                            if (boardGame.canAttack() || boardGame.emptySpace()) {
                                //moves unit
                                boardGame.makeMove();
                                //save kings postion
                                if (boardGame.pieces[boardGame.newRow][boardGame.newColumn][0] == '1'){
                                    boardGame.playerOneKingRow = boardGame.newRow;
                                    boardGame.playerOneKingColumn = boardGame.newColumn;
                                }
                                else{
                                    boardGame.playerTwoKingRow = boardGame.newRow;
                                    boardGame.playerTwoKingColumn = boardGame.newColumn;
                                }
                                
                                //switch Players Turn
                                boardGame.switchTurn();
                            }
                        }
                        break;
                    case 'P':cout << "moving P" << endl;
                        //checks if its a move a pawnd can do on first move for player one
                        if (boardGame.checkMove(PAWN)) {
                            //creates a path
                            boardGame.createStraightPath();
                            //checks if units are in the path
                            if (boardGame.noObjectsInPath()) {
                                //moves unit
                                boardGame.makeMove();//<----- need to add in checkCheck
                                
                                //switch Players Turn
                                boardGame.switchTurn();
                            }
                        }
                        break;
                    default: cout << endl << "----Please Select one of your items to move----";
                }
            }
        else {
            cout << endl << "----Please pick your own piece to move----" << endl;
        }
    }
    
    return 0;
}
