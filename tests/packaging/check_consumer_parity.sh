#!/usr/bin/env bash
#
# Checks that the two ways of consuming an installed Codac describe the same
# library: find_package(CODAC), which reads share/codac/cmake/codac-config.cmake
# and the export set beside it, and pkg-config, which reads
# share/pkgconfig/codac.pc. The two are written by hand in different places of
# src/CMakeLists.txt and had drifted apart in both directions -- the .pc file
# named no library at all for a while, and once it did, it was still missing the
# include root, Eigen's headers, -pthread and the interval-arithmetic flags, so
# a pkg-config consumer either could not compile or silently got unsound
# rounding. Nothing in CI compared them, because nothing built anything against
# an installed Codac at all.
#
# It builds the same main.cpp both ways, runs both binaries, and compares:
#   - the set of include directories,
#   - the set of other compile flags,
#   - the set of libraries on the link line,
#   - the set of other link flags.
# Sets, not sequences: CMake repeats an archive to satisfy a dependency cycle
# while pkg-config orders dependents first, and both are correct.
#
# Usage:
#   check_consumer_parity.sh --prefix <codac install prefix> [options]
#
#   --prefix DIR            where Codac was installed (required)
#   --build-dir DIR         scratch directory for the two builds (default: ./build_packaging_check)
#   --cmake-prefix-path P   passed to the consumer's cmake, for IBEX and friends
#   --pkg-config-path P     prepended to PKG_CONFIG_PATH
#   --require-pkgconfig     fail instead of skipping when pkg-config cannot be used
#   --generator NAME        generator for the consumer's cmake; "" leaves the choice
#                           to cmake (which is what the Visual Studio jobs want)
#   --cmake-arg ARG         extra argument for the consumer's cmake (repeatable)

set -euo pipefail

here=$(cd "$(dirname "$0")" && pwd)

prefix=""
build_dir="$PWD/build_packaging_check"
cmake_prefix_path=""
extra_pkg_config_path=""
require_pkgconfig=0
generator_set=0
generator=""
cmake_args=()

while [ $# -gt 0 ]; do
  case "$1" in
    --prefix)             prefix="$2" ; shift 2 ;;
    --build-dir)          build_dir="$2" ; shift 2 ;;
    --cmake-prefix-path)  cmake_prefix_path="$2" ; shift 2 ;;
    --pkg-config-path)    extra_pkg_config_path="$2" ; shift 2 ;;
    --require-pkgconfig)  require_pkgconfig=1 ; shift ;;
    --generator)          generator="$2" ; generator_set=1 ; shift 2 ;;
    --cmake-arg)          cmake_args+=("$2") ; shift 2 ;;
    *) echo "unknown argument: $1" >&2 ; exit 2 ;;
  esac
done

[ -n "$prefix" ] || { echo "--prefix is required" >&2 ; exit 2 ; }

echo "=============================================================="
echo " Codac packaging check"
echo "   install prefix : $prefix"
echo "   build dir      : $build_dir"
echo "=============================================================="

# --------------------------------------------------------------------------
# What the install tree is expected to contain
# --------------------------------------------------------------------------
missing=0
for f in share/codac/cmake/codac-config.cmake \
         share/codac/cmake/codac-targets.cmake \
         share/pkgconfig/codac.pc \
         include/codac ; do
  if [ -e "$prefix/$f" ]; then
    echo "  present: $f"
  else
    echo "  MISSING: $f"
    missing=1
  fi
done
[ "$missing" -eq 0 ] || { echo "The install tree is incomplete." >&2 ; exit 1 ; }

echo
echo "--- codac.pc as installed ---"
cat "$prefix/share/pkgconfig/codac.pc"
echo

# --------------------------------------------------------------------------
# Half one: find_package(CODAC)
# --------------------------------------------------------------------------
cmake_build="$build_dir/via_cmake"
rm -rf "$cmake_build"

# A Makefile generator by default, whatever the generator Codac itself was
# built with: link.txt and compile_commands.json are where the flags actually
# handed to the compiler can be read back, and only the Makefile and Ninja
# generators write them. The Visual Studio generators write neither, which is
# one of the two reasons the comparison is skipped there (the other being that
# pkg-config speaks -I/-L/-l, which cl.exe does not); those jobs pass
# --generator "" and get the find_package half alone.
if [ "$generator_set" -eq 0 ]; then
  generator="Unix Makefiles"
  case "${OSTYPE:-}" in
    msys*|cygwin*|win32*) command -v mingw32-make >/dev/null 2>&1 && generator="MinGW Makefiles" ;;
  esac
fi

cmake_cmd=(cmake -S "$here" -B "$cmake_build"
           -D CMAKE_BUILD_TYPE=Release
           -D CMAKE_EXPORT_COMPILE_COMMANDS=ON)
if [ -n "$generator" ]; then
  cmake_cmd+=(-G "$generator")
fi
if [ -n "$cmake_prefix_path" ]; then
  cmake_cmd+=(-D "CMAKE_PREFIX_PATH=$prefix;$cmake_prefix_path")
else
  cmake_cmd+=(-D "CMAKE_PREFIX_PATH=$prefix")
fi
if [ ${#cmake_args[@]} -gt 0 ]; then
  cmake_cmd+=("${cmake_args[@]}")
fi

echo "--- configuring the find_package consumer ---"
"${cmake_cmd[@]}"
echo "--- building it ---"
# --config is what a multi-config generator needs and what a single-config one
# ignores, so it can be passed either way.
cmake --build "$cmake_build" --config Release

# Single-config generators put the binary at the top of the build tree,
# multi-config ones under a directory named after the configuration.
cmake_bin=""
for candidate in "$cmake_build/codac_packaging_check" \
                 "$cmake_build/codac_packaging_check.exe" \
                 "$cmake_build/Release/codac_packaging_check.exe" \
                 "$cmake_build/Release/codac_packaging_check" ; do
  [ -f "$candidate" ] && { cmake_bin="$candidate" ; break ; }
done
[ -n "$cmake_bin" ] || { echo "Built, but the executable is not where it was looked for." >&2 ; exit 1 ; }
# Run from the build directory: the figure the program draws is written to the
# current directory, and it has no business landing wherever the check was
# started from.
echo "--- running it ---"
( cd "$cmake_build" && "$cmake_bin" ) | tee "$build_dir/via_cmake.out"
grep -q "codac-packaging-check-ok" "$build_dir/via_cmake.out" \
  || { echo "The find_package consumer did not run to completion." >&2 ; exit 1 ; }
echo "find_package: OK"
echo

# --------------------------------------------------------------------------
# Is pkg-config usable here?
# --------------------------------------------------------------------------
# The directories are handed to pkg-config in the form it reads: cygpath -m
# turns the "/c/..." of a shell running under MSYS into the "C:/..." a native
# Windows binary expects, and is absent, hence a no-op, everywhere else.
to_pkg_config_path() {
  if command -v cygpath >/dev/null 2>&1 ; then cygpath -m "$1" ; else printf '%s' "$1" ; fi
}

# Captured before the loop below starts overwriting PKG_CONFIG_PATH, so that
# the second attempt joins the same three pieces as the first rather than the
# result of the first.
inherited_pkg_config_path="${PKG_CONFIG_PATH:-}"

join_pkg_config_path() {
  local sep="$1" joined
  joined=$(to_pkg_config_path "$prefix/share/pkgconfig")
  if [ -n "$extra_pkg_config_path" ]; then
    joined="$joined$sep$(to_pkg_config_path "$extra_pkg_config_path")"
  fi
  if [ -n "$inherited_pkg_config_path" ]; then
    joined="$joined$sep$inherited_pkg_config_path"
  fi
  printf '%s' "$joined"
}

# The separator is ";" for the native Windows pkg-config -- the one
# chocolatey's pkgconfiglite installs -- and ":" everywhere else. It is settled
# by trying rather than by reading $OSTYPE, which said "msys" nowhere the first
# time this ran on a GitHub Windows runner and left every directory joined by a
# ":" that pkg-config could not take apart, a path there beginning "C:".
# Whichever separator lets pkg-config find codac is the right one, and on a
# platform where it cannot be found at all the loop simply leaves the preferred
# spelling in place for the diagnostic below.
if command -v pkg-config >/dev/null 2>&1 ; then
  for sep in ";" ":" ; do
    export PKG_CONFIG_PATH="$(join_pkg_config_path "$sep")"
    if pkg-config --exists codac 2>/dev/null ; then
      break
    fi
  done
else
  export PKG_CONFIG_PATH="$(join_pkg_config_path ":")"
fi
echo "PKG_CONFIG_PATH=$PKG_CONFIG_PATH"

skip_reason=""
# The comparison reads the command line CMake recorded, so it needs a generator
# that records one; the Visual Studio generators write neither
# compile_commands.json nor link.txt.
if [ ! -f "$cmake_build/compile_commands.json" ] || \
   [ ! -f "$cmake_build/CMakeFiles/codac_packaging_check.dir/link.txt" ] ; then
  skip_reason="this generator records no compile or link command to compare against"
# And cl.exe does not take -I/-L/-l, so there would be nothing to transpose a
# pkg-config answer into even if it did.
elif grep -qiE '"command": *"[^"]*cl\.exe' "$cmake_build/compile_commands.json" ; then
  skip_reason="the compiler is MSVC, which does not take pkg-config's -I/-L/-l"
elif ! command -v pkg-config >/dev/null 2>&1 ; then
  skip_reason="pkg-config is not installed"
# "--exists codac" also resolves everything on the Requires: line, so a module
# named there that no .pc answers for (IBEX, and CAPD when that module is
# built) is caught right here.
elif ! pkg-config --exists codac ; then
  skip_reason="pkg-config cannot resolve codac: $(pkg-config --print-errors --exists codac 2>&1 || true)"
fi

if [ -n "$skip_reason" ]; then
  echo "pkg-config half skipped: $skip_reason"
  if [ "$require_pkgconfig" -eq 1 ]; then
    echo "This platform is expected to support it (--require-pkgconfig)." >&2
    exit 1
  fi
  echo "PARITY CHECK: find_package half only, as configured for this platform."
  exit 0
fi

# --------------------------------------------------------------------------
# Half two: pkg-config
# --------------------------------------------------------------------------
compiler=$(sed -n 's/.*"command": *"\([^ "]*\).*/\1/p' "$cmake_build/compile_commands.json" | head -1)
[ -n "$compiler" ] || { echo "Could not read the compiler out of compile_commands.json." >&2 ; exit 1 ; }

pc_bin="$build_dir/via_pkgconfig"
echo "--- building the same main.cpp from pkg-config alone ---"
echo "  compiler: $compiler"
echo "  cflags  : $(pkg-config --cflags codac)"
echo "  libs    : $(pkg-config --libs codac)"
# -std and the optimisation level are the consumer's own business in both
# halves (CMAKE_CXX_STANDARD and CMAKE_BUILD_TYPE set them on the other side),
# so they are given here rather than expected from codac.pc.
# shellcheck disable=SC2046
"$compiler" -std=c++20 -O3 -DNDEBUG $(pkg-config --cflags codac) \
    "$here/main.cpp" -o "$pc_bin" $(pkg-config --libs codac)
echo "--- running it ---"
( cd "$build_dir" && "$pc_bin" ) | tee "$build_dir/via_pkgconfig.out"
grep -q "codac-packaging-check-ok" "$build_dir/via_pkgconfig.out" \
  || { echo "The pkg-config consumer did not run to completion." >&2 ; exit 1 ; }
echo "pkg-config: OK"
echo

# --------------------------------------------------------------------------
# Comparison
# --------------------------------------------------------------------------
# CMake does not always put the flags on the command line: past a certain
# length, and as a matter of course for the MinGW Makefiles generator, it
# writes them into a response file and passes "@that_file" instead. Unexpanded,
# the find_package half then looks empty and every flag of the pkg-config half
# reads as a difference -- which is exactly what the first Windows run of this
# check reported.
expand_response_files() {
  local base="$1" tok file
  while IFS= read -r tok ; do
    case "$tok" in
      @*)
        file=${tok#@}
        case "$file" in
          /*|[A-Za-z]:*) ;;
          *) file="$base/$file" ;;
        esac
        if [ -f "$file" ]; then
          tr -d '\r' < "$file" | tr ' \t' '\n\n' | sed -e 's/^"//' -e 's/"$//' | grep . || true
        else
          printf '%s\n' "$tok"
        fi
        ;;
      *) printf '%s\n' "$tok" ;;
    esac
  done
}

# Collapses "a/b/../c" to "a/c" so that the two spellings of one directory --
# codac-config.cmake reaches the include root as <prefix>/include/codac-core/..
# while codac.pc names <prefix>/include -- compare equal.
# A response file on Windows may spell its directories with backslashes while
# pkg-config answers with forward ones; only the path-bearing lines are
# rewritten, so that a flag containing a backslash is left alone.
normalize_paths() {
  sed -e '/^\(INC\|LIB\) /s#\\#/#g' \
      -e ':a' -e 's#/[^/][^/]*/\.\./#/#; ta' \
      -e ':b' -e 's#/[^/][^/]*/\.\.$##; tb' \
      -e 's#/\./#/#g' -e 's#/$##'
}

# Everything neither file has any business carrying: the consumer's own build
# type, language level and position independence, and the mechanics of a single
# command line. "-o" takes the name of the output next to it, and "-MT"/"-MF"
# likewise, so those have to go in pairs rather than one token at a time.
drop_neutral() {
  awk '
    /^(-o|-MT|-MF|-isysroot|--sysroot)$/ { getline ; next }
    /^(-O[0-9s]?|-DNDEBUG|-g[0-9]?|-fPIC|-c|-MD|-MMD)$/ { next }
    /^-std=/ { next }
    /\.(o|obj|cpp|cc|d|json)$/ { next }
    /^$/ { next }
    { print }
  '
}

# find_package side: the compile command CMake recorded, and the link command it
# wrote, with the pieces above removed.
cmake_compile=$(sed -n 's/.*"command": *"\(.*\)",\{0,1\}$/\1/p' "$cmake_build/compile_commands.json" \
  | head -1 | sed 's/\\"/"/g' | tr ' ' '\n' | tail -n +2)
cmake_link=$(cat "$cmake_build/CMakeFiles/codac_packaging_check.dir/link.txt" | tr ' ' '\n' | tail -n +2)

# Turns a stream of tokens into two sorted sets: include directories, and the
# remaining compile flags.
split_cflags() {
  awk '
    /^-isystem$/ { getline d; print "INC " d; next }
    /^-I/        { d = substr($0,3); if (d == "") { getline d } print "INC " d; next }
    /^$/         { next }
                 { print "FLAG " $0 }
  '
}

# Archives and -l entries become bare library names; -L directories are dropped,
# the two halves reaching the same files by different routes (absolute paths on
# one side, -L plus -l on the other).
#
# Only flags that mean something at link time are kept beside them. CMake puts
# CMAKE_CXX_FLAGS on the link command as well, so the compile flags reappear
# there; they are compared as part of the compile half and would otherwise show
# up here as a difference against "pkg-config --libs", which rightly has none
# of them.
split_libs() {
  awk '
    /^-l/ { print "LIB " substr($0,3); next }
    /^-L/ { next }
    /\.(a|lib)$/ {
      n = $0
      sub(/.*[\/\\]/, "", n)
      sub(/^lib/, "", n)
      sub(/\.(a|lib)$/, "", n)
      print "LIB " n
      next
    }
    /^(-pthread|-rdynamic|-shared|-static.*)$/ { print "FLAG " $0; next }
    # -Wl, options are how the generator drives the linker on this platform --
    # --whole-archive, --out-implib, --major-image-version and the like on
    # MinGW. They describe the executable being produced, not the library being
    # consumed, and no .pc file has any business carrying them.
    /^-Wl,/ { next }
    { next }
  '
}

tmp="$build_dir/parity"
mkdir -p "$tmp"

# A -I naming a directory that is not there contributes nothing to the
# compile, so it is set aside rather than compared -- but printed, because it
# is a defect in whatever wrote it. The ibex.pc shipped in the prebuilt IBEX
# packages is the reason this exists: its prefix= is the directory IBEX was
# built in on the release machine rather than the one it was installed under,
# so "Requires: ibex" drags three such directories in on every platform.
partition_existing() {
  local keep="$1" gone="$2" line dir
  : > "$keep" ; : > "$gone"
  while IFS= read -r line ; do
    case "$line" in
      "INC "*)
        dir=${line#INC }
        if [ -d "$dir" ]; then echo "$line" >> "$keep" ; else echo "$line" >> "$gone" ; fi
        ;;
      *) echo "$line" >> "$keep" ;;
    esac
  done
}

printf '%s\n' "$cmake_compile" | expand_response_files "$cmake_build" | drop_neutral | split_cflags | normalize_paths | sort -u \
  | partition_existing "$tmp/cmake_cflags.txt" "$tmp/cmake_cflags_missing.txt"
pkg-config --cflags codac    | tr ' ' '\n' | drop_neutral | split_cflags | normalize_paths | sort -u \
  | partition_existing "$tmp/pc_cflags.txt" "$tmp/pc_cflags_missing.txt"

printf '%s\n' "$cmake_link"  | expand_response_files "$cmake_build" | drop_neutral | split_libs   | normalize_paths | sort -u > "$tmp/cmake_libs.txt"
pkg-config --libs codac      | tr ' ' '\n' | drop_neutral | split_libs   | normalize_paths | sort -u > "$tmp/pc_libs.txt"

status=0
report() {
  local what="$1" a="$2" b="$3"
  echo "--- $what ---"
  if diff -q "$a" "$b" >/dev/null ; then
    echo "  identical"
  else
    status=1
    echo "  find_package only:" ; comm -23 "$a" "$b" | sed 's/^/    /' ; :
    echo "  pkg-config only:"   ; comm -13 "$a" "$b" | sed 's/^/    /' ; :
  fi
}

report "compile: include directories and flags" "$tmp/cmake_cflags.txt" "$tmp/pc_cflags.txt"
report "link: libraries and flags"              "$tmp/cmake_libs.txt"   "$tmp/pc_libs.txt"

if [ -s "$tmp/cmake_cflags_missing.txt" ] || [ -s "$tmp/pc_cflags_missing.txt" ]; then
  echo "--- include directories that do not exist (not compared) ---"
  [ -s "$tmp/cmake_cflags_missing.txt" ] && sed 's/^/    find_package: /' "$tmp/cmake_cflags_missing.txt"
  [ -s "$tmp/pc_cflags_missing.txt" ]    && sed 's/^/    pkg-config:   /' "$tmp/pc_cflags_missing.txt"
  echo "  Harmless to the compiler, but each is a wrong path in whatever file"
  echo "  named it. The prebuilt IBEX packages ship an ibex.pc whose prefix= is"
  echo "  the build machine's directory, which is where these come from."
fi

echo
if [ "$status" -eq 0 ]; then
  echo "PARITY CHECK PASSED: find_package(CODAC) and pkg-config agree."
else
  # The raw command lines, so that a difference reported on a platform that
  # cannot be reproduced at hand can still be read rather than guessed at.
  echo "--- the two command lines, as they were read ---"
  echo "  find_package compile:"
  printf '%s\n' "$cmake_compile" | expand_response_files "$cmake_build" | sed 's/^/    /'
  echo "  find_package link:"
  printf '%s\n' "$cmake_link" | expand_response_files "$cmake_build" | sed 's/^/    /'
  echo "  pkg-config --cflags: $(pkg-config --cflags codac)"
  echo "  pkg-config --libs:   $(pkg-config --libs codac)"
  echo
  echo "PARITY CHECK FAILED: the two ways of consuming Codac disagree." >&2
  echo "Both are written in src/CMakeLists.txt; whatever one of them gained," >&2
  echo "the other needs too." >&2
fi
exit "$status"
