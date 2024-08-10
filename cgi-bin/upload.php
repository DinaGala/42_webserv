<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_FILES['upload']) && $_FILES['upload']['error'] == 0) {
        $uploadDirectory = __DIR__ . '/html/dalailaming/';
        
        // Create uploads directory if it doesn't exist
        if (!is_dir($uploadDirectory)) {
            mkdir($uploadDirectory, 0755, true);
        }

        // Determine the new filename
        $urlPath = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
        $urlPathComponents = explode('/', trim($urlPath, '/'));
        $specificName = end($urlPathComponents) . '.jpg'; // Change this logic as needed
        
        $uploadFile = $uploadDirectory . $specificName;
        
        // Check if the uploaded file is an image
        $check = getimagesize($_FILES['upload']['tmp_name']);
        if ($check !== false) {
            // Move the uploaded file to the desired directory with the specific name
            if (move_uploaded_file($_FILES['upload']['tmp_name'], $uploadFile)) {
                echo "The file has been uploaded successfully.";
            } else {
                echo "Sorry, there was an error uploading your file.";
            }
        } else {
            echo "File is not an image.";
        }
    } else {
        echo "No file was uploaded or there was an upload error.";
    }
}
?>
