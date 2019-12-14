import random
import pickle
import time
from reinforcementLearning import *
from ticTacToe import *

ALPHA = float(input("podaj alphe =>\t"))

gamemode = int(input("wybierz tryb gry: \n \
\t0 - opusc gre                         \n \
\t1 - gracz vs bot                      \n \
\t2 - bot vs bot (oddzielny rl)         \n \
\t3 - bot vs bot (update'owane dict'y)  \n \
\t4 - bot vs bot (sam przeciwko sobie)  \n \
=>\t"))

if gamemode == 1:        #gra uzytkownik

    rl = reinf_learning(ALPHA, float(input("podaj epsilon =>\t")))
    #ponizszy kod sluzy do wczytywania dicta z wartosciami nagrod
    filename = input("jezeli chcesz wczytac plik to podaj jego nazwe =>\t")
    if filename != '':
        fileObject = open(filename, 'rb')
        old_rl = pickle.load(fileObject)

        if old_rl:  rl.prize_dict = old_rl.copy()       #sprawdzam czy w dictcie w pliku binarnym jest cokolwiek zapisane
        else:       print("plik ktory probujesz wczytac jest pusty !")

    tmp = 1
    while tmp != 0:
        tmp1 = 0
        game = Tablica()
        while True:
            if tmp1 % 2 == 0:   
                while game.postaw_znak_o(int(input("podaj x => ")), int(input("podaj y => "))) == 1:
                    print("podano zle liczby!")
            else:               
                game.tab = rl.check_possible_states(game.next_state_x())
                print("ruch komputera")
            game.wyswietl_tab()
            game.sprawdz_wygrana()
            if game.wygrana != -2:
                if game.wygrana == -1:
                    print("przegrales")
                    rl.change_prize(rl.prev_state, 1.0)
                if game.wygrana == 1:
                    print("wygrales")
                    rl.change_prize(rl.prev_state, -10.0)
                if game.wygrana == 0:
                    print("remis")
                    rl.change_prize(rl.prev_state, -1.0)
                #reset poprzednich stanow i tablicy
                rl.prev_state = 0 
                game = Tablica()
                break
            tmp1 += 1
        tmp = int(input("czy chcesz kontynuowac? (jezeli nie wpisz 0) =>\t"))

elif gamemode == 2 or gamemode == 3:      #gra komputer

    helpful = int(input("podaj ilosc gier =>\t"))
    greedies = [float(x) for x in input("podaj e =>\t").split()]    #podajemy jeden lub kilka epsilonow

    game = Tablica()                                                #deklaracja tablicy do grania

    for curr_greedy in greedies:                                    #powtarzamy te petle dla kazdego danego epsilonu
        rl, rl_2 = reinf_learning(ALPHA, curr_greedy), reinf_learning(ALPHA, 0.0)
        time_start = time.time()                                    #zmienna ktora przechowuje czas od jakiego zaczynamy
        rl_wins, rl_2_wins, remis = 0, 0, 0                         #w tych zmiennych bedziemy zapisywac wyniki rozegranych gier

        for a in range(0, helpful):                                 #ta petla wykonywana jest tyle razy ile 
                                                                    #boty rozegraja gier
            tmp1 = 0 if a % 2 == 0 else 1                           #ta linijka pozwala zmieniac boty kolejnoscia
            while True:                                             #ta petla wykonywana jest dopoki toczy sie jedna gra
                game.tab = rl.check_possible_states(game.next_state_x()) if tmp1 % 2 == 0 else rl_2.check_possible_states(game.next_state_o())
                game.sprawdz_wygrana()
                if game.wygrana != -2:
                    if game.wygrana == -1:                          # -1 wygr X 0 remis 1 wygr O
                        rl_2.change_prize(rl_2.prev_state, -10.0)
                        rl.change_prize(rl.prev_state, 1.0)
                        rl_2_wins += 1
                    if game.wygrana == 1:
                        rl_2.change_prize(rl_2.prev_state, 1.0)
                        rl.change_prize(rl.prev_state, -10.0)
                        rl_wins += 1
                    if game.wygrana == 0:
                        rl_2.change_prize(rl_2.prev_state, -1.0)
                        rl.change_prize(rl.prev_state, -1.0)
                        remis += 1
                    #reset poprzednich stanow i tablicy
                    rl.prev_state = 0 
                    rl_2.prev_state = 0
                    game = Tablica()
                    break
                tmp1 += 1
                if gamemode == 3:                                   #update dict'ow po kazdym ruchu
                    rl.prize_dict.update(rl_2.prize_dict)
                    rl_2.prize_dict.update(rl.prize_dict)

        print("chciwosc => ", curr_greedy)
        print("wygrane bota nr 1 => ", rl_wins, "wygrane bota nr 2 => ", rl_2_wins)
        print("remisy => %i czas => %.3f" % (remis, (time.time() - time_start)))

elif gamemode == 4:

    helpful = int(input("podaj ilosc gier =>\t"))
    game = Tablica()                                                #deklaracja tablicy do grania

    rl = reinf_learning(ALPHA, 0.001)
    time_start = time.time()                                    #zmienna ktora przechowuje czas od jakiego zaczynamy
    rl_wins, rl_2_wins, remis = 0, 0, 0                         #w tych zmiennych bedziemy zapisywac wyniki rozegranych gier
    rl_prev_state = 0                                           #poprzedni stan w pierwszym przypadku
    rl_2_prev_state = 0                                         #poprzedni stan w drugim przypadku

    for a in range(0, helpful):                                 #ta petla wykonywana jest tyle razy ile 
                                                                #boty rozegraja gier
        tmp1 = 0 if a % 2 == 0 else 1                           #ta linijka pozwala zmieniac boty kolejnoscia
        while True:                                             #ta petla wykonywana jest dopoki toczy sie jedna gra
            if tmp1 % 2 == 0:
                rl.prev_state = rl_prev_state                   #zmienne z poprzednimi stanami sa tutaj potrzebne
                game.tab = rl.check_possible_states(game.next_state_x())    #poniewaz bot rozgrywajac na przemian sam ze
                rl_prev_state = rl.prev_state                   #soba gry niestety moze sie zdarzyc ze zapamieta jako ostani
            else:                                               #stan gdy gral jako X i bedzie chcial jego nagrode powiazac z
                rl.prev_state = rl_2_prev_state                 #stanem gdy gra jako O
                game.tab = rl.check_possible_states(game.next_state_o())
                rl_2_prev_state = rl.prev_state

            game.sprawdz_wygrana()
            if game.wygrana != -2:
                if game.wygrana == -1:
                    rl.change_prize(rl_prev_state, 1.0)
                    rl.change_prize(rl_2_prev_state, -10.0)
                    rl_wins += 1
                if game.wygrana == 1:
                    rl.change_prize(rl_prev_state, -10.0)
                    rl.change_prize(rl_2_prev_state, 1.0)
                    rl_2_wins += 1
                if game.wygrana == 0:
                    rl.change_prize(rl_prev_state, -1.0)
                    rl.change_prize(rl_2_prev_state, -1.0)
                    remis += 1
                #reset poprzednich stanow i tablicy
                rl.prev_state = 0 
                rl_prev_state = 0
                rl_2_prev_state = 0
                game = Tablica()
                break
            tmp1 += 1

    print("wygrane bota nr 1 => ", rl_wins, "wygrane bota nr 2 => ", rl_2_wins)
    print("remisy => %i czas => %.3f" % (remis, (time.time() - time_start)))

if gamemode != 0: #tutaj jakby bylo zapisywanko dict z wartosciami nagrod
    filename = input("Czy chcesz zapisac rl ? (jezeli tak podaj nazwe pliku) =>\t")
    if filename != '':
        fileObject = open(filename, 'wb')
        pickle.dump(rl.prize_dict, fileObject)
