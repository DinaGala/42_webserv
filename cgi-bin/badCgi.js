const body = "Hello, JavaScript CGI works!!!";

console.log("HTTP/1.1 200 OK\r");
console.log("Content-Type: text/html\r");
console.log("Content-Length: " + body.length + "\r" + nonExistentVariable);
console.log("\r");
console.log(body);