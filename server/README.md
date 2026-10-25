# Server (XAMPP + PHP + MySQL)

Backend that receives votes from the ESP32 and displays live results.

## Files
- `db.php` — Opens the MySQL connection using credentials from `config.php`.
- `config.example.php` — Template for DB credentials. **Copy to `config.php` and fill in your real values.**
- `vote.php` — API endpoint the ESP32 POSTs to when a vote is cast.
- `results.php` — Live-updating dashboard showing vote totals per candidate.
- `sql_schema.sql` — Creates the `voting_db` database and required tables.

## Setup
1. Install [XAMPP](https://www.apachefriends.org/) and start Apache + MySQL from the control panel.
2. Import the schema via phpMyAdmin (`http://localhost/phpmyadmin`), or run:
   ```bash
   mysql -u root -p < sql_schema.sql
   ```
3. Copy this `server/` folder into `htdocs/`, e.g.:
   ```
   C:\xampp\htdocs\SecureVotingMachine\server
   ```
4. **Create your local config** (this file is gitignored, so credentials never get committed):
   ```bash
   cp config.example.php config.php
   ```
   Edit `config.php` with your real MySQL username/password if different from XAMPP's defaults (`root` / empty password).
5. Test in a browser: `http://localhost/SecureVotingMachine/server/results.php` — you should see an empty results table.
6. On the ESP32, set `SERVER_URL` in `esp32/include/config.h` to:
   ```
   http://<your-PC-IP-on-the-network>/SecureVotingMachine/server/vote.php
   ```
   Use your PC's LAN IP (e.g. `192.168.1.100`), not `localhost`, since the ESP32 is a separate device on the network.

## Notes
- `vote.php` uses a prepared statement to prevent SQL injection.
- For production use, add authentication to `results.php` so only admins can view it, and serve everything over HTTPS.
- If `results.php` shows a connection error, double check `config.php` exists and has the right credentials — it's not created automatically.
