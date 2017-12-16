INSERT INTO edition VALUES ('14774014R99', 'Don Quixote', 'Libro de bolsillo', 'CreateSpace Independent Publishing Platform', 19.99, 'Español', 0, 599, 'Miguel de Cervantes', '1998-06-26');


SELECT COUNT(DISTINCT(language_))
FROM edition
WHERE isbn IN (SELECT isbn FROM edition WHERE title = 'Don Quixote');