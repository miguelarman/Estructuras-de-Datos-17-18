SELECT sale.start_date, sale.end_Date
FROM edition NATURAL JOIN sale_isbn NATURAL JOIN sale
WHERE edition.publisher = 'Adelphi';