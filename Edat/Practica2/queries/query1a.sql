SELECT COUNT(*)
FROM edition
WHERE isbn IN (SELECT isbn FROM edition WHERE title = 'Don Quixote');