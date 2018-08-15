from collections import defaultdict
from string import punctuation
import math
import re
import sys

buffer = open("nbmodel.txt","r",encoding="utf-8")

line = buffer.readlines()

prior = defaultdict(int)
likelihood = defaultdict(lambda:defaultdict(int))

for i,sline in enumerate(line[1:]):
    sline = sline.strip();
    if(sline == "DummyLikelihood"):
        break
    word = sline.split();
    prior[word[0]] = math.log(float(word[1]))
    
for sline in line[i+2:]:
    sline = sline.strip();
    word = sline.split();
    likelihood[word[0]][word[1]] = math.log(float(word[2]))
    
buffer.close()

testdata = open(sys.argv[1],"r",encoding = "utf-8")

wfile = open("nboutput.txt","w+",encoding = "utf-8")

for reviews in testdata:
    sreview = reviews.strip()
    words = sreview.split()
    identifier = words[0]
    probtrue = prior["True"]
    probfake = prior["Fake"]
    probpos = prior["Pos"]
    probneg = prior["Neg"]
    
    for sword in words[1:]:
        sword = sword.strip(punctuation)
        moreword = re.split('[^A-Za-z0-9\']+',sword)
        for ind in moreword:
            if ind:
                lowerword = ind.lower()
           
                if likelihood[lowerword]["True"] is not None:       
                    probtrue = probtrue + likelihood[lowerword]["True"]
                    probfake = probfake + likelihood[lowerword]["Fake"]
                
                if likelihood[lowerword]["Pos"] is not None:
                    probpos = probpos + likelihood[lowerword]["Pos"]
                    probneg = probneg + likelihood[lowerword]["Neg"]
    
    if probtrue >= probfake:
        classify1 = "True"
    else:
        classify1 = "Fake"
        
    if probpos >= probneg:
        classify2 = "Pos"
    else:
        classify2 = "Neg"
        
    wfile.write(str(identifier) + " " + classify1 + " " + classify2)
    wfile.write("\n")

testdata.close()  
wfile.close()