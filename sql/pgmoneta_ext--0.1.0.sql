CREATE OR REPLACE FUNCTION pgmoneta_ext_version() RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION pgmoneta_ext_switch_wal(OUT success bool,
                                        OUT value text
)
RETURNS record
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_switch_wal(OUT success bool,
                                        OUT value text
) FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_checkpoint(OUT success bool,
                                        OUT value text
)
RETURNS record
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_checkpoint(OUT success bool,
                                        OUT value text
) FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_get_oid(dbname text) RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_get_oid(dbname text) FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_get_oids() RETURNS SETOF RECORD
AS 'MODULE_PATHNAME'
LANGUAGE C;

REVOKE ALL ON FUNCTION pgmoneta_ext_get_oids() FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_get_file(file_path text) RETURNS text
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_get_file(file_path text) FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_get_files(file_path text, OUT name text, OUT is_dir boolean, OUT size bigint) RETURNS SETOF record
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_get_files(file_path text, OUT name text, OUT is_dir boolean, OUT size bigint) FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_receive_file_chunk(base64_chunk text, file_path text) RETURNS int
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_receive_file_chunk(base64_chunk text, file_path text) FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_promote() RETURNS BOOLEAN
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_promote() FROM PUBLIC;

CREATE OR REPLACE FUNCTION pgmoneta_ext_set_guc(
   parameter text,
   value text,
   reload boolean DEFAULT true,
   OUT guc_param text,
   OUT old_value text,
   OUT new_value text,
   OUT restart_required boolean
)
RETURNS record
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

REVOKE ALL ON FUNCTION pgmoneta_ext_set_guc(text, text, boolean, OUT text, OUT text, OUT text, OUT boolean) FROM PUBLIC;
