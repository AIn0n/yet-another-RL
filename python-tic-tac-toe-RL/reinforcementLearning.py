import random 
class reinf_learning:
    def __init__(self, alfa, greedy): 
        self.prize_dict = dict()
        self.alpha = alfa       #alfa
        self.prev_state = 0     #poprzedni stan
        self.e = greedy         #wspolczynnik chciwosci

    def change_prize(self, curr_state, newPrize):
        self.prize_dict[curr_state] = newPrize

    #to jest do debugowania tylko (nie zwracac uwagi jesli cos nie dziala)
    def show_prizes(self, tab_states):
        for state in tab_states:
            if state in self.prize_dict:
                print("stan => ", state, "nagroda => ", self.prize_dict[state])


    def check_possible_states(self, tab_states):
        for possibly_state in tab_states:
            if possibly_state not in self.prize_dict:
                self.prize_dict[possibly_state] = 0.0

        if random.random() > self.e:        # jezeli randomowa liczba jest wieksza od jakiegostam epsilonu to wtedy mamy
                                            # klasycznie liczone funkcja wartosci a wybieramy przy pomocy 
             biggest_prize = max( [self.prize_dict[x] for x in tab_states] )    #robimy tablice z max wartosciami 
             biggest_prize_state = random.choice( [x for x in tab_states if self.prize_dict[x] == biggest_prize] )            
        else:
            biggest_prize_state = random.choice(tab_states)
            biggest_prize = self.prize_dict[biggest_prize_state]

        #funkcja wartosci here
        if self.prev_state in self.prize_dict:
            self.prize_dict[self.prev_state] += (self.alpha * (biggest_prize - self.prize_dict[self.prev_state]))

        self.prev_state = biggest_prize_state       #nasz obecnie wybrany stan bedzie poprzednim stanem przy nastepnym liczeniu
                
        return list(biggest_prize_state)
