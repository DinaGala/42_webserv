const fs = require('fs');

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
	<p>${message}</p>
</body>
</html>`;
}

// Attempt to create and write to the file
try {
	fs.writeFileSync('hello_world.txt', 'Hello, World!');

	const htmlResponse = generateHtmlResponse('201 Created', "File created: hello_world.txt.");
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