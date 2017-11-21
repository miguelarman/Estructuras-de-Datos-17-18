SELECT COUNT(DISTINCT(userID))
FROM edition NATURAL JOIN purchase
WHERE userID IS NOT NULL AND language_ = 'Inglés';