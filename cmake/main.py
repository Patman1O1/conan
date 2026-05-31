import sys
import re
from pathlib import Path

def _target_exists(cmake_lists_path: Path, target_name: str) -> bool:
    regex: str = r"\badd_(?:executable|library|custom_target)\s*(?:\([ \t]*#[^\n]*\n)?\s*\(\s*([A-Za-z0-9_\.+\-\$\{\}\"\":]+)"

    # Return false if the path to the CMakeLists.txt file is non-existent nor a regular file
    if not cmake_lists_path.exists() or not cmake_lists_path.is_file():
        return False

    target_names: list[str]
    with open(cmake_lists_path) as cmake_lists_file:
        if re.findall(regex, cmake_lists_file.read()).count(target_name) > 0:
            return True
    return False

def _target_add_source(cmake_lists_path: Path, target_name: str, source_path: Path) -> bool:  # raises re.PatternError
    # Define patterns (regular expressions)
    source_path_regex: re.Pattern[str] = re.compile(
        r"[A-Za-z0-9_+${}\":/-]+\.(?:c|cpp|cxx|cc|h|hpp|hxx|hh|s|asm|ui|qrc)\b",
        flags=re.IGNORECASE
    )
    fn_name_regex: re.Pattern[str] = re.compile(r"\b(?:add_(?:executable|library|custom_target)|target_sources)")
    ignore_regex: re.Pattern[str] = re.compile(r"\s*(?:#[^\n]*\n\s*)*")

    # Strictly bind to YOUR target name
    primary_regex: re.Pattern[str] = re.compile(
        rf"({fn_name_regex.pattern}{ignore_regex.pattern}\({ignore_regex.pattern}{re.escape(target_name)}\b.*?\))",
        flags=re.DOTALL
    )

    # Read the contents of the CMakeLists.txt file
    cmake_lists_contents: str = cmake_lists_path.read_text(encoding="utf-8")

    # Ensure the target actually exists inside the CMakeLists.txt file
    target_match: re.Match[str] | None = re.search(primary_regex.pattern, cmake_lists_contents, flags=re.DOTALL)
    if target_match is None:
        raise re.PatternError(f"Target '{target_name}' was not found in {cmake_lists_path}. No changes made.")

    # Ensure the path to the source file isn't already added to the target
    if re.search(rf"\b{re.escape(source_path.name)}\b", target_match.group(1)):
        return False

    def __callback(match: re.Match[str]) -> str:
        matched_block = match.group(1)

        # Clean off trailing parens and whitespace
        base_block: str = matched_block.rstrip().rstrip(")")

        # IF THE BLOCK IS A SINGLE LINE:
        # e.g., "add_library(mylib SHARED main.cpp"
        if "\n" not in base_block.strip():
            # Find any existing source files on this line
            files = source_path_regex.findall(base_block)
            if files:
                # Extract the base command portion (everything before the first file)
                # e.g., "add_library(mylib SHARED "
                cmd_header = base_block.split(files[0])[0].rstrip()

                # Build a beautifully aligned multi-line block
                new_block = f"{cmd_header}\n"
                for f in files:
                    new_block += f"    {f}\n"
                return f"{new_block}    {source_path}\n)"

            # If it's a completely empty target declaration: add_library(mylib SHARED)
            return f"{base_block}\n    {source_path}\n)"

        # IF THE BLOCK IS ALREADY MULTI-LINE:
        # Determine indentation by looking at the last line's leading whitespace
        lines: list[str] = base_block.splitlines()
        indent: str = "    "  # fallback default
        for line in reversed(lines):
            if line.strip() != "":
                indent_match: re.Match[str] | None = re.match(r"^(\s*)", line)
                if indent_match is not None:
                    indent = indent_match.group(1)
                break

        # Ensure we append on a clean, indented new line
        if not base_block.endswith("\n"):
            base_block += "\n"

        return f"{base_block}{indent}{source_path}\n)"

    # Execute modification cleanly in a single pass
    new_content, count = re.subn(primary_regex.pattern, __callback, cmake_lists_contents, count=1, flags=re.DOTALL)

    if count <= 0:
        raise re.PatternError(f"Target '{target_name}' was not found in {cmake_lists_path}. No changes made.")

    cmake_lists_path.write_text(new_content, encoding="utf-8")
    return True

if __name__ == "__main__":
    tmp_dir: Path = Path(__file__).parent.parent/"tmp"
    cmake_lists_filepath: Path = tmp_dir/"CMakeLists.txt"
    source_path: Path = tmp_dir/"src"/sys.argv[1]

    if _target_add_source(cmake_lists_filepath, "mylib", source_path):
        sys.stdout.write(f"{source_path} added\n")
    else:
        sys.stdout.write(f"{source_path} already added to mylib\n")
    sys.exit(0)
