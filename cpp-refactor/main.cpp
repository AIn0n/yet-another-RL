#include <iostream>
#include <array>
#include <cassert>

#define BOARD_SIZE 9
#define MAX_XY 3
#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O -1

enum Field : int8_t {
    o       = -1,
    empty   = 0,
    x       = 1
};

char
fieldToChar(Field f)
{
    switch (f) {
    case Field::o:
        return 'o';
    case Field::x:
        return 'x';
    }
    return '_';
}

class TicTacToe
{
public:
    TicTacToe(void) 
    { 
        clearBoard(); 
    }

    inline void 
    clearBoard(void)
    { 
        board.fill(Field::empty);
    }

    void 
    showBoard(void)
    {
        for (int y = 0; y < MAX_XY; ++y) {
            for (int x = 0; x < MAX_XY; ++x)
                std:: cout << fieldToChar(board[x + y * MAX_XY]) << ' ';
            std::cout << '\n';
        }
    }

    inline std::array<Field, BOARD_SIZE> 
    getBoard() 
    {
        return board;
    }

    inline void 
    setField(uint x, uint y, Field player)
    {
        assert(x < MAX_XY && y < MAX_XY);
        board[x + y * MAX_XY] = player;
    }

    Field 
    checkForWinner(void)
    {
        int winner = EMPTY;
        for (int i = 0; i < MAX_XY; ++i) {
            winner += winnerCheckCol(i);
            winner += winnerCheckRow(i);
        }
        winner += winnerCheckFirstDiagonal();
        winner += winnerCheckSecondDiagonal();

        if (!winner) 
            return Field::empty;
        return (winner > 0) ? Field::x : Field::o;
    }

    bool
    isGameEnded(void)
    {
        int emptyFields = 0;
        for (const auto &field : board)
            emptyFields += (field != Field::empty);
        return emptyFields == BOARD_SIZE || checkForWinner() != Field::empty;
    }

private:
    std::array<Field, BOARD_SIZE> board;

    inline Field
    winnerCheckFirstDiagonal(void) 
    { 
        return board[0]==board[4] && board[4]==board[8] ? board[0]:Field::empty;
    }

    inline Field
    winnerCheckSecondDiagonal(void) 
    { 
        return board[2]==board[4] && board[4]==board[6] ? board[2]:Field::empty;
    }

    inline Field
    winnerCheckCol(uint col)
    {
        return (
            board[col + 0 * MAX_XY] == board[col + 1 * MAX_XY] &&
            board[col + 1 * MAX_XY] == board[col + 2 * MAX_XY])
        ? board[col + 0 * MAX_XY] : Field::empty;
    }

    inline Field
    winnerCheckRow(uint row)
    {
        return (
            board[0 + row * MAX_XY] == board[1 + row * MAX_XY] &&
            board[1 + row * MAX_XY] == board[2 + row * MAX_XY])
        ? board[0 + row * MAX_XY] : Field::empty;
    }
};

int main(void)
{
//tictactoe constructor test
    TicTacToe ttt = TicTacToe();
    std::array<Field, BOARD_SIZE> tab = ttt.getBoard();
    for (const auto &field : tab)
        assert(field == Field::empty);

//change field test
    ttt.setField(0, 0, Field::x);
    ttt.setField(2, 2, Field::o);

    tab = ttt.getBoard();

    assert(tab[0] == Field::x);
    assert(tab[8] == Field::o);

//test clearing
    ttt.clearBoard();
    tab = ttt.getBoard();
    for (int i = 0; i < BOARD_SIZE; ++i)
        assert(tab[i] == Field::empty);

//check winner test (row)
    for(int i = 0; i < 3; ++i)
        ttt.setField(i, 0, Field::x);   
    assert(ttt.checkForWinner() == Field::x);

//check winner test (diagonal)
    for(int i = 0; i < 3; ++i)
        ttt.setField(i, i, Field::o);
    assert(ttt.checkForWinner() == Field::o);

//check isGameEnded function
    assert(ttt.isGameEnded() == true);
    ttt.setField(1, 1, Field::empty);
    assert(ttt.isGameEnded() == false);
    return 0;
}