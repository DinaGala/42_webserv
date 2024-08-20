#!/usr/bin/python3
import os
import shutil

try:
    path = os.environ.get("PATH_INFO")
    
    if path is None:
        print("Status: 400 Bad Request")
    
    elif not os.path.exists(path):
        print("Status: 404 Not Found")

    elif os.path.isfile(path):
        os.unlink(path)
        print("Status: 204 No Content")

    elif os.path.isdir(path):
        shutil.rmtree(path)
        print("Status: 204 No Content")

except Exception as e:
    print("Status: 500 Internal Server Error")

