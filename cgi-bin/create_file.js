const fs = require('fs');

// Function to generate HTML response
function generateHtmlResponse(status, message) {
    return `
<!DOCTYPE html>
<html>
<head>
    <title>CGI Script Response</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        h1 {
            color: #333;
        }
        p {
            font-size: 18px;
        }
        .status {
            font-weight: bold;
        }
    </style>
</head>
<body>
    <h1>CGI Script Response</h1>
    <p class="status">Status: ${status}</p>
    <p>${message}</p>
</body>
</html>`;
}

// Attempt to create and write to the file
try {
    // Create and open a file in write mode
    fs.writeFileSync('hello_world.txt', 'Hello, World!');
    
    // If the file is successfully created
    const htmlResponse = generateHtmlResponse('201 Created', "File 'hello_world.txt' created and 'Hello, World!' written to it.");
    console.log("HTTP/1.1 200 OK\r");
    console.log("Content-Type: text/html\r");
    console.log(`Content-Length: ${Buffer.byteLength(htmlResponse)}\r\n`);
    console.log(htmlResponse);
} catch (err) {
    // Handle any exception that occurs
    const htmlResponse = generateHtmlResponse('500 Internal Server Error', `Error: ${err.message}`);
    console.log("HTTP/1.1 500 Internal Server Error\r");
    console.log("Content-Type: text/html\r");
    console.log(`Content-Length: ${Buffer.byteLength(htmlResponse)}\r\n`);
    console.log(htmlResponse);
}