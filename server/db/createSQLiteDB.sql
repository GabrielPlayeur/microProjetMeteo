-- SQLite
CREATE TABLE stationmeteo(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    token TEXT NOT NULL UNIQUE
);

CREATE TABLE datameteo (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    station_id INTEGER,
    date TEXT,
    temperature REAL,
    pression REAL,
    pluie REAL,
    vent REAL,
    luminosite REAL,
    humidite REAL,
    FOREIGN KEY (station_id) REFERENCES stationmeteo(id)
);

INSERT INTO stationmeteo (name, token)
VALUES ("test", "test");

INSERT INTO datameteo (station_id, date, temperature, pression, pluie, vent, luminosite, humidite)
VALUES (1, "23/03/2023", 0, 0, 0, 0, 0, 0);