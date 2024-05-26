\newpage

# Install

for Fedora 40

## Install PostgreSql

``` sh
dnf install postgresql-server
```

for Fedora 40, this will install PostgreSQL 15

## Install pgmoneta_ext

### Pre-install

#### Basic dependencies

``` sh
dnf install git gcc cmake make postgresql-server-devel
```

#### Generate user and developer guide

This process is optional. If you choose not to generate the PDF and HTML files, you can opt out of downloading these dependencies, and the process will automatically skip the generation.

1. Download dependencies

    ``` sh
    dnf install pandoc texlive-scheme-basic
    ```

2. Download Eisvogel

    Use the command `pandoc --version` to locate the user data directory. On Fedora systems, this directory is typically located at `/root/.local/share/pandoc`.

    Download the `Eisvogel` template for `pandoc`, please visit the [pandoc-latex-template][pandoctemplate] repository. For a standard installation, you can follow the steps outlined below.

    ```sh
    wget https://github.com/Wandmalfarbe/pandoc-latex-template/releases/download/2.4.2/Eisvogel-2.4.2.tar.gz
    tar -xzf Eisvogel-2.4.2.tar.gz
    mkdir /root/.local/share/pandoc # user data directory
    mkdir /root/.local/share/pandoc/templates
    mv eisvogel.latex /root/.local/share/pandoc/templates/
    ```

3. Add package for latex

    Download the additional packages required for generating PDF and HTML files.

    ```sh
    dnf install 'tex(footnote.sty)' 'tex(footnotebackref.sty)' 'tex(pagecolor.sty)' 'tex(hardwrap.sty)' 'tex(mdframed.sty)' 'tex(sourcesanspro.sty)' 'tex(ly1enc.def)' 'tex(sourcecodepro.sty)' 'tex(titling.sty)' 'tex(csquotes.sty)' 'tex(zref-abspage.sty)' 'tex(needspace.sty)'
    ```

### Initialize PostgreSQL

Open a new window, switch to the `postgres` user.

``` sh
sudo su -
su - postgres
```

For Fedora, if you use `dnf` to install your postgresql, chances are the binary file is in `/usr/bin/`

``` sh
export PATH=/usr/bin:$PATH
initdb /tmp/pgsql
```

Since we initialized the database in `/tmp`, the data in this directory might be removed after you go offline, depending on your OS configuration. If you want to make it permanent, choose a different directory.

### Build

``` sh
git clone https://github.com/pgmoneta/pgmoneta_ext
cd pgmoneta_ext
mkdir build
cd build
cmake ..
make
sudo make install
```

This will install `pgmoneta_ext`.

### Configuration

Our project, `pgmoneta_ext`, is primarily designed for `pgmoneta` to perform delta backups. To ensure `pgmoneta_ext` functions properly, we should use `repl` to test all extension functions and verify they work correctly with `pgmoneta`.

**If you have already set up the `repl` role, you can skip this section.**

#### 1. Remove default access

Remove last lines from `/tmp/pgsql/pg_hba.conf`. The path mentioned can be found using the SQL command `SHOW hba_file;`.

``` ini
host    all             all             127.0.0.1/32            trust
host    all             all             ::1/128                 trust
host    replication     all             127.0.0.1/32            trust
host    replication     all             ::1/128                 trust
```

#### 2. Add access for users and a database

Add new lines to `/tmp/pgsql/pg_hba.conf`

``` ini
host    postgres         repl            127.0.0.1/32            scram-sha-256
host    postgres         repl            ::1/128                 scram-sha-256
host    replication      repl            127.0.0.1/32            scram-sha-256
host    replication      repl            ::1/128                 scram-sha-256
```

#### 3. Set password_encryption

Set `password_encryption` value in `/tmp/pgsql/postgresql.conf` to be `scram-sha-256`

``` sh
password_encryption = scram-sha-256
```

For version 13, the default is `md5`, while for version 14 and above, it is `scram-sha-256`. Therefore, you should ensure that the value in `/tmp/pgsql/postgresql.conf` matches the value in `/tmp/pgsql/pg_hba.conf`.

#### 4. Set replication level

Set wal_level value in `/tmp/pgsql/postgresql.conf` to be `replica`

``` sh
wal_level = replica
```

#### 5. Start PostgreSQL

``` sh
pg_ctl  -D /tmp/pgsql/ start
```

Here, you may encounter issues such as the port being occupied or permission being denied. If you experience a failure, you can go to `/tmp/pgsql/log` to check the reason.

You can use

``` sh
pg_isready
```

to test

#### 6. Add role `repl`

``` sh
psql postgres
CREATE ROLE repl WITH LOGIN REPLICATION PASSWORD 'secretpassword';
\q
```

#### 7. Add replication slot

Add the required replication slot

``` sh
psql postgres
SELECT pg_create_physical_replication_slot('repl', true, false);
\q
```

Here, the command in the context of `pgmoneta` is used to create a replication slot to manage and retain WAL segments necessary for replication and backup operations, ensuring data consistency and reliability.

For more details, please check the [pgmoneta][pgmoneta].

#### 8. Verify access

For the role `repl` (pgmoneta) use `secretpassword`

``` sh
psql -h localhost -p 5432 -U repl postgres
\q
```

#### 9. Grant superuser privileges

Since some functions require the role to have superuser privileges, you can grant these privileges to the `repl` role using the following SQL command. However, please be cautious, as this will give the role full access and control over the database, posing potential security risks.

``` sql
ALTER ROLE repl WITH SUPERUSER;
```

The following SQL command can revoke the superuser privileges:

``` sql
ALTER USER repl WITH NOSUPERUSER;
```

### Check

If you just want to verify if pgmoneta_ext was installed successfully, you can use the `postgres` role to test.

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
