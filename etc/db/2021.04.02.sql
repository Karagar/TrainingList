create table department (
    department_id int NOT NULL AUTO_INCREMENT,
    name VARCHAR(255),
    PRIMARY KEY (department_id),
    UNIQUE(name)
);

INSERT INTO department (name) VALUES ('IT');
INSERT INTO department (name) VALUES ('HR');
INSERT INTO department (name) VALUES ('HelpDesk');
INSERT INTO department (name) VALUES ('Directors');
INSERT INTO department (name) VALUES ('Кандидаты');
UPDATE department SET department_id = 0 WHERE name = 'Кандидаты';

create table position (
    position_id int NOT NULL AUTO_INCREMENT,
    department_id int NOT NULL,
    name VARCHAR(255) NOT NULL,
    PRIMARY KEY (position_id),
    UNIQUE(department_id, name)
);

INSERT INTO position (department_id, name) VALUES (4, 'Технический директор');
INSERT INTO position (department_id, name) VALUES (3, 'Заведующий хозяйственной частью');
INSERT INTO position (department_id, name) VALUES (2, 'Начальник отдела');
INSERT INTO position (department_id, name) VALUES (1, 'Ведущий специалист');
INSERT INTO position (department_id, name) VALUES (0, 'Кандидат');
UPDATE position SET position_id = 0 WHERE name = 'Кандидат';

create table employee (
    employee_id int NOT NULL AUTO_INCREMENT,
    position_id int NOT NULL,
    name VARCHAR(255) NOT NULL,
    PRIMARY KEY (employee_id),
    UNIQUE(position_id, name)
);

INSERT INTO employee (position_id, name) VALUES (4, 'Дартаньян');
INSERT INTO employee (position_id, name) VALUES (3, 'Атос');
INSERT INTO employee (position_id, name) VALUES (2, 'Портос');
INSERT INTO employee (position_id, name) VALUES (1, 'Арамис');

create table skill_group (
    skill_group_id int NOT NULL AUTO_INCREMENT,
    name VARCHAR(255),
    PRIMARY KEY (skill_group_id),
    UNIQUE(name)
);

INSERT INTO skill_group (name) VALUES ('Сетевые технологии');
INSERT INTO skill_group (name) VALUES ('Soft');
INSERT INTO skill_group (name) VALUES ('IT');

create table skill (
    skill_id int NOT NULL AUTO_INCREMENT,
    skill_group_id int NOT NULL,
    name VARCHAR(255) NOT NULL,
    PRIMARY KEY (skill_id),
    UNIQUE(skill_group_id, name)
);

INSERT INTO skill (skill_group_id, name) VALUES (3, 'Программирование на Go');
INSERT INTO skill (skill_group_id, name) VALUES (3, 'React JS');
INSERT INTO skill (skill_group_id, name) VALUES (2, 'Конфликтология');
INSERT INTO skill (skill_group_id, name) VALUES (1, 'Обжим витой пары');
INSERT INTO skill (skill_group_id, name) VALUES (3, 'Базовое программирование');
INSERT INTO skill (skill_group_id, name) VALUES (1, 'Сетевые утилиты Linux');

create table course (
    course_id int NOT NULL AUTO_INCREMENT,
    name VARCHAR(255),
    duration int DEFAULT 7,
    PRIMARY KEY (course_id),
    UNIQUE(name)
);

INSERT INTO course (name, duration) VALUES ('Обучение программированию на Golang', 50);
INSERT INTO course (name, duration) VALUES ('Обучение программированию на React JS', 40);
INSERT INTO course (name, duration) VALUES ('Обучение сетевым утилитам Linux', 200);
INSERT INTO course (name, duration) VALUES ('Конфликтология', 20);

create table requirement_skill (
    requirement_skill_id int NOT NULL AUTO_INCREMENT,
    course_id int NOT NULL,
    skill_id int NOT NULL,
    PRIMARY KEY (requirement_skill_id),
    UNIQUE(course_id, skill_id)
);

INSERT INTO requirement_skill (course_id, skill_id) VALUES (1, 5);
INSERT INTO requirement_skill (course_id, skill_id) VALUES (2, 5);
INSERT INTO requirement_skill (course_id, skill_id) VALUES (3, 5);

create table received_skill (
    received_skill_id int NOT NULL AUTO_INCREMENT,
    course_id int NOT NULL,
    skill_id int NOT NULL,
    PRIMARY KEY (received_skill_id),
    UNIQUE(course_id, skill_id)
);

INSERT INTO received_skill (course_id, skill_id) VALUES (1, 1);
INSERT INTO received_skill (course_id, skill_id) VALUES (2, 2);
INSERT INTO received_skill (course_id, skill_id) VALUES (3, 6);
INSERT INTO received_skill (course_id, skill_id) VALUES (4, 3);

create table employee_skill (
    employee_skill_id int NOT NULL AUTO_INCREMENT,
    employee_id int NOT NULL,
    skill_id int NOT NULL,
    PRIMARY KEY (employee_skill_id),
    UNIQUE(employee_id, skill_id)
);

INSERT INTO employee_skill (employee_id, skill_id) VALUES (1, 1);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (1, 2);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (1, 3);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (1, 4);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (1, 5);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (2, 3);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (3, 3);
INSERT INTO employee_skill (employee_id, skill_id) VALUES (4, 3);

create table position_skill (
    position_skill_id int NOT NULL AUTO_INCREMENT,
    position_id int NOT NULL,
    skill_id int NOT NULL,
    PRIMARY KEY (position_skill_id),
    UNIQUE(position_id, skill_id)
);

INSERT INTO position_skill (position_id, skill_id) VALUES (1, 3);
INSERT INTO position_skill (position_id, skill_id) VALUES (2, 3);
INSERT INTO position_skill (position_id, skill_id) VALUES (3, 3);
INSERT INTO position_skill (position_id, skill_id) VALUES (4, 1);
INSERT INTO position_skill (position_id, skill_id) VALUES (4, 3);