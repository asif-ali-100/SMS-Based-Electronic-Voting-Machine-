<?php
// Simple admin dashboard showing live vote counts per candidate
include 'db.php';

$sql = "SELECT candidate, COUNT(*) as total FROM votes GROUP BY candidate ORDER BY candidate";
$result = $conn->query($sql);
?>
<!DOCTYPE html>
<html>
<head>
    <title>Voting Results</title>
    <meta http-equiv="refresh" content="10"> <!-- auto refresh every 10s -->
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        table { border-collapse: collapse; width: 400px; }
        th, td { border: 1px solid #ccc; padding: 8px 12px; text-align: left; }
        th { background: #f4f4f4; }
    </style>
</head>
<body>
    <h2>Voting Results</h2>
    <table>
        <tr><th>Candidate</th><th>Votes</th></tr>
        <?php while ($row = $result->fetch_assoc()): ?>
        <tr>
            <td>Candidate <?php echo htmlspecialchars($row['candidate']); ?></td>
            <td><?php echo htmlspecialchars($row['total']); ?></td>
        </tr>
        <?php endwhile; ?>
    </table>
</body>
</html>
<?php
$conn->close();
?>
