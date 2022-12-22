# `spancopy`

[![ci](https://github.com/iamazeem/spancopy/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/iamazeem/spancopy/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/iamazeem/spancopy?logo=github&style=flat-square)](https://github.com/iamazeem/spancopy/releases)
[![Downloads](https://img.shields.io/github/downloads/iamazeem/spancopy/total?logo=github&style=flat-square)](https://github.com/iamazeem/spancopy/releases)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/iamazeem/spancopy/blob/master/LICENSE)

`spancopy` is a CLI tool that copies files from `source` to `target` directory
per `threshold` e.g. KB, MB, etc. The source files are copied to the
subdirectories under the `target` directory. A main subdirectory is created
under the `target` directory to avoid conflicts. The first subdirectory name is
`1`. If the total size of the subdirectory i.e. the size of the files copied
into it, reaches the threshold value, a new subdirectory with an incremented
name is created i.e. `2` and so on until all the files are copied.

The sizes of all the source files must be less or equal to the `threshold`
value.

There must be enough space at `target` for the `source` files to be copied
successfully.

**NOTE**: Only files are copied. Empty directories are ignored.

Supports:

- Linux (AMD64 and ARM64)
- macOS
- Windows

## Download

Download the prebuilt binaries from the
[Releases](https://github.com/iamazeem/spancopy/releases) page.

## Usage

```text
$ ./spancopy --help
spancopy v0.0.1 - CLI tool to copy files from source to target per threshold
Usage: ./build/spancopy [OPTIONS]

Options:
  --help                      show help
  --version                   show version
  --threshold UINT:SIZE [b, kb(=1024b), ...]:POSITIVE REQUIRED
                              threshold size
  --source TEXT:DIR REQUIRED  source directory
  --target TEXT:DIR REQUIRED  target directory

Notes:
- The `threshold` unit may be bytes, KB, MB, GB, etc.
- The `threshold` must be less or equal to all source files' sizes.
- A main subdirectory under `target` is created to avoid conflicts.
    Format:  YYYYMMDDTHHMMSSMS
    Example: 20220820T170159946
- The `target` directory tree is removed if it exists already.
- On all platforms, the `/` is used as the path separator.

Examples:
  # With Linux style options
  spancopy --threshold 500mb --source <source> --target <target>

  # With Windows style options
  spancopy /threshold 100kb /source <source> /target <target>
```

Here's a complete example:

Here's a `source` directory tree (files sizes are in bytes for simplicity):

```shell
$ tree -hF source/
source/
├── [4.0K]  dir1/
│   ├── [  10]  file.10
│   ├── [  20]  file.20
│   ├── [  30]  file.30
│   └── [  40]  file.40
├── [4.0K]  dir2/
│   ├── [  50]  file.50
│   ├── [  60]  file.60
│   ├── [  70]  file.70
│   └── [  80]  file.80
├── [4.0K]  dir3/
│   └── [  90]  file.90
├── [4.0K]  empty-dir/
└── [ 100]  file.100

4 directories, 10 files
```

In the above directory structure, the maximum size of 100 bytes so the
`threshold` value must be 100 or above e.g.:

```shell
./spancopy --threshold 100 --source ./source --target ./target
```

Here's the result of running with a `threshold` value of 100 bytes:

```shell
$ tree -hF target/20220820T170146582/
target/20220820T170146582/
├── [4.0K]  1/
│   └── [ 100]  file.100
├── [4.0K]  2/
│   └── [4.0K]  dir1/
│       ├── [  10]  file.10
│       ├── [  20]  file.20
│       ├── [  30]  file.30
│       └── [  40]  file.40
├── [4.0K]  3/
│   └── [4.0K]  dir2/
│       └── [  50]  file.50
├── [4.0K]  4/
│   └── [4.0K]  dir2/
│       └── [  60]  file.60
├── [4.0K]  5/
│   └── [4.0K]  dir2/
│       └── [  70]  file.70
├── [4.0K]  6/
│   └── [4.0K]  dir2/
│       └── [  80]  file.80
└── [4.0K]  7/
    └── [4.0K]  dir3/
        └── [  90]  file.90

13 directories, 10 files
```

## Build

Prerequisites:

- CMake
- C++17 compiler

Run:

```shell
git clone git@github.com:iamazeem/spancopy.git
cd spancopy
cmake -S . -B build
cmake --build build
```

The executable will be under the `build` directory.

```shell
./build/spancopy --help
```

## Contribute

- [Fork](https://github.com/iamazeem/spancopy/fork) the project.
- Clone the forked project.
- Check out the latest `main` branch.
- Create a `feature` or `bugfix` branch from `main`.
- Commit and push your changes.
- Submit the PR.

## License

[MIT](./LICENSE)
