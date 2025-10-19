# rast
rasterizer in c

## Cloning

```shell
git clone --recursive https://github.com/no-good-names/rast
```

## Build

### Building with ninja
```shell
cmake -S . -B build -G "Ninja"
ninja -C build
```

### Building without ninja
```shell
# or building without ninja
cmake -S . -B build
make -C build
```
