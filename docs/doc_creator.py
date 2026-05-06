import os
import re

try:
    import yaml

    HAS_YAML = True
except ImportError:
    HAS_YAML = False

valid_types = [
    "int",
    "char",
    "void",
    "uint8_t",
    "uint16_t",
    "uint32_t",
    "uint64_t",
    "unsigned",
    "static",
    "float",
    "short",
    "long",
]

SKIP_FILES = {".gitignore", "mkdocs.yml", "grub.cfg", "readme.md", ".gitmodules"}
SKIP_DIRS = {".git", "build", "docs", "iso"}


# ── Dependency helpers ───────────────────────────────────────────────────────


def find_dependencies(file_con):
    deps = []
    for line in file_con.split("\n"):
        if line.startswith("#include"):
            dep = line.replace("#include", "").strip().strip('"').strip("<").strip(">")
            if dep:
                deps.append(dep)
    return deps


def process_dependencies(deps, current_src_path):
    if not deps:
        return "None"
    processed = []
    depth = len(current_src_path.replace("\\", "/").split("/")) - 1
    for dep in deps:
        target_md = dep + "\\"
        if "/" not in dep:
            processed.append(f"[{dep}]({target_md})")
        else:
            steps_to_root = "../" * depth
            processed.append(f"[{dep}]({steps_to_root}{target_md})")
    return "\n".join(processed)


# ── Function extractor ───────────────────────────────────────────────────────


def extract_functions(file_con):
    lines = file_con.split("\n")
    functions = []
    seen_sigs = set()

    i = 0
    while i < len(lines) - 1:
        line = lines[i].strip()
        next_line = lines[i + 1].strip() if i + 1 < len(lines) else ""

        is_fn_def = next_line == "{" or (
            next_line.startswith("{") and len(next_line) <= 2
        )

        if is_fn_def and (
            any(
                line.startswith(t)
                or f" {t} " in line
                or f" {t}*" in line
                or f" {t}(" in line
                for t in valid_types
            )
            and ";" not in line
            and "if" not in line
            and "while" not in line
            and "for" not in line
            and "switch" not in line
            and "else" not in line
            and "(" in line
            and line not in seen_sigs
        ):
            seen_sigs.add(line)

            # ── Parse params ─────────────────────────────────────────────────
            ret_match = re.match(
                r"^((?:static\s+)?(?:inline\s+)?(?:unsigned\s+)?[\w\*]+(?:\s+\*?)?)\s+([\w_]+)\s*\((.*)\)\s*$",
                line,
            )
            params = []
            if ret_match:
                params_str = ret_match.group(3).strip()
                if params_str and params_str != "void":
                    depth = 0
                    current = ""
                    for ch in params_str:
                        if ch in "([":
                            depth += 1
                            current += ch
                        elif ch in ")]":
                            depth -= 1
                            current += ch
                        elif ch == "," and depth == 0:
                            params.append(current.strip())
                            current = ""
                        else:
                            current += ch
                    if current.strip():
                        params.append(current.strip())

            # ── Scan function body for ALL return statements ──────────────────
            # Walk into the body tracking brace depth; collect every return
            returns = []
            brace_depth = 0
            j = i + 1
            in_body = False

            while j < len(lines):
                bline = lines[j]
                stripped = bline.strip()

                for ch in stripped:
                    if ch == "{":
                        brace_depth += 1
                        in_body = True
                    elif ch == "}":
                        brace_depth -= 1

                if in_body and brace_depth == 0:
                    break

                # Match every `return ...;` on this line (could be multiple on one line, rare but handle)
                for m in re.finditer(r"\breturn\b([^;]*);([^\n]*)", stripped):
                    ret_val = m.group(1).strip()
                    comment_raw = m.group(2).strip()

                    # Strip comment markers
                    comment = re.sub(r"^//\s*", "", comment_raw).strip()
                    comment = re.sub(r"^/\*\s*", "", comment)
                    comment = re.sub(r"\s*\*/$", "", comment).strip()

                    if comment:
                        meaning = comment
                    elif ret_val:
                        meaning = ret_val
                    else:
                        meaning = "void return"

                    entry = (ret_val if ret_val else "void", meaning)
                    if entry not in returns:  # keep order, dedupe exact dups
                        returns.append(entry)

                j += 1

            functions.append(
                {
                    "signature": line,
                    "params": params,
                    "returns": returns,
                }
            )

        i += 1

    return functions


def format_functions_md(functions):
    if not functions:
        return "_No functions found._"

    parts = []
    for fn in functions:
        block = f"### `{fn['signature']}`\n"

        if fn["params"]:
            block += "\n**Parameters:**\n"
            for p in fn["params"]:
                block += f"- `{p}`\n"
        else:
            block += "\n**Parameters:** none\n"

        if fn["returns"]:
            block += "\n**Returns:**\n"
            for val, meaning in fn["returns"]:
                block += f"- `{val}` → {meaning}\n"
        else:
            block += "\n**Returns:** nothing / void\n"

        parts.append(block)

    return "\n---\n\n".join(parts)


# ── Index page ───────────────────────────────────────────────────────────────


def generate_index(all_files_info, out_path):
    lines = ["# Project File Index\n"]
    lines.append("| File | Functions | Dependencies |")
    lines.append("|------|-----------|--------------|")

    for info in sorted(all_files_info, key=lambda x: x["path"]):
        path = info["path"]
        fn_names = []
        for f in info["functions"]:
            m = re.search(r"([\w_]+)\s*\(", f["signature"])
            fn_names.append(m.group(1) if m else "?")

        fn_cell = ", ".join(f"`{n}()`" for n in fn_names) if fn_names else "_none_"
        dep_names = [d.split("/")[-1] for d in info["deps"]]
        dep_cell = ", ".join(f"`{d}`" for d in dep_names) if dep_names else "_none_"

        doc_link = path + "/"
        lines.append(f"| [{path}]({doc_link}) | {fn_cell} | {dep_cell} |")

    with open(out_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    print(f"Index written → {out_path}")


# ── mkdocs.yml nav builder ────────────────────────────────────────────────────


def paths_to_nav(doc_paths):
    """
    Build a nested list-of-dicts nav structure from doc paths relative to docs/.
    e.g. ["GOS/index.md", "GOS/src/vga.c.md", ...]
    """
    tree = {}  # nested dict: {segment: {} or "path"}

    for p in sorted(doc_paths):
        parts = p.split("/")
        node = tree
        for seg in parts[:-1]:
            node = node.setdefault(seg, {})
        leaf = parts[-1]
        # display name: strip trailing .md
        display = leaf[:-3] if leaf.endswith(".md") else leaf
        node[display] = p  # leaf value is the full doc path

    def to_nav_list(node):
        items = []
        # files first (str values), then dirs (dict values)
        files = {k: v for k, v in node.items() if isinstance(v, str)}
        dirs = {k: v for k, v in node.items() if isinstance(v, dict)}
        for name, path in sorted(files.items()):
            items.append({name: path})
        for name, subtree in sorted(dirs.items()):
            items.append({name.capitalize(): to_nav_list(subtree)})
        return items

    return to_nav_list(tree)


def write_mkdocs_yml(all_doc_paths, yml_path="mkdocs.yml"):
    """
    Write (or update) mkdocs.yml preserving non-GOS nav entries.
    Works without pyyaml by doing a manual write when yaml is absent.
    """
    # Build the full list including the index
    full_paths = ["GOS/index.md"] + [f"GOS/{p}" for p in sorted(all_doc_paths)]
    gos_nav = paths_to_nav(full_paths)

    if HAS_YAML:
        # Load existing config
        if os.path.exists(yml_path):
            with open(yml_path, "r") as f:
                config = yaml.safe_load(f) or {}
        else:
            config = {}

        config.setdefault("site_name", "OS Docs")

        # Replace or append GOS section
        nav = config.get("nav", [])
        new_nav = [e for e in nav if not (isinstance(e, dict) and "GOS" in e)]
        new_nav.append({"GOS": gos_nav})
        config["nav"] = new_nav

        with open(yml_path, "w") as f:
            yaml.dump(
                config, f, default_flow_style=False, allow_unicode=True, sort_keys=False
            )

    else:
        # ── Manual YAML writer (no pyyaml needed) ──────────────────────────
        def nav_to_yaml(nav_list, indent=0):
            lines = []
            pad = "  " * indent
            for entry in nav_list:
                for title, val in entry.items():
                    if isinstance(val, str):
                        lines.append(f"{pad}- {title}: {val}")
                    else:
                        lines.append(f"{pad}- {title}:")
                        lines.append(nav_to_yaml(val, indent + 1))
            return "\n".join(lines)

        # Read existing yml (everything up to a `nav:` block or end)
        existing_lines = []
        if os.path.exists(yml_path):
            with open(yml_path, "r") as f:
                for line in f:
                    if line.startswith("nav:"):
                        break
                    existing_lines.append(line.rstrip())

        nav_yaml = nav_to_yaml(gos_nav, indent=1)
        out = "\n".join(existing_lines) + "\nnav:\n- GOS:\n" + nav_yaml + "\n"

        with open(yml_path, "w") as f:
            f.write(out)

    print(f"mkdocs.yml updated — {len(full_paths)} nav entries.")


# ── Main ──────────────────────────────────────────────────────────────────────

paths = []
for root, dirs, files in os.walk("."):
    dirs[:] = [d for d in dirs if d not in SKIP_DIRS]
    for file in files:
        if file in SKIP_FILES:
            continue
        rel = os.path.join(root, file).lstrip("./").lstrip(".\\").replace("\\", "/")
        paths.append(rel)

all_files_info = []
all_doc_paths = []  # relative to docs/GOS/

for path in paths:
    path_md = f"docs/GOS/{path}.md"
    os.makedirs(os.path.dirname(path_md), exist_ok=True)

    try:
        with open(path, "r", encoding="utf-8", errors="ignore") as f_src:
            content = f_src.read()
    except Exception as e:
        print(f"Could not read {path}: {e}")
        continue

    deps = find_dependencies(content)
    linked = process_dependencies(deps, path)
    functions = extract_functions(content)
    fn_md = format_functions_md(functions)

    with open(path_md, "w", encoding="utf-8") as f_dest:
        f_dest.write(
            f"# `{path}`\n\n"
            f"## Dependencies\n\n{linked}\n\n"
            f"## Functions\n\n{fn_md}\n"
        )

    all_files_info.append({"path": path, "deps": deps, "functions": functions})
    all_doc_paths.append(path + ".md")

os.makedirs("docs/GOS", exist_ok=True)
generate_index(all_files_info, "docs/GOS/index.md")
write_mkdocs_yml(all_doc_paths)

print(f"Done — {len(paths)} files documented.")
