import re
import sys
str1 = "consensus.nPowTargetTimespan ="
str2 = "consensus.nPowTargetSpacing ="
def changeFile(secs, deStr):
	strSecs = str(secs)
	with open("TEST_chainparams.cpp") as rFile:
		rText = rFile.read()
	output = re.search(deStr, rText)
	startChar = output.span()[1]
	allSemis = re.finditer(";", rText)
	endChar = 0
	for i in allSemis:
		endChar = i.span()[0]
		if(endChar > startChar):
			break;
	editSect = rText[startChar:endChar]
	splitFile = rText.split(editSect)
	editStr = strSecs.join(splitFile)
	wFile = open("chainparams.cpp", "w")
	wFile.write(editStr)
def dewit(numSecs):
	changeFile(numSecs, str1)
	changeFile(numSecs,str2)
dewit(sys.argv[1])
