ALTER VIEW viewObjectTags AS
SELECT
	ot.*,
	o.*,
    t.*
FROM
	objet_tag AS ot
    JOIN object AS o ON ot.fkObject = o.ObjectId
    JOIN tag AS t ON ot.fkTag = t.TagId