\newpage

# Quick start

If you install `pgmoneta_ext` through the repo using yum, before you start, you need to add the `pgmoneta_ext` library to `postgresql.conf` as well:

```ini
shared_preload_libraries = 'pgmoneta_ext'
```

And remember to restart PostgreSQL to make it work.

You can use the `postgres` role to test.

1. Log into PostgreSQL

    ``` sh
    psql
    ```

2. Create a new test database

    ``` sql
    CREATE DATABASE testdb;
    ```

3. Enter the database

    ``` sql
    \c testdb
    ```

4. Follow the SQL commands below to check the function

    ``` sql
    DROP EXTENSION IF EXISTS pgmoneta_ext;
    CREATE EXTENSION pgmoneta_ext;
    SELECT pgmoneta_ext_version();
    ```

You should see

``` console
 pgmoneta_ext_version 
----------------------
 0.1.0
(1 row)
```
