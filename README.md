# spancopy

[![ci](https://github.com/iamazeem/spancopy/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/iamazeem/spancopy/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/iamazeem/spancopy?logo=github&style=flat-square)](https://github.com/iamazeem/spancopy/releases)
[![Downloads](https://img.shields.io/github/downloads/iamazeem/spancopy/total?logo=github&style=flat-square)](https://github.com/iamazeem/spancopy/releases)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square)](https://github.com/iamazeem/spancopy/blob/master/LICENSE)

`spancopy` is a CLI tool to span (copy) files with size threshold from `source`
to `destination` directory. On reaching the `threshold` value, the `source`
files are spanned over multiple subdirectories under the `destination`
directory.

For more details, pleas refer to the [Usage](#usage) section.

Supported and tested on:

- Linux (AMD64 and ARM64)
- macOS
- Windows

See the [CI workflow](.github/workflows/ci.yml) for more details.

## Download

Download the prebuilt binaries from the
[Releases](https://github.com/iamazeem/spancopy/releases) page.

## Usage

Run `spancopy --help` for usage.

```text
$ ./spancopy --help
spancopy v0.0.1 - a CLI tool to span (copy) files with size threshold
Usage: ./spancopy [OPTIONS]

Options:
  -h,--help                   show help and exit
  -v,--version                show version and exit
  -t,--threshold UINT:SIZE [b, kb(=1024b), ...]:POSITIVE REQUIRED
                              threshold size to span files
  -s,--source TEXT:DIR REQUIRED
                              source directory
  -d,--destination TEXT:DIR REQUIRED
                              destination directory

Notes:
- On all platforms, the `/` is used as the path separator.
- Empty directories are ignored.
- The sizes of all the `source` files must be less than or equal to the
  `threshold` value. So, choose a `threshold` value accordingly.
- The `destination` directory must have enough space for all the `source`
  files to be copied successfully.
- A main subdirectory under `destination` is created to avoid conflicts.
  Following naming convention is used for this subdirectory:
    Format:  YYYYMMDDTHHMMSSMS
    Example: 20220820T170159946
- The main subdirectory under `destination` is removed if it already exists.
- For each `threshold` reached, the file(s) with their complete paths are
  copied under their respective subdirectories i.e. 1, 2, 3, and so on.
  For example, if for a `source` directory with some files, the `threshold`
  is reached three times then there will three subdirectories e.g.:
    Main subdir: 20220820T170159946
    Threshold 1: 20220820T170159946/1/.../<file(s)>
    Threshold 2: 20220820T170159946/2/.../<file(s)>
    Threshold 3: 20220820T170159946/3/.../<file(s)>

Examples:
  # spancopy from `src` to `dst` with threshold 500 bytes
  spancopy --threshold 500b --source ./src --destination ./dst

  # spancopy from `src` to `dst` with threshold 100KB
  spancopy --threshold 100kb --source ./src --destination ./dst

For any feedback or to report any issues, please open an issue on
GitHub: https://github.com/iamazeem/spancopy

Written by: AZEEM SAJID <azeem.sajid@gmail.com>

```

## Example

Here's a `source` directory (file sizes are in bytes for simplicity):

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

In the `source` directory, the maximum size is 100 bytes so the `threshold`
value must at least be 100 e.g.:

```shell
./spancopy --threshold 100 --source ./source --destination ./destination
```

Here's the result:

```shell
$ tree -hF destination/20220820T170146582/
destination/20220820T170146582/
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
