import random 
class reinf_learning:
    def __init__(self, alfa, usr_fi): 
        self.prize_dict = dict()
        self.alpha = alfa       #alfa
        self.prev_state = 0     #poprzedni stan
        self.is_policy_stable = False      #true jezeli policy jest stabilne
        self.fi =  usr_fi             #to powinna byc mala, dodatnia liczba

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
            self.prize_dict[self.prev_state] = [0.0, False]
        
        for state in states:

            delta = 0.0

            old_prize = self.prize_dict[self.prev_state][0]
            self.prize_dict[self.prev_state][0] += (self.alpha * (self.prize_dict[state][0] - self.prize_dict[self.prev_state][0]))

            delta = max(delta, abs( old_prize - self.prize_dict[self.prev_state][0]))

            if delta < self.fi: self.is_policy_stable = True
            else:               self.is_policy_stable = False

        if self.prize_dict[self.prev_state][1] == False:
            self.prize_dict[self.prev_state][1] = random.choice(states)

        return states


    def policy_improvement(self, states):
        for state in states:

            old_action = self.prize_dict[self.prev_state][1]
            self.prize_dict[self.prev_state][1] = max(self.prize_dict, key=self.prize_dict.get)            

            if old_action != self.prize_dict[self.prev_state][1]: self.is_policy_stable = False

        if self.is_policy_stable == False:
            policy_evaluation(states)
