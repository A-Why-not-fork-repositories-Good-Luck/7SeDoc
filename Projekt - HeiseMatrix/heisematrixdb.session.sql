-- @block
CREATE TABLE `user`(
    `user_id` VARCHAR(255) NOT NULL PRIMARY KEY,
    `username` VARCHAR(255) NOT NULL 
);
CREATE TABLE `workshop`(
    `workshop_id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `thema` INT UNSIGNED NULL,
    `name` VARCHAR(255) NULL
);
CREATE TABLE `participation_disc`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `user` VARCHAR(255) NOT NULL,
    `discussion` VARCHAR(255) NOT NULL
);
CREATE TABLE `discussion`(
    `discussion_id` VARCHAR(255) NOT NULL PRIMARY KEY,
    `thema` INT UNSIGNED NULL,
    `question` VARCHAR(255) NULL,
    `creator` VARCHAR(255) NOT NULL
);
CREATE TABLE `thema`(
    `thema_id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(255) NOT NULL
);
ALTER TABLE
    `thema` ADD UNIQUE `thema_name_unique`(`name`);
CREATE TABLE `participation_work`(
    `id` INT UNSIGNED PRIMARY KEY NOT NULL AUTO_INCREMENT,
    `user` VARCHAR(255) NOT NULL,
    `workshop` INT UNSIGNED NOT NULL
);

ALTER TABLE
    `participation_work` ADD CONSTRAINT `participation_work_user_foreign` FOREIGN KEY(`user`) REFERENCES `user`(`user_id`);
ALTER TABLE
    `participation_work` ADD CONSTRAINT `participation_work_workshop_foreign` FOREIGN KEY(`workshop`) REFERENCES `workshop`(`workshop_id`);
ALTER TABLE
    `workshop` ADD CONSTRAINT `workshop_thema_foreign` FOREIGN KEY(`thema`) REFERENCES `thema`(`thema_id`);
ALTER TABLE
    `participation_disc` ADD CONSTRAINT `participation_disc_user_foreign` FOREIGN KEY(`user`) REFERENCES `user`(`user_id`);
ALTER TABLE
    `participation_disc` ADD CONSTRAINT `participation_disc_discussion_foreign` FOREIGN KEY(`discussion`) REFERENCES `discussion`(`discussion_id`);
ALTER TABLE
    `discussion` ADD CONSTRAINT `discussion_thema_foreign` FOREIGN KEY(`thema`) REFERENCES `thema`(`thema_id`);
ALTER TABLE
    `discussion` ADD CONSTRAINT `discussion_creator_foreign` FOREIGN KEY(`creator`) REFERENCES `user`(`user_id`);


-- @block
DROP TABLE participation_disc;
DROP TABLE participation_work;
DROP TABLE discussion;
DROP TABLE workshop;
DROP TABLE thema;
DROP TABLE user;

-- @block
SELECT * from user;
SELECT * from thema;
SELECT * from workshop;
SELECT * from discussion;
SELECT * from participation_disc;
SELECT * from participation_work;

 