\newpage

# Test suite

## Installation

Since we are using source code to install, please first create a new user `postgres` for a normal installation.

``` sh
sudo adduser postgres
```

Enter into the `/test` directory.

``` sh
chmod +x installation.sh
```

Run `installation.sh` to install any necessary dependencies and configuration.

``` sh
./installation.sh
```

The script will install PostgreSQL 13 by default. If you prefer to install a specific version, please input the version as shown below. It will automatically download and use the latest patch release number. You can find the source versions at [PostgreSQL GIT repository][postgresqlrepo].

``` sh
./installation.sh 14
```

> IMPORTANT:
> If you want to test different versions, please make sure you have deleted all previous version data, removed the role 'repl', and stopped the PostgreSQL server. Otherwise, it will raise some compatibility errors when you install and configure a new version. It is better to kill all current PostgreSQL processes.

If you encounter the following error when creating the extension, you may need to inform the system where to find the `libpq.so.5` library. Use the command `echo "/usr/local/pgsql/lib" | sudo tee -a /etc/ld.so.conf` and then update the cache using `sudo ldconfig`. After this, you need to create the extension manually.

``` console
ERROR:  could not load library "/usr/local/pgsql/lib/pgmoneta_ext.so": libpq.so.5: cannot open shared object file: No such file or directory
```

If you see the error `could not change directory to "/home/pgmoneta/pgmoneta_ext/test/build": Permission denied`, it doesn't matter. This will not affect the execution of the command; it is just because you used `sudo`, which tries to change the current working directory.

## Test functions

If you have already installed `pgmoneta_ext` successfully and completed all configurations described in [DEVELOPERS.md][developers], you can skip the **Installation** step above and directly follow the steps below to test the functions in the API extension.

download dependencies

``` sh
dnf install check check-devel
```

Enter into the `/test` directory.

``` sh
mkdir build
cd build
cmake ..
make
```

Before running the `pgmoneta_ext_test` executable, you need to set the `PGPASSWORD` environment variable for the `repl` role. For our setup, the password is `secretpassword`. Follow these steps:

Set the `PGPASSWORD` environment variable:

``` sh
export PGPASSWORD='secretpassword'
```

Run the `pgmoneta_ext_test` executable.

``` sh
./pgmoneta_ext_test
```
