class TicTacToe:
    def __init__(self):
        self.tab = ['.','.','.','.','.','.','.','.','.','.']


    def curr_state(self):       #obecny stan tablicy
        return ''.join(self.tab)


    def postaw_znak_o(self, x, y):    #interfejs dla stawiania znaku przez gracza
        if x < 0 or x > 2 or y < 0 or y > 2 or self.tab[ x + (y*3) ] != '.': return 1              #kod bledu 1 - podano zla liczbe
        self.tab[ x + (y*3) ] = 'O'
        self.tab[9] = 'O'
        return 0


    def wyswietl_tab(self):     #wyswietla tablice
        for i in range(0, 7, 3):
            print( self.tab[ i : (i+3) ] )


    def reset(self):
        self.tab = ['.','.','.','.','.','.','.','.','.','.']
        

    def check_state(self, state):  #sprawdza wygrana
        # 1 = wygral x, 0 = remis, -1 = wygral O, -2 

        for i in range(0, 7, 3):
            if state[i:(i+3)] == ['X', 'X', 'X']: return 1
            if state[i:(i+3)] == ['O', 'O', 'O']: return -1

        for i in range(0,3):
            if state[i] == 'X' and state[(i+3)] == 'X' and state[(i+6)] == 'X': return 1
            if state[i] == 'O' and state[(i+3)] == 'O' and state[(i+6)] == 'O': return -1

        if state[0] == 'X' and state[4] == 'X' and state[8] == 'X': return 1
        elif state[0] == 'O' and state[4] == 'O' and state[8] == 'O': return -1
        elif state[2] == 'X' and state[4] == 'X' and state[6] == 'X': return 1
        elif state[2] == 'O' and state[4] == 'O' and state[6] == 'O': return -1
        elif '.' not in state: return 0 #w razie remisu return 0

        return -2


    def next_state_x(self):     #wyswietl mozliwe stany dla gracza x
        possibilities = []
        for i in range(0, 9):
            if self.tab[i] == '.':
                possibility = []
                possibility.extend(self.tab)
                possibility[9] = 'X'
                possibility[i] = 'X'
                possibilities.append(''.join(possibility))
        return possibilities

    #wyswietl mozliwe nastepne stany dla state przyjmuje stan jako string
    def next_state_x_for(self, state):     
        state = list(state)
        possibilities = []
        for i in range(0, 9):
            if state[i] == '.':
                possibility = []
                possibility.extend(self.tab)
                possibility[9] = 'X'
                possibility[i] = 'X'
                possibilities.append(''.join(possibility))
        return possibilities

    #zasadnicza roznica miedzy obydwiema powyzszymi funkcjami jest taka ze ta
    #pierwsza dziala dla game.tab a druga dla dowolnego stanu, napisalem ja
    #poniewaz musze dostac kolejne stany po danym w rekurencyjnym policy evaluation

    def next_state_o(self):     #wyswietl mozliwe stany dla gracza o
        possibilities = []
        for i in range(0,9):
            if self.tab[i] == '.':
                possibility = []
                possibility.extend(self.tab)
                possibility[9] = 'O'
                possibility[i] = 'O'
                possibilities.append(''.join(possibility))
        return possibilities
