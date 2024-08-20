// Function to generate the HTML content with the current time
function generateHtml() {
  const now = new Date();
  const formattedTime = now.toLocaleString('en-US', {
    timeZone: 'UTC',
    weekday: 'long',
    year: 'numeric',
    month: 'long',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
  });

  return `
  <!DOCTYPE html>
  <html>
  <head>
    <title>Current Server Time</title>
  </head>
  <body>
    <h1>Current Server Time</h1>
    <p>The current server time (UTC) is:</p>
    <p>${formattedTime}</p>
  </body>
  </html>
  `;
}

// Main function to handle CGI output
function main() {
  // Generate the HTML content
  const htmlContent = generateHtml();
  console.log(`HTTP/1.1 200 OK\r`);
  console.log(`Content-Type: text/html\r`);
  console.log(`Content-Length: ` + htmlContent.length + `\r\n\r`);
  console.log(htmlContent);

}

// Execute the main function
main();
