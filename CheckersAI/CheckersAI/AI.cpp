#include "AI.hpp"

const int player = 0;
const int computer = 1;
const int d = 3;


//makes the best move for the computer
Move makeMove(Board board) {

    //gets the potential moves
    vector<Move> moves = getMoves(board, computer);

    //builds the list of values for those moves
    int values[(int)moves.size()];
    for (int i = 0; i < (int)moves.size(); i++) {
        Board compMove = board.copyBoard();
        compMove.movePiece(moves[i].oldX, moves[i].oldY, moves[i].newX, moves[i].newY);
        values[i] = getMoveRec(compMove, d, computer);
    }

    //finds the highest value move
    int highestVal = values[0];
    int index = 0;
    for (int i = 1; i < (int)moves.size(); i++) {
        if (values[i] > highestVal) {
            highestVal = values[i];
            index = i;
        }
        //used to give variety to indifferent moves
        else if (values[i] == highestVal) {
            long r = rand() % 2;
            if (r == 0) {
                index = i;
            }
        }
    }

    //makes that move
    return moves[index];

}


//helper function for getMove
//gets the recursive value of a move, assuming the best move is made each time
int getMoveRec(Board board, int depth, int side) {
    //base case
    if (depth == 0) {
        return scoreBoard(board, side);
    }
    else {
        //first, we copy board, so we don't affect the original board
        Board newBoard = board.copyBoard();

        //gets the player's best immidiate move
        vector<Move> playerMoves = getMoves(newBoard, player);
        
        //builds list of values for player's moves
        int values[(int)playerMoves.size()];
        for (int i = 0; i < (int)playerMoves.size(); i++) {
            Board playerMove = board.copyBoard();
            playerMove.movePiece(playerMoves[i].oldX, playerMoves[i].oldY, playerMoves[i].newX, playerMoves[i].newY);
            values[i] = getMoveRec(playerMove, 0, player);
        }

        //finds the highest value move
        int highestVal = values[0];
        int index = 0;
        for (int i = 1; i < (int)playerMoves.size(); i++) {
            if (values[i] > highestVal) {
                highestVal = values[i];
                index = i;
            }
            //used to give variety to indifferent moves
            else if (values[i] == highestVal) {
                long r = rand() % 2;
                if (r == 0) {
                    index = i;
                }
        }
        }
        
        //updates newBoard with player's move
        newBoard.movePiece(playerMoves[index].oldX, playerMoves[index].oldY, playerMoves[index].newX, playerMoves[index].newY);

        //gets the computers potential moves
        vector<Move> moves = getMoves(newBoard, computer);

        //returns the greatest valued move
        int value = INT_MIN;
        for (int i = 0; i < (int)moves.size(); i++) {
            Board compMove = newBoard.copyBoard();
            compMove.movePiece(moves[i].oldX, moves[i].oldY, moves[i].newX, moves[i].newY);
            int newVal = getMoveRec(compMove, depth - 1, side);
            if (value < newVal) {
                value = newVal;
            }
        }

        return scoreBoard(board, side) + value;
    }
}


//gets the potential moves
vector<Move> getMoves(Board board, int side) {

    //stores moves
    vector<Move> valid;

    //gets which side to check for
    int piece;
    int king;
    if (side == player) {
        piece = WHITE;
        king = WHITE_KING;
    }
    else {
        piece = BLACK;
        king = BLACK_KING;
    }

    //finds valid moves for each piece
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            int p = board.getPiece(i, j);

            //if the piece matches the side we are examiniing
            if (p == piece || p == king) {
                int count = 0;
                Move potentialMoves[8];

                //initializes potential moves
                //initializes one square away
                for (int k = -1; k < 2; k += 2) {
                    for (int l = -1; l < 2; l += 2) {
                        Move move;
                        move.oldX = i;
                        move.oldY = j;
                        move.newX = i + k;
                        move.newY = j + l;
                        potentialMoves[count] = move;

                        count++;
                    }
                }

                //initializes jumps
                for (int k = -2; k < 3; k += 4) {
                    for (int l = -2; l < 3; l += 4) {
                        Move move;
                        move.oldX = i;
                        move.oldY = j;
                        move.newX = i + k;
                        move.newY = j + l;
                        potentialMoves[count] = move;

                        count++;
                    }
                }

                //determines valid moves and adds them to valid
                for (int i = 0; i < 8; i++) {
                    if (board.validMove(potentialMoves[i].oldX, potentialMoves[i].oldY, potentialMoves[i].newX, potentialMoves[i].newY)) {
                        valid.push_back(potentialMoves[i]);
                    }
                }


            }
        }
    }

    return valid;
}

//scores a given board
int scoreBoard(Board board, int side) {

    //holds pieces and opponenet pieces
    int ally;
    int allyKing;
    int opp;
    int oppKing;
    if (side == player) {
        ally = WHITE;
        allyKing = WHITE_KING;
        opp = BLACK;
        oppKing = BLACK_KING;
    }
    else {
        opp = WHITE;
        oppKing = WHITE_KING;
        ally = BLACK;
        allyKing = BLACK_KING;
    }

    //stores the score of a board
    int score = 0;

    //counts the number of black and white pieces
    int black = 0;
    int white = 0;
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            int piece = board.getPiece(i, j);
            if (piece == WHITE || piece == WHITE_KING) {
                white++;
            }
            else if (piece != EMPTY) {
                black++;
            }
        }
    }

    //updates score to favor taking pieces
    score = score + (black - white) * 2;


    //counts the number of black and white kings
    black = 0;
    white = 0;
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            int piece = board.getPiece(i, j);
            if (piece == WHITE_KING) {
                white++;
            }
            else if (piece == BLACK_KING) {
                black++;
            }
        }
    }

    //double counts kings to favor getting / taking kings
    score = score + (black - white) * 3;

    //inverts for player
    if (side == player) {
        score = -score;
    }

    //favors keeping pieces together
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            int piece = board.getPiece(i, j);
            if (piece == ally || piece == allyKing) {
                score += together(board, i, j, side);
            }
        }
    }

    return score;
}

//helper function for scoreBoard
//takes in a piece, and determines if there is space behind it
int together(Board board, int x, int y, int side) {
    //handles bottom and top cases
    if (y == 0 && side == player) {
        return 1;
    }
    else if (y == 7 && side == computer) {
        return 1;
    }

    //handles cases along walls
    bool lWall = false;
    bool rWall = false;
    if (x == 0) {
        lWall = true;
    }
    else if (x == 7) {
        rWall = true;
    }

    //evaluates if friendly piece is behind it
    int behind = -1;
    //handles computer side
    if (side == computer) {
        //evaluates right side
        if (!rWall) {
            if (board.getPiece(x + 1, y + 1) == BLACK || board.getPiece(x + 1, y + 1) == BLACK_KING) {
                behind += 2;
            }
            //negative if a king is behind you
            else if (board.getPiece(x + 1, y + 1) == WHITE_KING) {
                behind -= 1;
            }
        }
        //incentivize staying against wall
        else {
            behind += 1;
        }
        //left side
        if (!lWall) {
            if (board.getPiece(x - 1, y + 1) == BLACK || board.getPiece(x + 1, y + 1) == BLACK_KING) {
                behind += 2;
            }
            //negative if a king is behind you
            else if (board.getPiece(x - 1, y + 1) == WHITE_KING) {
                behind -= 1;
            }
        }
        //incentivize staying against wall
        else {
            behind += 1;
        }
    }
    //handles player side
    if (side == player) {
        //evaluates right side
        if (!rWall) {
            if (board.getPiece(x + 1, y - 1) == BLACK || board.getPiece(x + 1, y - 1) == BLACK_KING) {
                behind += 2;
            }
            //negative if a king is behind you
            else if (board.getPiece(x + 1, y - 1) == WHITE_KING) {
                behind -= 1;
            }
        }
        //incentivize staying against wall
        else {
            behind += 1;
        }
        //left side
        if (!lWall) {
            if (board.getPiece(x - 1, y - 1) == BLACK || board.getPiece(x + 1, y - 1) == BLACK_KING) {
                behind += 2;
            }
            //negative if a king is behind you
            else if (board.getPiece(x - 1, y - 1) == WHITE_KING) {
                behind -= 1;
            }
        }
        //incentivize staying against wall
        else {
            behind += 1;
        }
    }

    //return result
    return behind;
}




