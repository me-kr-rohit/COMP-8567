#!/bin/bash

# Check if sample.txt is present in your PWD
if [ -e sample.txt ]; then
  # Check if you are the owner of the file
  if [ $(stat -c '%U' sample.txt) == $(whoami) ]; then
    # Check if the file has write permission
    if [ -O sample.txt ]; then
      # Append the contents of ls -1 into sample.txt
      ls -1 >> sample.txt
      echo "Output of ls -1 has been appended to sample.txt"
    else
      # If the file has no write permission the set the write permission of the file using chmod
      chmod u+w sample.txt
      echo "Write permission is provided to sample.txt"
      ls -1 >> sample.txt
      echo "Output of ls -1 has been appended to sample.txt"
    fi
  else
    # Check If you are not the owner, output an appropriate message
    echo "!!Sorry, you are not the owner of the sample.txt file"
  fi
else
  # Create the file if it doesn't exist
  touch sample.txt
  echo "sample.txt file is created"
fi