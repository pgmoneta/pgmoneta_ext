# Test

## Container Environment

To ensure the test suite works well, please make sure you have installed `Docker` or `podman` on your OS. If neither `Docker` nor `podman` is installed, the test suite compilation will be skipped.

### Docker

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

### Podman

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

## Test suite

Before you test, you need to install the `check` library. If there is no package for `check`, the `CMakeLists.txt` will not compile the test suite. Only after you have installed `check` will it compile the test suite.

``` sh
dnf install -y check check-devel check-static
```

You can simply use `CTest` to test all PostgreSQL versions from 14 to 18. It will automatically run `testsuite.sh` to test `pgmoneta_ext` for each version. The script will automatically create the Docker container, run it, and then use the `check` framework to test their functions inside it. After that, it will automatically clean up everything for you.

After you follow the [DEVELOPERS.md](https://github.com/pgmoneta/pgmoneta_ext/blob/main/doc/DEVELOPERS.md) to install `pgmoneta_ext`, go to the directory `/pgmoneta_ext/build` and run the test.

``` sh
make test
```

This command will run the tests sequentially. If you want them to run simultaneously, please use the following command:

``` sh
ctest -j 8
```

This will run the tests in parallel using the given number of jobs.

`CTest` will output logs into `/pgmoneta_ext/build/Testing/Temporary/LastTest.log`. If you want to check the specific process, you can review that log file.

`testsuite.sh` accepts three variables. The first one is `dir`, which specifies the `/test` directory location, with a default value of `./`. The second one is `dockerfile`, with a default value of `Dockerfile.rocky8`. The third one is the PostgreSQL `version`, with a default value of `14`.

## Local test

If you just want to test if `pgmoneta_ext` works well on your machine, please follow these steps:

- Install `pgmoneta_ext` and create the extension in PostgreSQL.
- Navigate to the directory `/pgmoneta_ext/build/test`.
- Run the following commands:

```sh
export PGPASSWORD='secretpassword'
./pgmoneta_ext_test
```

The `pgmoneta_ext_test` executable will use `check` to run all the functions in the extension locally and verify if they work correctly.
