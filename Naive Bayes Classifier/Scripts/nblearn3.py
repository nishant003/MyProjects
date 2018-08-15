from _collections import defaultdict
from string import punctuation
import re
import sys

buffer = open(sys.argv[1], "r", encoding = "utf-8")
corpus = buffer.readlines()
buffer.close()

prior = defaultdict(int)
likelihood = defaultdict(int)
priordenom = 0
likelidenom = defaultdict(int)
stopwords = defaultdict(int)

stopwords = {'a':1,'an':1,'and':1,'are':1,'as':1,'at':1,'be':1,'by':1,'for':1,'from':1,'has':1,'he':1,'i':1,'in':1,'is':1,'it':1,'its':1,'of':1,'on':1,'that':1,'the':1,'this':1,'to':1,'was':1,'were':1,'will':1,'with':1}

for reviews in corpus:
    review = reviews.strip()
    tokens = review.split()
    prior[tokens[1]]+=1
    prior[tokens[2]]+=1
    priordenom+=1
    for word in tokens[3:]:
        word = word.strip(punctuation)
        moreword = re.split('[^A-Za-z0-9\']+',word)
        for ind in moreword:
            if ind:
                lowerword = ind.lower()
                if lowerword not in stopwords:
                    likelihood[lowerword,tokens[1]]+=1
                    likelihood[lowerword,tokens[2]]+=1
                    likelidenom[tokens[1]]+=1
                    likelidenom[tokens[2]]+=1

wfile = open("nbmodel.txt", "w+", encoding="utf-8")

wfile.write("DummyPrior" + "\n")
for ind in prior:
    priorprob = prior[ind]/priordenom
    wfile.write(str(ind) + " " + str(priorprob) + "\n")
    
for ind in list(likelihood):
    likelihood[ind]+=1
    likelidenom[ind[1]]+=1
    if ind[1] == "True" and likelihood[ind[0],"Fake"] == 0:
        likelihood[ind[0],"Fake"]+=1
        likelidenom["Fake"]+=1
    elif ind[1] == "Fake" and likelihood[ind[0],"True"] == 0:
        likelihood[ind[0],"True"]+=1
        likelidenom["True"]+=1
    elif ind[1] == "Pos" and likelihood[ind[0],"Neg"] == 0:
        likelihood[ind[0],"Neg"]+=1
        likelidenom["Neg"]+=1
    elif ind[1] == "Neg" and likelihood[ind[0],"Pos"] == 0:
        likelihood[ind[0],"Pos"]+=1
        likelidenom["Pos"]+=1

wfile.write("DummyLikelihood" + "\n")
for ind in likelihood:
    likeliprob = likelihood[ind]/likelidenom[ind[1]]
    wfile.write(str(ind[0]) + " " + str(ind[1]) + " " + str(likeliprob) + "\n")
                
wfile.close()