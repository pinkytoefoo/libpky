"""
used to generate `compile_commands.json` (using cmake)
which is used by the clangd lsp (used by nvim)

make sure to restart lsp after building
if using basic clangd and neovim setup
`:LspRestart`
"""

import subprocess

def configure():
    cmake_result = subprocess.run(["cmake", "-S", ".", "-B", "build", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"], capture_output=True, text=True)
    print(cmake_result.stdout)

if __name__ == "__main__":
    configure()
