\newpage

# All functions

## Security

After you create the `pgmoneta_ext` extension using the `postgres` role, you can test all functions as shown below. Some functions may require specific privileges (as indicated in the `Privilege` column). If you log in with a role that lacks the required privileges, the function will return `false` or display a warning. If you have the necessary privileges, it will work as expected.

We recommend using the `postgres` role to test these functions. If you want to test them using another role, you can grant the necessary privileges with the following command:

```sql
ALTER ROLE repl WITH SUPERUSER;
```

```sql
ALTER USER repl WITH NOSUPERUSER;
```

## Extension functions

| Function                    | Privilege | Parameters | Description                                            |
|-----------------------------|-----------|------------|--------------------------------------------------------|
| `pgmoneta_ext_version()`    |   Default |    None    | Return the version number of `pgmoneta_ext` as a Datum.|
| `pgmoneta_ext_switch_wal()` | SUPERUSER |    None    | A function for switching to a new WAL file.            |
| `pgmoneta_ext_checkpoint()` | SUPERUSER <br>pg_checkpoint | None | A function which forces a checkpoint. <br>This function can only be executed by a `SUPERUSER` in PostgreSQL 13/14, but can also be executed by `pg_checkpoint` in PostgreSQL 15+. <br>You can use the SQL command `GRANT pg_checkpoint TO repl;` to assign the role in PostgreSQL 15+.|
| `pgmoneta_ext_get_oid()`|   Default        | dbname  | Return the specific database OID by the database name.|
| `pgmoneta_ext_get_oids()`    |   Default    | None   | Return all OIDs on the current server.|
| `pgmoneta_ext_get_file()`|   SUPERUSER        | path/to/file  | Return the bytes of the specified file that is passed in.|
| `pgmoneta_ext_get_files()`    |   SUPERUSER    | path/to/dir   | Return all file paths in the specified directory passed in.|
| `pgmoneta_ext_receive_file_chunk()`    |   SUPERUSER    | data_chunk <br>path/to/file   | Receive the file chunk from the client side and write it to the file.|
| `pgmoneta_ext_promote()`    |   SUPERUSER    | None   | Promote a standby (replica) server to become the primary server. This function can only be executed on a server running in recovery mode.|
