CREATE FUNCTION pgmoneta_version_ext() RETURNS text 
AS 'MODULE_PATHNAME' 
LANGUAGE C STRICT;