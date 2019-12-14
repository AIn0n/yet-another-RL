class Tablica:
    def __init__(self):
        self.tab = ['.','.','.','.','.','.','.','.','.','.']
        # -2 brak wyniku, -1 wygral komputer, 0 remis, 1 wygral gracz
        self.wygrana = -2
    def curr_state(self):       #obecny stan tablicy
        return ''.join(self.tab)
    def postaw_znak_o(self, x, y):    #interfejs dla stawiania znaku przez gracza
        if x < 0 or x > 2 or y < 0 or y > 2 or self.tab[ x + (y*3) ] != '.': return 1                    #kod bledu 1 - podano zla liczbe
        self.tab[ x + (y*3) ] = 'O'
        self.tab[9] = 'O'
        return 0
    def wyswietl_tab(self):     #wyswietla tablice
        for i in range(0, 7, 3):
            print( self.tab[ i : (i+3) ] )

    def sprawdz_wygrana(self):  #sprawdza wygrana
        # 1 = wygral x, 0 = remis, -1 = wygral O, -2 
        for i in range(0, 7, 3):
            if self.tab[i:(i+3)] == ['X', 'X', 'X']:
                self.wygrana = -1
                return 0
            if self.tab[i:(i+3)] == ['O', 'O', 'O']:
                self.wygrana = 1
                return 0
        for i in range(0,3):
            if self.tab[i] == 'X' and self.tab[(i+3)] == 'X' and self.tab[(i+6)] == 'X':
                self.wygrana = -1
                return 0
            if self.tab[i] == 'O' and self.tab[(i+3)] == 'O' and self.tab[(i+6)] == 'O':
                self.wygrana = 1
                return 0
        if self.tab[0] == 'X' and self.tab[4] == 'X' and self.tab[8] == 'X':
            self.wygrana = -1
            return 0
        elif self.tab[0] == 'O' and self.tab[4] == 'O' and self.tab[8] == 'O':
            self.wygrana = 1
            return 0
        elif self.tab[2] == 'X' and self.tab[4] == 'X' and self.tab[6] == 'X':
            self.wygrana = -1
            return 0
        elif self.tab[2] == 'O' and self.tab[4] == 'O' and self.tab[6] == 'O':
            self.wygrana = 1
            return 0
        elif '.' not in self.tab: #w razie remisu
            self.wygrana = 0
            return 0

        for elem in self.tab:
            if elem != 'O' and elem != 'X' and elem != '.':
                return 1        #zwroc jeden jesli w tablicy pojawil sie niedozwolony znak 

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
