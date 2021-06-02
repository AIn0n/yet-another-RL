#include <iostream>
#include <array>
#include <cassert>

#define BOARD_SIZE 9
#define MAX_XY 3
#define EMPTY_FIELD 0
#define PLAYER_X 1
#define PLAYER_O -1
class TicTacToe
{
public:
    TicTacToe(void) { clearBoard(); }

    void clearBoard(void){ board.fill(EMPTY_FIELD);}

    void showBoard(void)
    {
        for(int y = 0; y < MAX_XY; ++y)
        {
            for(int x = 0; x < MAX_XY; ++x)
            {
                std::cout << board[x + y * MAX_XY] << ' ';
            }
            std::cout << '\n';
        }
    }

    std::array<int, BOARD_SIZE> getBoard() {return board;}

    void setField(uint x, uint y, int player)
    {
        assert((player == PLAYER_X || player == PLAYER_O) && x < MAX_XY && y < MAX_XY);
        board[x + y * MAX_XY] = player;
    }

    int checkForWinner(void)
    {
        int winner = EMPTY_FIELD;
        for(int i = 0; i < MAX_XY; ++i)
        {
            winner += winnerCheckCol(i);
            winner += winnerCheckRow(i);
        }
        winner += winnerCheckFirstDiagonal();
        winner += winnerCheckSecondDiagonal();

        if(!winner) return EMPTY_FIELD;
        return (winner > 0) ? PLAYER_X : PLAYER_O;
    }

private:
    std::array<int, BOARD_SIZE> board;

    int winnerCheckFirstDiagonal(void) { 
        return(board[0] == board[4] && board[4] == board[8]) ? board[0] : EMPTY_FIELD;}

    int winnerCheckSecondDiagonal(void) { 
        return(board[2] == board[4] && board[4] == board[6]) ? board[2] : EMPTY_FIELD;}

    int winnerCheckCol(uint col)
    {
        return (
            board[col + 0 * MAX_XY] == board[col + 1 * MAX_XY] &&
            board[col + 1 * MAX_XY] == board[col + 2 * MAX_XY]
        ) 
        ? board[col + 0 * MAX_XY] : EMPTY_FIELD;
    }

    int winnerCheckRow(uint row)
    {
        return (
            board[0 + row * MAX_XY] == board[1 + row * MAX_XY] &&
            board[1 + row * MAX_XY] == board[2 + row * MAX_XY]
        )
        ? board[row + 0 * MAX_XY] : EMPTY_FIELD;
    }
};

int main(void)
{
//tictactoe constructor test
    TicTacToe ttt = TicTacToe();
    std::array<int, BOARD_SIZE> tab = ttt.getBoard();
    for(int i = 0; i < BOARD_SIZE; ++i)
        assert(tab[i] == EMPTY_FIELD);

//change field test
    ttt.setField(0, 0, PLAYER_X);
    ttt.setField(2, 2, PLAYER_O);

    tab = ttt.getBoard();

    assert(tab[0] == PLAYER_X);
    assert(tab[8] == PLAYER_O);

//test clearing
    ttt.clearBoard();
    tab = ttt.getBoard();
    for(int i = 0; i < BOARD_SIZE; ++i)
        assert(tab[i] == EMPTY_FIELD);

//check winner test
    ttt.setField(0, 0, PLAYER_X);   //1, 1, 1
    ttt.setField(1, 0, PLAYER_X);   //0, 0, 0
    ttt.setField(2, 0, PLAYER_X);   //0, 0, 0
    assert(ttt.checkForWinner() == PLAYER_X);

    ttt.setField(0, 0, PLAYER_O);   //1, 0, 0
    ttt.setField(1, 1, PLAYER_O);   //0, 1, 0
    ttt.setField(2, 2, PLAYER_O);   //0, 0, 1
    assert(ttt.checkForWinner() == PLAYER_O);

    return 0;
}