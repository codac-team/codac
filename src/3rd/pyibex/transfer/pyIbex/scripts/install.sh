#/bin/sh

py_version=$(python -c "import sys; print(sys.version[:3])") # default python version
prefix=""  # Default install prefix

# extract options and their arguments into variables.
for var in "$@"; do
    case "$var" in
        -h)
            echo "Usage:"
            echo "    $0 [options]"
            echo "Options: "
            echo "    -h                           Display this help message."
            echo "    --prefix=<install_prefix>     Install prefix for libs"
            echo "    --python=<python_version>     Python version to use"
            exit 0;;
        --prefix=*)
            prefix="-DCMAKE_INSTALL_PREFIX=${var##*=}" ;;# Remove 'install' from the argument list
        --python=*)
            py_version="${var##*=}"; ;;# Remove 'install' from the argument list
        *) echo "Internal error!" ; exit 1 ;;
    esac
done


# set -x
echo "Installation of pyIbex and its dependancies"
echo "Configuration :"

if [ -n "$prefix" ]; then
  echo "    - custom install prefix = ${prefix##*=}"
fi
echo "    - python version = ${py_version}"

# save current path
curr_path=$(pwd)

cd $curr_path
echo " ibex_lib installation"
git clone -b develop https://github.com/ibex-lib/ibex-lib.git
cd ibex-lib
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=${prefix##*=} -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ../
make -j2 && make check
make install

cd $curr_path
git submodule init
git submodule update
echo " build pyIbex"
mkdir -p build && cd build
cmake $prefix -DPYTHON_VERSION=${py_version} ../
make && make test
