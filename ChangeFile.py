import re
import sys
#Variables in chainparams.cpp which affect blocktime
str1 = "consensus.nPowTargetTimespan ="
str2 = "consensus.nPowTargetSpacing ="
def changeFile(secs, whichStr):
	strSecs = str(secs)
	#Opens chainparams.cpp in a way so it closes automatically
	with open("TEST_chainparams.cpp") as rFile:
		rText = rFile.read()
	output = re.search(whichStr, rText)
	startChar = output.span()[1]
	allSemis = re.finditer(";", rText)
	endChar = 0
	for i in allSemis:
		endChar = i.span()[0]
		#If it is after the end of the passed string...
		if(endChar > startChar):
			break;
			#...The for loop breaks and endChar remains the correct value
	editSect = rText[startChar:endChar]
	splitFile = rText.split(editSect)
	#editStr is made the whole file now with edited blocktime
	editStr = strSecs.join(splitFile)
	wFile = open("chainparams.cpp", "w")
	wFile.write(editStr)
def dewit(numSecs):
	changeFile(numSecs, str1)
	changeFile(numSecs,str2)
dewit(sys.argv[1])
