// cgiScript.js
const querystring = require('querystring');

// Read the QUERY_STRING environment variable
const queryString = process.env.QUERY_STRING;

// Parse the query string into an object
const queryParams = querystring.parse(queryString);

// Generate HTML response based on query parameters
let htmlResponse = `
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
        table {
            border-collapse: collapse;
            width: 50%;
            margin-top: 20px;
        }
        table, th, td {
            border: 1px solid #ddd;
            padding: 8px;
        }
        th {
            background-color: #f2f2f2;
            text-align: left;
        }
    </style>
</head>
<body>
    <h1>CGI Script Response</h1>
    <p>Query Parameters Received:</p>
    <table>
        <tr>
            <th>Parameter</th>
            <th>Value</th>
        </tr>`;

for (const [key, value] of Object.entries(queryParams)) {
    htmlResponse += `<tr>
            <td>${key}</td>
            <td>${value}</td>
        </tr>`;
}

htmlResponse += `
    </table>
</body>
</html>`;

// Output the HTTP headers followed by the HTML response
console.log(`HTTP/1.1 200 OK\r`);
console.log(`Content-Type: text/html\r`);
console.log(`Content-Length: ` + htmlResponse.length + `\r\n\r`);
console.log(htmlResponse);

