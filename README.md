## Table of Contents
1. [Project Overview](#project-overview)
2. [Features](#features)
3. [How It Works](#how-it-works)
4. [Installation](#installation)
5. [Usage](#usage)


## Project Overview

Webserv is a lightweight HTTP server written in C++ as part of the 42 curriculum. It is designed to handle basic web serving functionalities like handling HTTP requests, serving static files, and executing CGI scripts. The goal of this project is to gain a deeper understanding of how web servers work by building one from scratch, focusing on key aspects such as:

- Handling multiple clients using multiplexing (select, poll, or epoll)
- Supporting various HTTP methods like `GET`, `POST`, and `DELETE`
- Configurable virtual hosts
- CGI support for dynamic content

## Features

- HTTP/1.1 support for basic methods (`GET`, `POST`, `DELETE`)
- Multiple virtual hosts with customizable configurations
- Static file serving (HTML, CSS, JS, etc.)
- Dynamic content via CGI
- Basic Error Handling (404, 500, etc.)
- Connection management using multiplexing techniques (epoll)
- Configurable timeouts and limits for client requests

## How It Works

Webserv listens on a specified port and waits for incoming HTTP requests. Based on the configuration file, it determines which virtual host and route to serve, and whether to return a static file or delegate the request to a CGI script.
Key Components:

**1. Request Parsing:** Reads and parses HTTP requests to extract the method, path, headers, and body.

**2. Response Handling:** Sends back appropriate responses, including headers, status codes, and content.

**3. CGI Execution:** Executes external scripts (p.e., Python, PHP) and sends their output back to the client.

**4. Multiplexing:** Manages multiple client connections efficiently.


## Installation

Clone the repository:

```
git clone https://github.com/DinaGala/42_webserv.git webserv
cd webserv
```

Compile the project:

```
make
```
Ensure you have Python or another scripting language for CGI, if you want to test dynamic content.

## Usage

Once compiled, you can start the web server by running:

```
./webserv
```

If you want execute it with the default configuration file, or:

```
./webserv <path/to/configuration/file>
```

If you have something more specific in mind (p.e., `./conf/gpt.conf`).
