CREATE FUNCTION pgmoneta_ext_version() RETURNS text 
AS 'MODULE_PATHNAME' 
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_switch_wal(OUT success bool,
                                        OUT value text
) 
RETURNS record 
AS 'MODULE_PATHNAME' 
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_checkpoint(OUT success bool,
                                        OUT value text
) 
RETURNS record 
AS 'MODULE_PATHNAME' 
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_get_oid(dbname text) RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_get_oids() RETURNS SETOF RECORD
AS 'MODULE_PATHNAME'
LANGUAGE C;

CREATE FUNCTION pgmoneta_ext_get_file(file_path text) RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_get_files(file_path text) RETURNS text[]
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION pgmoneta_ext_full_backup() RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;
