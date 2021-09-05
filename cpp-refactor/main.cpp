#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>

#define BOARD_SIZE 9
#define MAX_XY 3

enum
Field : int8_t {
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

template<typename Container, typename T>
struct RandomChoice
{
    T
    choice(Container n)
    {
        std::uniform_int_distribution<uint> distr(0.0, n.size() - 1);
        return n[distr(rng)];
    }

    RandomChoice(std::default_random_engine engine) : rng(engine) {}

private:
    std::default_random_engine rng;
};

struct TicTacToe
{
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
                std::cout << fieldToChar(board[x + y * MAX_XY]) << ' ';
            std::cout << '\n';
        }
    }

    inline std::array<Field, BOARD_SIZE> 
    getBoard() 
    {
        return board;
    }

    inline void
    setBoard(std::array<Field, BOARD_SIZE> other)
    {
        board = other;
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
        int winner = 0;
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

    std::vector<std::array<Field, BOARD_SIZE>>
    getNextLegalStates(Field player)
    {
        std::vector<std::array<Field, BOARD_SIZE>> states;
        for (uint i = 0; i < BOARD_SIZE; ++i)
            if (board[i] == Field::empty) {
                std::array<Field, BOARD_SIZE> tmp;
                std::copy(board.begin(), board.end(), tmp.begin());
                tmp[i] = player;
                states.push_back(tmp);
            }
        return states;
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

template<typename T>
struct RL
{
    T 
    getBestState(const std::vector<T> &states)
    {
        for (const T &it : states)
            prizeDict.insert(std::pair<T, float>(it, 0.0));

        std::uniform_real_distribution<float> zeroOneDist(0.0, 1.0);

        float maxPrize;
        T maxState;
        if (zeroOneDist(rng) > epsilon) {
            maxPrize = getStatesMaxPrize(states);
            std::vector<T> maxStates;
            std::copy_if(states.begin(), states.end(), maxStates.begin(),
                [=](T i) { return prizeDict[i] == maxPrize; } );
            maxState = randomChoice.choice(maxStates);
        } else {
            maxState = randomChoice.choice(states);
            maxPrize = prizeDict[maxState];
        }

        if (prizeDict.find(prevState) != prizeDict.end())
            prizeDict[prevState] += alpha * (maxPrize - prizeDict[prevState]);

        prevState = maxState;
        return maxState;   
    }

    inline void
    setPrize(T state, float prize)
    {
        assert(prizeDict.find(state) != prizeDict.end());
        prizeDict[state] = prize;
    }

    void
    resetPrevState()
    {
        prevState = initState;
    }

    RL(float a, float e, T init, std::default_random_engine engine)
    {
        alpha   = a;
        epsilon = e;
        rng     = engine;
        prevState = init;
        randomChoice = RandomChoice<std::vector<T>, T>(engine);
    }

private:

    float
    getStatesMaxPrize(const std::vector<T> &states)
    {
        assert(states.size() > 0);
        float max = prizeDict[states[0]];
        for (const float & it : states)
            if (prizeDict[it] > max)
                max = prizeDict[it];
        return max;
    }

    T prevState, initState;
    float alpha, epsilon;
    std::unordered_map<T, float> prizeDict;
    RandomChoice<std::vector<T>, T> randomChoice;
    std::default_random_engine rng;
};

void
test(void)
{
// tictactoe constructor test
    TicTacToe ttt = TicTacToe();
    std::array<Field, BOARD_SIZE> tab = ttt.getBoard();
    for (const auto &field : tab)
        assert(field == Field::empty);

// change field test
    ttt.setField(0, 0, Field::x);
    ttt.setField(2, 2, Field::o);

    tab = ttt.getBoard();

    assert(tab[0] == Field::x);
    assert(tab[8] == Field::o);

// test clearing
    ttt.clearBoard();
    tab = ttt.getBoard();
    for (int i = 0; i < BOARD_SIZE; ++i)
        assert(tab[i] == Field::empty);

// check winner test (row)
    for (int i = 0; i < 3; ++i)
        ttt.setField(i, 0, Field::x);   
    assert(ttt.checkForWinner() == Field::x);

// check winner test (diagonal)
    for (int i = 0; i < 3; ++i)
        ttt.setField(i, i, Field::o);
    assert(ttt.checkForWinner() == Field::o);

// check isGameEnded function
    assert(ttt.isGameEnded() == true);
    ttt.setField(1, 1, Field::empty);
    assert(ttt.isGameEnded() == false);

// test possible states
    ttt = TicTacToe();
    assert(ttt.getNextLegalStates(Field::o).size() == 9);
    ttt.setField(1, 1, Field::o);
    ttt.setField(2, 2, Field::x);
    assert(ttt.getNextLegalStates(Field::o).size() == 7);
}


int main(void)
{
    test();
// game
    static std:: default_random_engine rng_engine;
    TicTacToe ttt = TicTacToe();
    rng_engine.seed(time(nullptr));
    std::array<Field, BOARD_SIZE> emptyArr = std::array<Field, BOARD_SIZE>();
    emptyArr.fill(Field::empty);
    RL<std::array<Field, BOARD_SIZE>> rl1 = RL<std::array<Field, BOARD_SIZE>>(0.01, 0.001, emptyArr, rng_engine);
    RL<std::array<Field, BOARD_SIZE>> rl2 = RL<std::array<Field, BOARD_SIZE>>(0.01, 0.001, emptyArr, rng_engine);
    int i = 0;
    uint x, y;
    for (int i = 0; !ttt.isGameEnded(); ++i) {
        Field currPlayer = (i % 2) ? Field::x : Field::o;
        std::cout << "player -> "<< fieldToChar(currPlayer) << '\n'; 
        std::cout << "\nenter coords (first x then y)" << std:: endl;
        std::cin >> x;
        std::cin >> y;

        ttt.setField(x, y, currPlayer);
        ttt.showBoard();
    }
    return 0;
}