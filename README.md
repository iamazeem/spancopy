# spancopy

## Usage

```text
$ ./spancopy --help
spancopy v0.0.1 - utility to span (copy) files into subdirectories
Usage: ./spancopy [OPTIONS]

Options:
  --help                      show help
  --version                   show version
  --threshold FLOAT:POSITIVE REQUIRED
                              threshold in GBs
  --source TEXT:DIR REQUIRED  source directory
  --target TEXT:DIR REQUIRED  target directory
```

## Build

```shell
cmake -S . -B build
cmake --build build
```
