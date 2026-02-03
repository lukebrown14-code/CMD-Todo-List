# CMD-Todo-List
Dead simple command-line todo list. No config, no database, just works.

## Requirements

- GCC
- Make

## Installation

```bash
git clone https://github.com/lukebrown14-code/CMD-Todo-List.git
cd CMD-Todo-List
make
sudo cp todo /usr/local/bin/
```

This installs the `todo` binary to `/usr/local/bin/` so you can run it from anywhere. Data is stored in `~/.todo/data.bin`.

To uninstall:

```bash
sudo rm /usr/local/bin/todo
rm -rf ~/.todo
```

## Usage

```
todo add "Buy milk"          # add an item
todo add Pick up package     # quotes are optional
todo print                   # show all items
todo remove 2                # remove item by number
todo clear                   # delete all items
```
