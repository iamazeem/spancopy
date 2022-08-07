# spancopy

## Usage

```text
$ spancopy --help
spancopy - utility to span (copy) files into subdirectories
Usage: spancopy [OPTIONS]

Options:
  --help                      show usage help
  --size FLOAT:POSITIVE REQUIRED
                              threshold in GBs to span (copy) files
  --source TEXT:DIR REQUIRED  source directory
  --target TEXT:DIR REQUIRED  target directory
```

## Build

```shell
cmake -S . -B build
cmake --build build
```
