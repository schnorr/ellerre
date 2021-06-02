#!/usr/bin/python3
import os,sys
from re import search

write = True
step = 0;

while(write):

    step_next = str(step+1)
    step_str = str(step)
    write = False
    with open('./LR0-steps.dot') as f:
        for line in f:
            if ("step=" + step_str) in line:
                f1 = open("LR0-steps-" + step_str + ".dot", "w")
                write=True
                
            if(write):
                if(search ("# newstep=" + step_next, line.strip())):
                    break
                else:
                    f1.write(line)   

    step=step+1 

for i in range(0, (step-1)):
    convert_line = "dot -Tpng LR0-steps-" + str(i) + ".dot -o " + str(i) + "-out.png"
    os.system(convert_line)