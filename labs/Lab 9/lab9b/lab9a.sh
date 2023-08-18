#!/bin/bash  

#error out, in case of no text file provided
if [ $# -eq 0 ]; then  
  echo "!!ErrorOccured: No-input-files-specified."  
  exit 1  
fi 

#declared variable for output.txt
output="output.txt"  
content=""  

#append the contents of all the text files in serial order
until [ -z "$1" ]; do  
  if [ ! -f "$1" ] || [ "${1: -4}" != ".txt" ]; then  
    echo "!!ErrorOccured: $1 is not a valid text file"  
    exit 1  
  fi 
 
#check the number of allowed text files
text_file_counter=0  
for file in "$@"; do  
        ((text_file_counter++))  
        if ((text_file_counter > 5)); then  
            echo "ErrorOccured: Only up to 5 text files are allowed."  
            exit 1  
        fi  
done  
  content+=$(cat "$1") 
  content+="\n"  
  shift  
done  

#write in the output.txt file
echo -e "$content" > "$output"  
echo "Concatenated contents of input files saved to $output"