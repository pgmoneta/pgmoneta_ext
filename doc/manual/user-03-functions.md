\newpage

# All functions

## Security

After you create the extension `pgmoneta_ext` using the `postgres` role, you can test all functions as shown below. Some functions may require specific privileges (For the column `Privilege`), so if you log in with a role without the specific privileges, the function will return `false`. If you have the specific privileges, it will work as desired.

## Extension functions

| Function                    | Privilege                    | Description                                            |
|-----------------------------|-----------|--------------------------------------------------------|
| `pgmoneta_ext_version()`    |   Default                      | Return the version number of `pgmoneta_ext` as a Datum.|
| `pgmoneta_ext_switch_wal()` | SUPERUSER                   | A function for switching to a new WAL file.            |
| `pgmoneta_ext_checkpoint()` | SUPERUSER <br>pg_checkpoint | A function which forces a checkpoint. <br>This function can only be executed by a `SUPERUSER` in PostgreSQL 13/14, but can also be executed by `pg_checkpoint` in PostgreSQL 15+. <br>You can use the SQL command `GRANT pg_checkpoint TO repl;` to assign the role in PostgreSQL 15+.|
