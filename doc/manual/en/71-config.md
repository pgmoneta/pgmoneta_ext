## Configuration

Our project, `pgmoneta_ext`, is primarily designed for `pgmoneta` to perform delta backups. You also need to install [`pgmoneta`][pgmoneta].

To ensure `pgmoneta_ext` functions properly, we should use the `repl` role to test all extension functions and verify they work correctly with `pgmoneta`. Therefore, we need to make sure that all necessary configurations for `repl` are in place.

**If you have already set up the `repl` role, you can skip sections 1-8.**

### 1. Remove default access

Remove last lines from `/tmp/pgsql/pg_hba.conf`. The path mentioned can be found using the SQL command `SHOW hba_file;`.

``` ini
host    all             all             127.0.0.1/32            trust
host    all             all             ::1/128                 trust
host    replication     all             127.0.0.1/32            trust
host    replication     all             ::1/128                 trust
```

### 2. Add access for users and a database

Add new lines to `/tmp/pgsql/pg_hba.conf`

``` ini
host    postgres         repl            127.0.0.1/32            scram-sha-256
host    postgres         repl            ::1/128                 scram-sha-256
host    replication      repl            127.0.0.1/32            scram-sha-256
host    replication      repl            ::1/128                 scram-sha-256
```

### 3. Set password_encryption

Set `password_encryption` value in `/tmp/pgsql/postgresql.conf` to be `scram-sha-256`

``` sh
password_encryption = scram-sha-256
```

### 4. Set replication level

Set wal_level value in `/tmp/pgsql/postgresql.conf` to be `replica`

``` sh
wal_level = replica
```

### 5. Start PostgreSQL

``` sh
pg_ctl  -D /tmp/pgsql/ start
```

Here, you may encounter issues such as the port being occupied or permission being denied. If you experience a failure, you can go to `/tmp/pgsql/log` to check the reason.

You can use

``` sh
pg_isready
```

to test

### 6. Add role `repl`

``` sh
psql postgres
CREATE ROLE repl WITH LOGIN REPLICATION PASSWORD 'secretpassword';
\q
```

### 7. Add replication slot

Add the required replication slot

``` sh
psql postgres
SELECT pg_create_physical_replication_slot('repl', true, false);
\q
```

Here, the command in the context of `pgmoneta` is used to create a replication slot to manage and retain WAL segments necessary for replication and backup operations, ensuring data consistency and reliability.

For more details, please check the [pgmoneta][pgmoneta].

### 8. Verify access

For the role `repl` (pgmoneta) use `secretpassword`

``` sh
psql -h localhost -p 5432 -U repl postgres
\q
```

### 9. Grant superuser privileges

Since some functions require the role to have `SUPERUSER` privileges, you can grant these privileges to the `repl` role using the following SQL command. However, please be cautious, as this will give the role full access and control over the database, posing potential security risks.

``` sql
ALTER ROLE repl WITH SUPERUSER;
```

The following SQL command can revoke the superuser privileges:

``` sql
ALTER USER repl WITH NOSUPERUSER;
```

### 10. Check

If you install `pgmoneta_ext` through the repo using yum, before you start, you need to add the `pgmoneta_ext` library to `postgresql.conf` as well:

```ini
shared_preload_libraries = 'pgmoneta_ext'
```

And remember to restart PostgreSQL to make it work.

If you just want to verify if `pgmoneta_ext` was installed successfully, you can use the `postgres` role to test.

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

If you encounter any errors, you can check the log to find the reason. The log's location can be found in the `postgresql.conf` file.

``` ini
# These are only used if logging_collector is on:
log_directory = 'log'                   # directory where log files are written,
                                        # can be absolute or relative to PGDATA
log_filename = 'postgresql-%a.log'      # log file name pattern,
                                        # can include strftime() escapes
```
