# CServer - Simple Web Server in C++

## Introduction

CServer is a simple web server application written in C++ that listens on port 8080 (or a specified port) and serves a basic website composed of `index.html` and `style.css` files. It handles HTTP GET requests and responds with the requested file using the HTTP  protocol.

## Usage

To use CServer, follow these steps:

1. Build the project using an appropriate C++ compiler.
2. Run the server executable using
        
        ./server
3. Open a web browser and navigate to http://localhost:8080 (or the specified port) to view your website.

## Safety Concerns

This server is designed for educational purposes and may not be suitable for production use. It lacks many security features typically found in production-ready web servers.

The server does not perform input validation or sanitize user inputs, which could potentially lead to security vulnerabilities.

