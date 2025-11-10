\newpage

## Test suite

### Container Environment

To ensure the test suite works well, please make sure you have installed `Docker` or `podman` on your OS. If neither `Docker` nor `podman` is installed, the test suite compilation will be skipped.

**Docker**

First, ensure your system is up to date.

```sh
dnf update
```

Install the necessary packages for Docker.

```sh
dnf -y install dnf-plugins-core
```

Add the Docker repository to your system.

``` sh
sudo dnf config-manager --add-repo https://download.docker.com/linux/fedora/docker-ce.repo
```

Install Docker Engine, Docker CLI, and Containerd.

```sh
sudo dnf install docker-ce docker-ce-cli containerd.io
```

Start the Docker service and enable it to start on boot.

```sh
sudo systemctl start docker
sudo systemctl enable docker
```

Verify that Docker is installed correctly.

```sh
docker --version
```

If you see the Docker version, then you have successfully installed Docker on Fedora.

**Podman**

Install Podman and the Docker alias package.

```sh
dnf install podman podman-docker.noarch
```

Verify that Podman is installed correctly.

```sh
podman --version
```

If you see the Podman version, then you have successfully installed Podman on Fedora.

The `podman-docker.noarch` package simplifies the use of `Podman` for users accustomed to Docker.

### Test on container

You can easily use `CTest` to test all PostgreSQL versions from 14 to 18. It will automatically build the container and run all the test suites for you.

After you follow the [DEVELOPERS.md][developers] to install `pgmoneta_ext`, go to the directory `/pgmoneta_ext/build` and run the test.

``` sh
make test
```

This command will run the tests sequentially. If you want them to run simultaneously, please use the following command:

``` sh
ctest -j 8
```

This will run the tests in parallel using the given number of jobs.

`CTest` will output logs into `/pgmoneta_ext/build/Testing/Temporary/LastTest.log`. If you want to check the specific process, you can review that log file.

### Local test

Before you test, you need to install the `check` library. If there is no package for `check`, the `CMakeLists.txt` will not compile the test suite. Only after you have installed `check` will it compile the test suite.

``` sh
dnf install -y check check-devel check-static
```

If you want to test if `pgmoneta_ext` works well on your machine, please follow these steps:

- Make sure you have installed PostgreSQL.

- Download the `pgmoneta_ext` code from GitHub and perform all the configurations:

    ```sh
    git clone https://github.com/pgmoneta/pgmoneta_ext
    cd pgmoneta_ext
    ```

- Change the parameters related to your machine in the file `/test/lib/pgmoneta_ext_test.c`:

    ```c
    #define PGMONETA_EXT_GET_FILE_PATH        "/pgsql/logfile"
    #define PGMONETA_EXT_GET_FILES_PATH       "/conf"
    #define PGMONETA_EXT_GET_OID_DBNAME       "mydb"
    ```

- Compile the code using:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
    ```

- Run PostgreSQL and ensure that you have already created the `pgmoneta_ext` extension.

- Run the following commands:

    ```sh
    export PGPASSWORD='secretpassword'
    ./test/pgmoneta_ext_test
    ```

The `pgmoneta_ext_test` executable will use `check` to run all the functions in the extension locally and verify if they work correctly.
