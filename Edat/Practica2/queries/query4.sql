SELECT SUM(amount)
FROM edition NATURAL JOIN purchase
WHERE author = 'Arturo Perez-Reverte';