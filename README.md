# Database Project
### Andrii Sysoiev s30422 Group: 30c

This is my database project for PJC course at PJATK university. In this README I will describe what commands are implemented in this project and what restrictions it has.

#### Commands
1. DATABASE - outputs all the tables in the database.
2. EXIT - saves and exits the program.
3. CREATE TABLE: CREATE TABLE car ( idCar integer primary key, model varchar(50) not null,
yearOfProduction integer not null; - creates a table and its columns.
4. INSERT INTO car (idCar, model, productionYear) VALUES (1, Golf, 2016), (2, Octavia, 2012), (3, Camaro, 2018); - fills the table with data.
5. SELECT model, productionYear FROM car WHERE productionYear < 2017 ORDER BY productionYear ASC; - allows for output of specific data in specific order.
6. SELECT * FROM driver JOIN car ON driver.idCar = car.idCar WHERE model != Octavia ORDER BY idCar DESC; - allows for joining table. Restrictions:
   1. Works only for SELECT *.
   2. The condition should be in very specific order with correct specifications, like in the example (the first table is initial table and the second one is joining table).
7. ALTER TABLE driver ADD phoneNumber integer; - allows for adding new columns to **empty** tables.
8. ALTER TABLE car DROP COLUMN productionYear; - allows for deleting tables from **empty** tables.
9. TRUNCATE TABLE car; - delete all the data from the table.
10. DROP TABLE car; - delete the table from the database.


#### Notes
Datatypes supported:
1. integer
2. varchar(%)
3. date
4. number(%,%)

**Cases**: all commands are case-insensitive. It doesn't matter if you write FROM or from (you can even write fROm and it will work).
