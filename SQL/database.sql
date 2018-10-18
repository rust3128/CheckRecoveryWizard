CREATE TABLE `connections` (
           `conn_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
           `conn_name`	VARCHAR(50) NOT NULL UNIQUE,
           `conn_host`	varchar(50) NOT NULL,
           `conn_db`	VARCHAR(100) NOT NULL,
           `conn_user`	INTEGER NOT NULL,
           `conn_pass`	INTEGER NOT NULL);

INSERT INTO `connections`(`conn_name`,`conn_host`,`conn_db`,`conn_user`,`conn_pass`)
           VALUES ('Саноил','192.168.123.11','sunoil','SYSDBA','sunoil00');
