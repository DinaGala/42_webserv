# cgi-bin/cookies_test.py

import os
import http.cookies
import sys

def main():
    # Define the cookie name and value
    cookie_name = 'user'
    cookie_value = 'user42'

    # Retrieve the cookies from environment variables
    cookies_header = os.environ.get('HTTP_COOKIE', '')
    
    # Create a cookie object
    cookies = http.cookies.SimpleCookie()
    cookies.load(cookies_header)

    # Check if the cookie is already set
    if cookie_name not in cookies:
        # Cookie is not set
        body = f"Cookie named '{cookie_name}' is not set!"
    else:
        # Cookie is set, retrieve its value
        current_value = cookies[cookie_name].value
        body = f"Cookie '{cookie_name}' is set!<br>Value is: {current_value}"

    # Set the cookie for future requests
    cookies[cookie_name] = cookie_value

    # Print HTTP headers
    print("Content-Type: text/html")
    print(f"Set-Cookie: {cookies[cookie_name].OutputString()}")
    print()  # End of headers

    # Output the body content
    print(f"<html><body>{body}</body></html>")

if __name__ == '__main__':
    main()