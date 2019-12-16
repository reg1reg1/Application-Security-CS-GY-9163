
DROP TABLE IF EXISTS user;
DROP TABLE IF EXISTS queries;
DROP TABLE IF EXISTS logs;
CREATE TABLE user (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username TEXT UNIQUE NOT NULL,
  password TEXT NOT NULL,
  phone TEXT NOT NULL,
  isAdmin BOOLEAN NOT NULL
);

CREATE TABLE queries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userid TEXT NOT NULL,
    query_text TEXT,
    query_response TEXT
);

CREATE TABLE logs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userid TEXT NOT NULL,
    request TEXT NOT NULL,
    request_time TEXT NOT NULL
    
);


INSERT INTO user (username, password, phone, isAdmin)
VALUES
  ('user1', 'pbkdf2:sha256:150000$RVwv8Ig2$0f8cd25ac738722f186550531d1813502bf1581bb8b6908693443aa265e38fe8','1234567890',0),
  ('reg1','pbkdf2:sha256:150000$qhNflQu7$18ef0e2923c5e93f2ffcdf2ee5ac60a8aa269a7f3e3ce214dddc421e2e269ae0','1234567890',0)
