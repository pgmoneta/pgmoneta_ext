\newpage

# All functions

## Configuration

First, activate the extension in the database. We’ll use `testdb` as an example. You can use the following SQL command to create the database:

``` sql
CREATE DATABASE testdb;
```

Activate the extension in the `testdb` database.

``` sql
psql testdb
CREATE EXTENSION pgmoneta_ext;
```

## Extension functions

| Function                 | Description                                            |
|--------------------------|--------------------------------------------------------|
| `pgmoneta_ext_version()` | Return the version number of `pgmoneta_ext` as a Datum.|
