INSERT INTO sale (saleid, start_date, end_date, discount) VALUES (666, '2017-01-01', '2017-01-31', 20);
INSERT INTO sale_isbn (saleid, isbn) VALUES (666, '1941999174');
UPDATE purchase SET saleid = 666 WHERE isbn = '1941999174';

SELECT COUNT(*)
FROM edition NATURAL JOIN purchase
WHERE author = 'Arturo Perez-Reverte' AND saleID IS NOT NULL;