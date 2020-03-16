import sys
import random
import string

strip = []
slength = int(raw_input("Strip Length: ")) 

b = 0
while b < slength:
	strip.append(0)
	b += 1

ncoins = int(raw_input("Number of Coins: "))

def placeCoins(s, n):
	i = 0
	while i < n:
		position = random.randint(0, (len(s) - 1))
		if s[position] == 0:
			s[position] += 1
			i += 1
	return s

rstrip = placeCoins(strip, ncoins)

def willCross(s, c, n):
	i = 1
	for i in range(1, n):
		if s[c-i] == 1:
			return True
		else:
			i += 1

def firstXFilled(s, n):
	i = 0
	while i < n:
		if s[i] == 1:
			i += 1
		else:
			return False
	return True

j = 0
while (firstXFilled(rstrip, ncoins) == False):
	if j == 0:
		j += 1
		print("Rules:\n 1. Coins move only to the left.\n 2. No coin may pass another.\n 3. No square may hold more than one coin.\n 4. All user inputs must be positive integer values.\n 5. Follow instructions on how to input coin position and number of steps\nEnjoy!")
	if ((j % 2) != 0):
		print(rstrip)
		uinput = raw_input("Player 1: coin position, steps\n").split(",")
		coinp = int(uinput[0])
		msteps = int(uinput[1])
		while ((coinp > len(rstrip)) or (rstrip[coinp] == 0) or (msteps > coinp) or (rstrip[coinp - msteps] == 1) or (willCross(rstrip, coinp, msteps))): ## add more stuff
			print("Illegal input. Try again!")
			print(rstrip)
			uinput = raw_input("Player 1: coin position, steps\n").split(",")
			coinp = int(uinput[0])
			msteps = int(uinput[1])
		rstrip[coinp] -= 1
		rstrip[coinp-msteps] += 1
		j += 1
	elif ((j % 2) == 0):
		print(rstrip)
		uinput = raw_input("Player 2: coin position, steps\n").split(",")
		coinp = int(uinput[0])
		msteps = int(uinput[1])
		while ((coinp > len(rstrip)) or (rstrip[coinp] == 0) or (msteps > coinp) or (rstrip[coinp - msteps] == 1) or (willCross(rstrip, coinp, msteps))): ## add more stuff
			print("Illegal input. Try again!")
			print(rstrip)
			uinput = raw_input("Player 2: coin position, steps\n").split(",")
			coinp = int(uinput[0])
			msteps = int(uinput[1])
		rstrip[coinp] -= 1
		rstrip[coinp-msteps] += 1
		j += 1

if j == 0:
	print(rstrip)
	print("Game is already set. Restart!")

else:
	if (((j-1) % 2) == 0):
		print(rstrip)
		print("Winner: Player 2")
	else:
		print(rstrip)
		print("Winner: Player 1")