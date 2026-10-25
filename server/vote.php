<?php
// Receives POST requests from the ESP32 to record a vote
include 'db.php';

$mobile = $_POST['mobile'] ?? '';
$candidate = $_POST['candidate'] ?? '';

if ($mobile === '' || $candidate === '') {
    http_response_code(400);
    echo "Missing mobile or candidate parameter";
    exit;
}

// Using a prepared statement to avoid SQL injection
$stmt = $conn->prepare("INSERT INTO votes (mobile, candidate, timestamp) VALUES (?, ?, NOW())");
$stmt->bind_param("si", $mobile, $candidate);

if ($stmt->execute()) {
    echo "Vote recorded successfully";
} else {
    http_response_code(500);
    echo "Error: " . $stmt->error;
}

$stmt->close();
$conn->close();
?>
