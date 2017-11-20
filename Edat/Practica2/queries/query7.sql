SELECT SUM(amount)
FROM edition NATURAL JOIN purchase
WHERE language_ = 'Francés';