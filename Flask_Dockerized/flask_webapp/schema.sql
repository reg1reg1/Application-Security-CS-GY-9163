 
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
