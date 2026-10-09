CREATE FUNCTION pgmoneta_ext_get_blocks(file_path text, blocks bigint[], block_size int) RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_get_blocks(file_path text, blocks bigint[], block_size int) FROM PUBLIC;
