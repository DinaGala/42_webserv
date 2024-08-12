
function setUsername() {
    const username = document.getElementById('username').value.trim();
    
    // Validate if username is not empty
    if (username === "") {
        alert("Please enter a username.");
        return;
    }

    // Set a cookie with the username that expires in 7 days
    document.cookie = `username=${encodeURIComponent(username)}; path=/; max-age=${7 * 24 * 60 * 60}; SameSite=Strict`;

    // Redirect back to the main page
    window.location.href = 'cookies.html';
}

function showCookies() {
	const cookies = document.cookie.split(';'); // Split the cookie string into individual cookies

	if (cookies.length === 1 && cookies[0] === "") {
		document.getElementById('cookies').innerHTML = "No cookies set.";
		return;
	}
	// Prepare an unordered list of cookies
	let cookieList = '<ul>';
	cookies.forEach(cookie => {
	  cookieList += `<li>${cookie.trim()}</li>`; // Trim whitespace and add to list
	});
	cookieList += '</ul>';

	// Display the list of cookies in the 'cookies' code element
	document.getElementById('cookies').innerHTML = `Current Cookies: ${cookieList}`;
}

function clearOutputCookies() {
	const cookies = document.cookie.split(';'); // Split the cookie string into individual cookies

	cookies.forEach(cookie => {
		const cookieName = cookie.split('=')[0].trim(); // Get the name of each cookie
		document.cookie = `${cookieName}=;expires=Thu, 01 Jan 1970 00:00:00 GMT;path=/`; // Set cookie expiry to a past date
	});

	//Display a message indicating all cookies have been cleared
	document.getElementById('cookies').innerHTML = "All cookies cleared.";
}