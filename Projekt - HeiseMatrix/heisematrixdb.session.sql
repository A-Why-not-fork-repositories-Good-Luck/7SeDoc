-- @block
CREATE TABLE `User`(
    `user_id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `username` VARCHAR(255) NOT NULL,
    `password` VARCHAR(255) NOT NULL,
    `mail` VARCHAR(255) NULL,
    `phone_number` VARCHAR(255) NULL
);
CREATE TABLE `Workshop`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `thema_id` INT UNSIGNED NOT NULL,
    `name` VARCHAR(255) NOT NULL
);
CREATE TABLE `Participation_disc`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `user_id` INT UNSIGNED NOT NULL,
    `discussion_id` INT UNSIGNED NOT NULL
);
CREATE TABLE `Discussion`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `thema_id` INT UNSIGNED NOT NULL,
    `topic` VARCHAR(255) NOT NULL
);
CREATE TABLE `Thema`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(255) NOT NULL
);
CREATE TABLE `Participation_work`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `user_id` INT UNSIGNED NOT NULL,
    `workshop_id` INT UNSIGNED NOT NULL
);

ALTER TABLE
    `Participation_work` ADD CONSTRAINT `participation_work_user_id_foreign` FOREIGN KEY(`user_id`) REFERENCES `User`(`user_id`);
ALTER TABLE
    `Participation_work` ADD CONSTRAINT `participation_work_workshop_id_foreign` FOREIGN KEY(`workshop_id`) REFERENCES `Workshop`(`id`);
ALTER TABLE
    `Workshop` ADD CONSTRAINT `workshop_thema_id_foreign` FOREIGN KEY(`thema_id`) REFERENCES `Thema`(`id`);
ALTER TABLE
    `Participation_disc` ADD CONSTRAINT `participation_disc_user_id_foreign` FOREIGN KEY(`user_id`) REFERENCES `User`(`user_id`);
ALTER TABLE
    `Participation_disc` ADD CONSTRAINT `participation_disc_discussion_id_foreign` FOREIGN KEY(`discussion_id`) REFERENCES `Discussion`(`id`);
ALTER TABLE
    `Discussion` ADD CONSTRAINT `discussion_thema_id_foreign` FOREIGN KEY(`thema_id`) REFERENCES `Thema`(`id`);


-- @block
DROP TABLE Participation_disc;
DROP TABLE Participation_work;
DROP TABLE Discussion;
DROP TABLE Workshop;
DROP TABLE Thema;
DROP TABLE user;

-- @block
INSERT INTO user(username, password) VALUES('lasse', '1234');
INSERT INTO thema(name) VALUES('C++');
INSERT INTO Workshop(thema_id, name) VALUES(1, 'C++20 new features');
INSERT INTO Participation_work(user_id, workshop_id) VALUES(1, 1);
INSERT INTO Discussion(thema_id, topic) VALUES(1, 'Hello World in C++?');
INSERT INTO Participation_disc(user_id, discussion_id) VALUES(1, 1);

-- @block
SELECT * from user;
SELECT * from Thema;
SELECT * from Workshop;
SELECT * from Discussion;
SELECT * from Participation_disc;
SELECT * from Participation_work;

-- @block
-- Alle User mit den Discussionen und dem Thema in welchen die Discussion enthalten sind
SELECT u.user_id, u.username, d.id as Discussion_id, d.topic as Discussion, t.id as Thema_id, t.name as Thema FROM user u 
JOIN Participation_disc p ON u.user_id = p.user_id
JOIN discussion d ON d.id = p.discussion_id
JOIN thema t ON t.id = d.thema_id;


 