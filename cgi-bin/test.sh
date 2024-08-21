#!/bin/bash

# Define the URL for the POST request
url="http://localhost:8081/cgi"

# Define the filename and data to write
url="localhost:8081/cgi"

curl -X POST -F 'file=@"src/Utils.cpp";filename="uploaded_files"' $url
