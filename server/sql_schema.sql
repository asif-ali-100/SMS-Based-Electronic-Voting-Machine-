CREATE DATABASE IF NOT EXISTS voting_db;
USE voting_db;

CREATE TABLE IF NOT EXISTS votes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    mobile VARCHAR(20) NOT NULL,
    candidate INT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Optional: table to track registered voters
CREATE TABLE IF NOT EXISTS registered_voters (
    id INT AUTO_INCREMENT PRIMARY KEY,
    mobile VARCHAR(20) NOT NULL UNIQUE,
    has_voted BOOLEAN DEFAULT FALSE
);
