CREATE FUNCTION pgmoneta_ext_version() RETURNS text 
AS 'MODULE_PATHNAME' 
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_switch_wal(OUT success bool,
                                        OUT value text
) 
RETURNS record 
AS 'MODULE_PATHNAME' 
LANGUAGE C STRICT;