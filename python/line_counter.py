#Takes in directory name and counts lines in files in dir using wc

import os
import sys
import subprocess 

#works on windows?
dir_name = "\\" + sys.argv[1] 

path = os.path.join(os.getcwd() + dir_name)
os.chdir(path)

for file in os.listdir(path):
	word_count = subprocess.getoutput(['wc', '-l', file])
	print(file + " " + str(word_count.split()[0]))
