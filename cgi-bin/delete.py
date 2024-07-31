#!/usr/bin/python3
import os
import sys
import shutil


try:
	if not os.path.exists(sys.argv[1]):
		print("Status: 404 Not Found")

	if os.path.isfile(sys.argv[1]):
		os.unlink(sys.argv[1])
		print("Status: 204 No Content")

	elif (os.path.isdir(sys.argv[1])):
		shutil.rmtree(sys.argv[1])
		print("Status: 204 No Content")

except Exception as e:
    print("Status: 500 Internal Server Error")
