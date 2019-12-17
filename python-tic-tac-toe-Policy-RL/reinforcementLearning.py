import random 
class reinf_learning:
    def __init__(self, alfa): 
        self.prize_dict = dict()
        self.alpha = alfa       #alfa
        self.prev_state = 0     #poprzedni stan
        self.is_policy_stable = False      #true jezeli policy jest stabilne
        self.delta = 0.0

    def change_prize(self, curr_state, newPrize):
        self.prize_dict[curr_state] = [newPrize, False]
        # to pierwsze pokazuje nagrode   ^           ^
        # dla danego stanu a drugie     V(s)        Pi(s)
        # pokazuje najbardziej
        # optymalna akcje dla naszego obecnego stanu


    #to jest do debugowania tylko (nie zwracac uwagi jesli cos nie dziala)
    def show_prizes(self, tab_states):
        for state in tab_states:
            if state in self.prize_dict:
                print("stan => ", state, " nagroda => ", self.prize_dict[state][0], " policy => ", self.prize_dict[state][1])


    def check_possible_states(self, states):
        for state in states:
            if state not in self.prize_dict:
                self.prize_dict[state] = [0.0, False]

        if self.prize_dict[self.prev_state][1] in states:               #jezeli stan w policy istnieje to go wybierzemy
            biggest_prize_state = self.prize_dict[self.prev_state][1]
        else:
            biggest_prize_state = random.choice(states)            

        self.prev_state = biggest_prize_state       #nasz obecnie wybrany stan bedzie poprzednim stanem przy nastepnym liczeniu
        return list(biggest_prize_state)

    def policy_evaluation(self, states):
        for state in states:
            if state not in self.prize_dict:
                self.prize_dict[state] = [0.0, False]

        if self.prev_state not in self.prize_dict:
            self.prize_dict[self.prev_state] = [0.0, random.choice(states)]
        
        for state in states:

            old_prize = self.prize_dict[self.prev_state][0]
            self.prize_dict[self.prev_state][0] += (self.alpha * (self.prize_dict[state][0] - self.prize_dict[self.prev_state][0]))

            self.delta = max(self.delta, abs( old_prize - self.prize_dict[self.prev_state][0]))



    def policy_improvement(self, states):
        for state in states:

            old_action = self.prize_dict[self.prev_state][1]

            biggest_prize = max( [self.prize_dict[x] for x in states] )
            biggest_prize_states = [x for x in states if self.prize_dict[x] == biggest_prize]
            if old_action not in biggest_prize_states:
                self.prize_dict[self.prev_state][1] = random.choice(biggest_prize_states)
                self.is_policy_stable = False
