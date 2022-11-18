-- @block
CREATE TABLE `User`(
    `user_id` VARCHAR(255) NOT NULL PRIMARY KEY,
    `username` VARCHAR(255) NOT NULL 
);
CREATE TABLE `Workshop`(
    `workshop_id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `thema` INT UNSIGNED NULL,
    `name` VARCHAR(255) NULL
);
CREATE TABLE `Participation_disc`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `user` VARCHAR(255) NOT NULL,
    `discussion` VARCHAR(255) NOT NULL
);
CREATE TABLE `Discussion`(
    `discussion_id` VARCHAR(255) NOT NULL PRIMARY KEY,
    `thema` INT UNSIGNED NULL,
    `question` VARCHAR(255) NULL,
    `creator` VARCHAR(255) NOT NULL
);
CREATE TABLE `Thema`(
    `thema_id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(255) NOT NULL
);
ALTER TABLE
    `Thema` ADD UNIQUE `thema_name_unique`(`name`);
CREATE TABLE `Participation_work`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `user` VARCHAR(255) NOT NULL,
    `workshop` INT UNSIGNED NOT NULL
);

ALTER TABLE
    `Participation_work` ADD CONSTRAINT `participation_work_user_foreign` FOREIGN KEY(`user`) REFERENCES `User`(`user_id`);
ALTER TABLE
    `Participation_work` ADD CONSTRAINT `participation_work_workshop_foreign` FOREIGN KEY(`workshop`) REFERENCES `Workshop`(`workshop_id`);
ALTER TABLE
    `Workshop` ADD CONSTRAINT `workshop_thema_foreign` FOREIGN KEY(`thema`) REFERENCES `Thema`(`thema_id`);
ALTER TABLE
    `Participation_disc` ADD CONSTRAINT `participation_disc_user_foreign` FOREIGN KEY(`user`) REFERENCES `User`(`user_id`);
ALTER TABLE
    `Participation_disc` ADD CONSTRAINT `participation_disc_discussion_foreign` FOREIGN KEY(`discussion`) REFERENCES `Discussion`(`discussion_id`);
ALTER TABLE
    `Discussion` ADD CONSTRAINT `discussion_thema_foreign` FOREIGN KEY(`thema`) REFERENCES `Thema`(`thema_id`);
ALTER TABLE
    `Discussion` ADD CONSTRAINT `discussion_creator_foreign` FOREIGN KEY(`creator`) REFERENCES `User`(`user_id`);


-- @block
DROP TABLE Participation_disc;
DROP TABLE Participation_work;
DROP TABLE Discussion;
DROP TABLE Workshop;
DROP TABLE Thema;
DROP TABLE user;

-- @block
SELECT * from user;
SELECT * from Thema;
SELECT * from Workshop;
SELECT * from Discussion;
SELECT * from Participation_disc;
SELECT * from Participation_work;

 