#!/bin/bash

# My clangd LSP couldn't resolve header paths, so this script compiles the paths

src_dir=$(realpath "$(pwd)")
cd ..
cmake -S . -B clangd -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ln -s clangd/compile_commands.json compile_commands.json
cd $src_dir
