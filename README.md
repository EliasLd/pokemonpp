A CLI pokemon battle simulation.

## Getting started

First clone the repositery including submodule:

```bash
git clone --recurse-submodules <repositery_url>
```

### Classical Build and Run
Then create a build folder and execute the following commands:

```bash
mkdir -p build
cd build
cmake ..
make
```

You can now run the executable:

```bash
./pokemon
```

### Docker Build and Run
Build the Docker image using the following command:

```bash
docker build . -t pokemonpp
```

Then run `pokemonpp` in a Docker container:

```bash
docker run -it --rm pokemonpp
```

