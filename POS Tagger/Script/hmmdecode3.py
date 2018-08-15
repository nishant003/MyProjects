from collections import defaultdict
import time
import sys

start=time.time()

buffer = open("hmmmodel.txt","r",encoding="utf-8")

line = buffer.readlines()

emission=defaultdict(lambda:defaultdict(int))
transition = defaultdict(int)
for i,sline in enumerate(line[1:]):
    sline = sline.strip()
    if (sline=="DummyTransition"):
        break
    word = sline.split(" ")
    t = (word[0],word[1])
    emission[word[0]][word[1]] = float(word[2])

for j,sline2 in enumerate(line[i+2:]):
    sline2 = sline2.strip()
    if (sline2=="DummyTag"):
        break
    word = sline2.split(" ")
    t = (word[0],word[1])
    transition[t] = float(word[2])

tags = list()
for sline3 in line[i+j+3:]:
    sline3 = sline3.strip()
    tags.append(sline3)

buffer.close()
    
line = open("zh_dev_raw.txt","r",encoding = "utf-8")

wfile = open("hmmoutput.txt","w+",encoding = "utf-8")

prob_prior = defaultdict(lambda:defaultdict(int))
prev_tag = defaultdict(str)
taglist = list()
wordtag = list()
for sline in line:
    sline = sline.strip()
    words = sline.split(" ")
    prob_prior[-1]["startstate"] = 1;
    pretag = "startstate"
    for index,sword in enumerate(words):
        flag = True
        flag = emission.__contains__(sword)
        if flag==False:
            prob_em=1
            for curtag in tags:
                maxim = 0
                for temptag in prob_prior[index-1]:
                    pretag = temptag
                    prob_trans = transition[pretag,curtag]
                    pretag_prob = prob_em*prob_trans*prob_prior[index-1][pretag]
                    if pretag_prob > maxim:
                        maxim = pretag_prob
                        maxtag = pretag
                prob_prior[index][curtag] = maxim
                prev_tag[curtag,index] = maxtag
        else:
            for sem in emission[sword]:
                curtag = sem
                prob_em = emission[sword][sem]
                maxim = 0
                maxtag = "nothing"
                for temptag in prob_prior[index-1]:
                    pretag = temptag
                    prob_trans = transition[pretag,curtag]
                    pretag_prob = prob_em*prob_trans*prob_prior[index-1][pretag]
                    if pretag_prob > maxim:
                        maxim = pretag_prob
                        maxtag = pretag
                prob_prior[index][curtag] = maxim
                prev_tag[curtag,index] = maxtag
   
    maxim = 0
    index+= 1
    for temptag in prob_prior[index-1]:
            pretag = temptag
            prob_trans = transition[pretag,"endstate"]
            end_prob = prob_trans*prob_prior[index-1][pretag]
            if end_prob > maxim:
                maxim = end_prob
                maxtag = pretag
    prob_prior[index]["endstate"] = maxim
    prev_tag["endstate",index] = maxtag
    
    tag = "endstate"
    tempindex = index;
    while index!=0:
        taglist.append(prev_tag[tag,index])
        tag = prev_tag[tag,index]
        index-= 1
    taglist.reverse()
    for it in range(0,tempindex):
        t = (words[it],taglist[it])
        wfile.write(words[it]+"/"+taglist[it]+" ")
    wfile.write("\n")
    prob_prior.clear()
    prev_tag.clear()
    
        
        
                
                        
            