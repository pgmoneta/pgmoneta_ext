\newpage

# All functions

## Security

After you create the extension `pgmoneta_ext` using the `postgres` role, you can test all functions as shown below. Some functions may require superuser privileges (For the column `Superuser`, the value is `true`), so if you log in with a role without superuser privileges, the function will return `false`. If you have superuser privileges, it will work as desired.

## Extension functions

| Function                    | Superuser | Description                                            |
|-----------------------------|-----------|--------------------------------------------------------|
| `pgmoneta_ext_version()`    |   false   | Return the version number of `pgmoneta_ext` as a Datum.|
| `pgmoneta_ext_switch_wal()` |   true    | A function for switching to a new WAL file.            |
