#!/usr/bin/python3
import os,sys
from re import search

write = True
step = 0;
file_name = sys.argv[1]

while(write):
    
    step_str = str(step)
    write = False

    with open(file_name) as f:
        for line in f:
            if(search ("step=" + str(step+1), line.strip())):
                break

            if ("step=" + step_str) in line:
                f1 = open("LR-steps-" + step_str + ".dot", "w")
                write=True
            
            if(write):
                f1.write(line) 
    step=step+1 

for i in range(0, step-1):
    convert_line = "dot -Tpng LR-steps-" + str(i) + ".dot -o " + str(i) + "-out.png"
    print(convert_line)
    os.system(convert_line)

