\newpage

# All functions

## Security

After you create the `pgmoneta_ext` extension using the `postgres` role, you can test all functions as shown below. By default, `EXECUTE` privilege on each function is revoked for every user (except `SUPERUSER`).

### Creating new role

It is recommended to create a dedicated role (for example, `repl`) to execute functions provided by the extension. A new role can be created with:

```
CREATE ROLE repl WITH LOGIN REPLICATION PASSWORD 'secretpassword';
```

The `REPLICATION` attribute is optional. It may be useful if you also use pgmoneta for replication, but for most use cases, a simple `LOGIN` role is sufficient.

### Function execution privilege

To allow a role to execute an extension function, a superuser must explicitly grant the `EXECUTE` privilege. For example, to allow the role `repl` to call the function `pgmoneta_ext_get_files(text)`, run:

```
GRANT EXECUTE ON FUNCTION pgmoneta_ext_get_files(text) TO repl;
```

or in general

```
GRANT EXECUTE ON FUNCTION <pgmoneta_ext_function_name>(<signature>) TO <role>;
```

Note: When granting `EXECUTE` privilege, you must include the function’s exact signature (name and argument types).

### Function role-based privileges

Some functions require specific predefined PostgreSQL privileges or roles (as indicated in the `Privilege` column below). If you log in with a role that lacks the required privileges, the function will simply gets terminated with an error message. If you have the necessary privileges, it will work as expected. 

A superuser can grant predefined roles to the role that are required to execute a function. For example, to allow `repl` to run function `pgmoneta_ext_checkpoint()`, first make sure `repl` has the `EXECUTION` privilege for this function, then run the following command:

```
GRANT pg_checkpoint To repl;
```

or in general,

```
GRANT <required_role> TO <role>;
```

Note: For a specific function the `<required_role>` must be in the set of the roles in the Privilege column below, that have access to execute that function.

## Extension functions

| Function                    | Privilege | Parameters | Description                                            |
|-----------------------------|-----------|------------|--------------------------------------------------------|
| `pgmoneta_ext_version()`    |   Default |    None    | Return the version number of `pgmoneta_ext` as a Datum.|
| `pgmoneta_ext_switch_wal()` | SUPERUSER |    None    | A function for switching to a new WAL file.            |
| `pgmoneta_ext_checkpoint()` | SUPERUSER <br>pg_checkpoint | None | A function which forces a checkpoint. <br>This function can only be executed by a `SUPERUSER` for all PostgreSQL versions but can also be executed by `pg_checkpoint` in PostgreSQL 15+. <br>Use the SQL command `GRANT pg_checkpoint TO repl;` to assign the role in PostgreSQL 15+.|
| `pgmoneta_ext_get_oid()`|   SUPERUSER <br>pg_read_all_data        | dbname  | Return the specific database OID by the database name. <br>This function can only be executed by a `SUPERUSER` for all PostgreSQL versions but can also be executed by `pg_read_all_data` in PostgreSQL 14+. <br>Use the SQL command `GRANT pg_read_all_data TO repl;` to assign the role in PostgreSQL 14+|
| `pgmoneta_ext_get_oids()`    |   SUPERUSER <br>pg_read_all_data    | None   | Return all OIDs on the current server. <br>This function can only be executed by a `SUPERUSER` for all PostgreSQL versions but can also be executed by `pg_read_all_data` in PostgreSQL 14+. <br>Use the SQL command `GRANT pg_read_all_data TO repl;` to assign the role in PostgreSQL 14+|
| `pgmoneta_ext_get_file()`|   SUPERUSER <br>pg_read_server_files       | path/to/file  | Return the bytes of the specified file that is passed in. <br>This function can only be executed by a `SUPERUSER` for all PostgreSQL versions but can also be executed by `pg_read_server_files` in PostgreSQL 11+. <br>Use the SQL command `GRANT pg_read_server_files TO repl;` to assign the role in PostgreSQL 11+|
| `pgmoneta_ext_get_files()`    |   SUPERUSER <br>pg_read_server_files    | path/to/dir   | Return all file and sub directory paths along with some metadata in the specified directory passed in. <br>This function can only be executed by a `SUPERUSER` for all PostgreSQL versions but can also be executed by `pg_read_server_files` in PostgreSQL 11+. <br>Use the SQL command `GRANT pg_read_server_files TO repl;` to assign the role in PostgreSQL 11+|
| `pgmoneta_ext_receive_file_chunk()`    |   SUPERUSER <br>pg_write_server_files    | data_chunk <br>path/to/file   | Receive the file chunk from the client side and write it to the file. <br>This function can only be executed by a `SUPERUSER` for all PostgreSQL versions but can also be executed by `pg_write_server_files` in PostgreSQL 11+. <br>Use the SQL command `GRANT pg_write_server_files TO repl;` to assign the role in PostgreSQL 11+|
| `pgmoneta_ext_promote()`    |   SUPERUSER    | None   | Promote a standby (replica) server to become the primary server. This function can only be executed on a server running in recovery mode.|
