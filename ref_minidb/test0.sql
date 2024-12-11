CREATE DATABASE test_university;
USE DATABASE test_university;
CREATE TABLE student (
    ID INTEGER,
    Name TEXT,
    GPA FLOAT,
    Major TEXT
);
CREATE TABLE course (
    CourseID INTEGER,
    CourseName TEXT,
    Department TEXT
);
CREATE TABLE enrollment (
    StudentID INTEGER,
    CourseID INTEGER
);
INSERT INTO student VALUES (1, 'Alice Johnson', 3.5, 'Computer Science');
INSERT INTO student VALUES (2, 'Bob Smith', 3.6, 'Electrical Engineering');
INSERT INTO student VALUES (3, 'Catherine Lee', 3.9, 'Mathematics');
INSERT INTO student VALUES (4, 'Dave Brown', 3.2, 'Physics');
INSERT INTO student VALUES (5, 'Eva White', 3.8, 'Chemistry');
INSERT INTO course VALUES (101, 'Introduction to Computer Science', 'Computer Science');
INSERT INTO course VALUES (102, 'Circuit Analysis', 'Electrical Engineering');
INSERT INTO course VALUES (103, 'Linear Algebra', 'Mathematics');
INSERT INTO course VALUES (104, 'Quantum Mechanics', 'Physics');
INSERT INTO course VALUES (105, 'Organic Chemistry', 'Chemistry');
INSERT INTO enrollment VALUES (1, 101);
INSERT INTO enrollment VALUES (1, 103);
INSERT INTO enrollment VALUES (2, 102);
INSERT INTO enrollment VALUES (3, 103);
INSERT INTO enrollment VALUES (4, 104);
INSERT INTO enrollment VALUES (5, 105);
SELECT * FROM student;
SELECT * FROM course;
SELECT student.Name, enrollment.CourseID
FROM student
INNER JOIN enrollment
ON student.ID = enrollment.StudentID
WHERE student.Name = 'Alice Johnson';
UPDATE student SET GPA = 4.0 WHERE Name = 'Alice Johnson';
DELETE FROM student WHERE Name = 'Dave Brown';
SELECT * FROM student WHERE Name = 'Alice Johnson';
SELECT student.Name, enrollment.CourseID
FROM student
INNER JOIN enrollment
ON student.ID = enrollment.StudentID;
DELETE FROM course WHERE CourseName = 'Quantum Mechanics';
SELECT * FROM course;