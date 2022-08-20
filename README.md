# spancopy

[![ci](https://github.com/iamazeem/spancopy/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/iamazeem/spancopy/actions/workflows/ci.yml)

## Usage

```text
$ ./spancopy --help
spancopy v0.0.1 - spancopy source files into target subdirectories per threshold
Usage: ./spancopy [OPTIONS]

Options:
  --help                      show help
  --version                   show version
  --threshold UINT:SIZE [b, kb(=1024b), ...]:POSITIVE REQUIRED
                              threshold size
  --source TEXT:DIR REQUIRED  source directory
  --target TEXT:DIR REQUIRED  target directory
```

- The `threshold` can be in bytes, KB, MB, GB, etc.
- The `target` directory is removed before copying.

Examples:

- Spancopy with 1 KB threshold:

```shell
./spancopy --threshold 1k --source ./source --target ./target
```

- Spancopy with 1 MB threshold:

```shell
./spancopy --threshold 1mb --source ./source --target ./target
```

## Build

```shell
cmake -S . -B build
cmake --build build
```

## Run

```shell
./build/spancopy --help
```

## Contribute

- Fork the project.
- Check out the latest `main` branch.
- Create a feature or bugfix branch from `main`.
- Commit and push your changes.
- Submit the PR.
