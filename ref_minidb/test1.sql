CREATE DATABASE university_db;
USE DATABASE university_db;
CREATE TABLE student (
    ID INTEGER,
    Name TEXT,
    GPA FLOAT,
    Major TEXT
);
INSERT INTO student VALUES (1000, 'Jay Chou', 3.0, 'Microelectronics');
INSERT INTO student VALUES (1001, 'Taylor Swift', 3.2, 'Data Science');
INSERT INTO student VALUES (1002, 'Bob Dylan', 3.5, 'Financial Technology');
SELECT * FROM student;
SELECT Name, GPA FROM student WHERE GPA > 3.1;
UPDATE student SET GPA = 3.8 WHERE ID = 1001;
DELETE FROM student WHERE ID = 1000;
SELECT * FROM student;
USE DATABASE university_db;
CREATE TABLE course_enrollment (
    StudentID INTEGER,
    Course TEXT
);
INSERT INTO course_enrollment VALUES (1001, 'Data Science');
INSERT INTO course_enrollment VALUES (1001, 'Machine Learning');
INSERT INTO course_enrollment VALUES (1002, 'Financial Technology');
INSERT INTO course_enrollment VALUES (1002, 'Advanced Algorithms');
SELECT student.Name, course_enrollment.Course
FROM student
INNER JOIN course_enrollment ON student.ID = course_enrollment.StudentID;
DROP TABLE student;