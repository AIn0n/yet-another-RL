from reinforcementLearning import *
from ticTacToe import *
import time

ALPHA = 0.1
FI = 0.1

rl = reinf_learning(ALPHA)
game = TicTacToe()

#==============================PONIZSZY KOD MA UCZYC============================

def policy_eval_rec(rl, game, char):
    if char == 'X':
        next_state_set = game.next_state_x()
    else:
        next_state_set = []
        aux = game.next_state_o()
        for state in aux:
            next_state_set += game.next_state_x_for(state)

    for state in next_state_set:

        win = game.check_state(state)
        if win == 1:    rl.change_prize( state, 5.0)
        elif win == -1: rl.change_prize( state, -5.0)
        elif win == 0:  rl.change_prize( state, -1.0)

        if win == -2:
            curr_state = game.curr_state()

            rl.prev_state = state
            game.tab = state

            if char == 'X': policy_eval_rec(rl, game, 'O')
            else:           policy_eval_rec(rl, game, 'X')

            rl.prev_state = curr_state
            game.tab = curr_state

    rl.policy_evaluation(next_state_set)

def policy_imprv_rec(rl, game, char):
    if char == 'X':
        next_state_set = game.next_state_x()
    else:
        next_state_set = []
        aux = game.next_state_o()
        for state in aux:
            next_state_set += game.next_state_x_for(state)

    for state in next_state_set:

        win = game.check_state(state)

        if win == -2:

            curr_state = game.curr_state()

            rl.prev_state = state
            game.tab = state

            if char == 'X': policy_imprv_rec(rl, game, 'O')
            else:           policy_imprv_rec(rl, game, 'X')

            rl.prev_state = curr_state
            game.tab = curr_state

    rl.policy_improvement(next_state_set)

time_counter = time.time()      #do liczenia czasu

while rl.is_policy_stable == False:

    print("policy improvment")

    while True:
        rl.delta = 0.0                  #zawsze zaczyna od zerowej delty
        aux = policy_eval_rec(rl, game, "X")  #tutaj rekurencyjnie policy evaluation
        print("policy eval")
        game.reset()
        if rl.delta < FI: break

    rl.is_policy_stable = True
    policy_imprv_rec(rl, game, "X")  #tutaj rekurencyjnie policy improvment
    game.reset()
print("czas = ", time.time() - time_counter)

#=========================KOD NIZEJ SLUZY DO GRY===============================

tmp = 1                 #zmiana wartosci tmp umozliwia nam przerwanie gry
while tmp != 0:
    tmp1 = 1            #zmiana tej wartosci umozliwa nam decydowanie kto zaczyna
                        #jesli jest parzysta/zero to gracz, w przeciwnym razie RL
    game.reset()        #reset tablicy i poprzedniego stanu
    rl.prev_state = game.curr_state() 
    while True:
        if tmp1 % 2 == 0:   
            while game.postaw_znak_o(int(input("podaj x => ")), int(input("podaj y => "))) == 1:
                print("podano zle liczby!")
        else:               
            game.tab = rl.check_possible_states(game.next_state_x())
            print("ruch komputera")

        game.wyswietl_tab()
        win = game.check_state(game.tab)
        if win != -2:
            if win == 1:    print("przegrales")
            if win == -1:   print("wygrales")
            if win == 0:    print("remis")
            break
        tmp1 += 1        
    tmp = int(input("czy chcesz kontynuowac? (jezeli nie wpisz 0) =>\t"))
