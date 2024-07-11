#!/bin/bash

# URL to send the POST request to
URL="http://localhost:8080/your_endpoint"

# Data to send in the POST request
DATA='{"filename": "test.txt", "content": "This is a test file content."}'

# Sending the POST request
curl -X POST -H "Content-Type: application/json" -d "$DATA" "$URL"
