from collections import defaultdict
import time
import math
import sys

line = open("zh_train_tagged.txt","r",encoding = "utf-8")

emission=defaultdict(int)
transition = defaultdict(int)
tag = defaultdict(int)

for sline in line:
    sline = sline.strip()
    word = sline.split(" ")
    tag["startstate"]+=1
    tag["endstate"]+=1
    ftag = "startstate"
    
    for sword in word:
        wordtag = sword.rsplit('/',1)
        t1 = (wordtag[0],wordtag[1])
        t2 = (ftag,wordtag[1])
        tag[wordtag[1]]+=1
        emission[t1]+= 1
        transition[t2]+= 1
        ftag = wordtag[1]
    transition[ftag,"endstate"]+= 1

line.close()



wfile = open("hmmmodel.txt","w+",encoding = "utf-8")

wfile.write("DummyEmission" + "\n")
for sem in emission:
    emission[sem] = emission[sem]/tag[sem[1]]
    wfile.write(str(sem[0]) + " " + str(sem[1]) + " " + str(emission[sem]) + "\n")
    
for stag1 in tag:
    for stag2 in tag:
        transition[stag1,stag2]+= 1
        tag[stag1]+= 1

wfile.write("DummyTransition" + "\n")  
for strans in transition:  
    transition[strans] = transition[strans]/tag[strans[0]]
    wfile.write(str(strans[0]) + " " + str(strans[1]) + " " + str(transition[strans]) + "\n")

tag.__delitem__("startstate")
tag.__delitem__("endstate")
wfile.write("DummyTag" + "\n")
for stag in tag:
    wfile.write(str(stag) + "\n")
  
wfile.close()

    
    