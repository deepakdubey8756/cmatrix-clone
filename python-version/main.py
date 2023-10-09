import time
import os
import random
import string


GREEN = '\033[92m'
RESET = '\033[0m'

def setCharacters(threshold):
    characters = string.ascii_letters + string.digits + string.punctuation
    for i in range(threshold):
        characters += " "
    return characters
    


class MyMatrix():

    def __init__(self):
         self.characters = setCharacters(160)
         self.rowLength = 0
         self.columnLength = 0
         self.matrix = None

        
    def giveRandomChars(self, columnLength):
        reqChars = []
        for i in range(columnLength):
            reqChars.append(random.choice(self.characters))
        return reqChars

    def setMatrix(self):
        tSize = os.get_terminal_size()
        if tSize.columns - 2 * self.columnLength < 2 and tSize.lines == (self.rowLength + 2):
            return
        self.columnLength = (tSize.columns // 2)
        self.rowLength = tSize.lines - 2
        self.matrix = [[" "]*tSize.columns]*tSize.lines
        

    def logic(self):
        self.setMatrix()
        newChars = self.giveRandomChars(self.columnLength)
        for i in range(self.rowLength):
            item = self.matrix[i]
            self.matrix[i] = newChars
            newChars = item
    

    def draw(self):
        for i in range(self.rowLength):
            for j in range(self.columnLength):
                print(GREEN  + self.matrix[i][j], end=" ")
            print(RESET)
    		
    def clear(self, t):
        time.sleep(t)
        os.system("clear")
	
if __name__ == "__main__":
    g1 = MyMatrix() 
    g1.clear(0)
    while True:
        g1.logic()
        g1.draw()
        g1.clear(0.08)