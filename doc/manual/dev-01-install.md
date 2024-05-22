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

    Download the `Eisvogel` template for `pandoc`, please visit the [pandoc-latex-template](https://github.com/Wandmalfarbe/pandoc-latex-template) repository. For a standard installation, you can follow the steps outlined below.

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

### Check

You can open `PostgreSQL` to verify if `pgmoneta_ext` was installed successfully.

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
