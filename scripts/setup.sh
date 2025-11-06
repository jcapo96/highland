#!/bin/bash

export MAKEFLAGS=-j8 QUICK=1

# automatically find the highland source directory
if [[ -d "highland" ]]; then
    export HIGHLANDPATH="$(pwd)"
else
    # Calculate path from script location
    # Script is in highland/scripts/, so go up two levels to get to HIGHLAND_NEW
    # Use a more robust method that works when sourced from different locations
    local script_dir="$(dirname -- "${BASH_SOURCE[0]:-$(realpath "$0")}")"
    if [[ -z "$script_dir" ]] || [[ "$script_dir" == "." ]]; then
        # Fallback: try to find the script location from the command line
        script_dir="$(dirname -- "$(realpath "${BASH_SOURCE[0]:-$0}")")"
    fi

    if [[ -n "$script_dir" ]] && [[ "$script_dir" != "." ]]; then
        export HIGHLANDPATH="$(cd -P -- "$script_dir/../.." && pwd -P)"
    else
        echo "Error: Could not determine script location. Please run this script from the HIGHLAND_NEW directory."
        return 1
    fi

    # Verify the path is correct by checking if highland directory exists
    if [[ ! -d "$HIGHLANDPATH/highland" ]]; then
        echo "Error: Could not find highland directory in calculated path: $HIGHLANDPATH"
        return 1
    fi
fi
echo "HIGHLANDPATH=${HIGHLANDPATH}"

# Function to find the latest version of a product
find_latest_version() {
    local base_dir="$1"
    local product="$2"
    local compiler="$3"

    if [[ ! -d "$base_dir/$product" ]]; then
        return 1
    fi

    # Find the latest version directory
    local latest_version=""
    local latest_major=0
    local latest_minor=0
    local latest_patch=0

    for version_dir in "$base_dir/$product"/*; do
        if [[ -d "$version_dir" ]]; then
            local version=$(basename "$version_dir")
            # Extract version numbers (e.g., v6_22_08d -> 6, 22, 8)
            if [[ $version =~ v([0-9]+)_([0-9]+)_([0-9]+) ]]; then
                local major=${BASH_REMATCH[1]}
                local minor=${BASH_REMATCH[2]}
                local patch=${BASH_REMATCH[3]}

                if (( major > latest_major )) || \
                   (( major == latest_major && minor > latest_minor )) || \
                   (( major == latest_major && minor == latest_minor && patch > latest_patch )); then
                    latest_major=$major
                    latest_minor=$minor
                    latest_patch=$patch
                    latest_version=$version
                fi
            fi
        fi
    done

    if [[ -n "$latest_version" ]]; then
        echo "$latest_version"
        return 0
    fi
    return 1
}

# Function to find the appropriate compiler directory
find_compiler_dir() {
    local base_dir="$1"
    local product="$2"
    local version="$3"

    if [[ ! -d "$base_dir/$product/$version" ]]; then
        return 1
    fi

    # Look for common compiler directory patterns
    local compiler_patterns=(
        "Linux64bit+3.10-2.17-e20-p392-prof"
        "Linux64bit+3.10-2.17-e20"
        "Linux64bit+3.10-2.17"
        "Linux64bit+2.6-2.12-e17-prof"
        "Linux64bit+2.6-2.12"
        "Linux64bit+2.6-2.12-e17-prof"
        "Linux64bit+2.6-2.12"
    )

    for pattern in "${compiler_patterns[@]}"; do
        if [[ -d "$base_dir/$product/$version/$pattern" ]]; then
            echo "$pattern"
            return 0
        fi
    done

    # If no pattern matches, try to find any directory
    for dir in "$base_dir/$product/$version"/*; do
        if [[ -d "$dir" ]]; then
            echo "$(basename "$dir")"
            return 0
        fi
    done

    return 1
}

# Function to auto-detect products directory
detect_products_dir() {
    local possible_dirs=(
        "/cvmfs/larsoft.opensciencegrid.org/products"
        "/data4/NEXT/software/scisoft"
        "/cvmfs/fermilab.opensciencegrid.org/products"
        "/cvmfs/cern.ch/sw/lcg"
        "/opt/products"
        "/usr/local/products"
    )

    for dir in "${possible_dirs[@]}"; do
        if [[ -d "$dir" ]]; then
            echo "$dir"
            return 0
        fi
    done

    return 1
}

# Function to auto-detect ROOT installation
detect_root_installation() {
    local possible_roots=(
        "/Applications/root_src/root_install"  # Prioritize this path for ARM64 compatibility
        "/usr/local/root"
        "/opt/root"
        "/usr/local/root_install"
        "/Applications/root_src/root_install"
        "/hep/sw/root-6.12.06/myroot"
        "$HOME/root"
        "$HOME/root_install"
    )

    # Check if ROOT is already in PATH
    if command -v root-config >/dev/null 2>&1; then
        local root_config_path=$(command -v root-config)
        local root_bin_dir=$(dirname "$root_config_path")
        local root_dir=$(dirname "$root_bin_dir")
        echo "$root_dir"
        return 0
    fi

    # Check for root-config in common locations
    for root_dir in "${possible_roots[@]}"; do
        if [[ -f "$root_dir/bin/root-config" ]]; then
            echo "$root_dir"
            return 0
        fi
    done

    return 1
}

# Function to check if we're in a conda environment and clean it up
clean_conda_environment() {
    if [[ -n "$CONDA_PREFIX" ]] || [[ -n "$CONDA_DEFAULT_ENV" ]]; then
        echo "Warning: Conda environment detected. Cleaning up compiler flags for ARM64 compatibility..."

        # Unset conda-related compiler flags that might interfere with ARM64
        unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
        unset CONDA_BACKUP_CFLAGS CONDA_BACKUP_CXXFLAGS CONDA_BACKUP_CPPFLAGS CONDA_BACKUP_LDFLAGS

        # Ensure we're using system compilers
        export CC="/usr/bin/clang"
        export CXX="/usr/bin/clang++"

        echo "Compiler set to system ARM64 clang: $CC, $CXX"
    fi
}

# Main setup logic
echo "Setting up HIGHLAND environment..."

# Clean up conda environment interference for ARM64 compatibility
clean_conda_environment

# Allow environment variables to override auto-detection
if [[ -n "$HIGHLAND_PRODUCTS_DIR" ]]; then
    products_dir="$HIGHLAND_PRODUCTS_DIR"
    echo "Using products directory from environment: $products_dir"
elif [[ -n "$HIGHLAND_ROOT_DIR" ]]; then
    root_dir="$HIGHLAND_ROOT_DIR"
    echo "Using ROOT directory from environment: $root_dir"
else
    # Auto-detect products directory
    if products_dir=$(detect_products_dir); then
        echo "Auto-detected products directory: $products_dir"
    else
        echo "No products directory found, will try to use system ROOT"
    fi

    # Auto-detect ROOT installation
    if root_dir=$(detect_root_installation); then
        echo "Auto-detected ROOT installation: $root_dir"
    else
        echo "Warning: No ROOT installation found automatically"
        root_dir="/usr/local/root"  # Default fallback
    fi
fi

# Set up products if we have a products directory
if [[ -n "$products_dir" ]] && [[ -d "$products_dir" ]]; then
    echo "Setting up products from: $products_dir"

    # Auto-detect versions and compilers
    if gcc_version=$(find_latest_version "$products_dir" "gcc" ""); then
        echo "Using GCC version: $gcc_version"
        if gcc_compiler=$(find_compiler_dir "$products_dir" "gcc" "$gcc_version"); then
            echo "Using GCC compiler: $gcc_compiler"
            export gcc_dir="$products_dir/gcc/$gcc_version/$gcc_compiler"
        fi
    fi

    if root_version=$(find_latest_version "$products_dir" "root" ""); then
        echo "Using ROOT version: $root_version"
        if root_compiler=$(find_compiler_dir "$products_dir" "root" "$root_version"); then
            echo "Using ROOT compiler: $root_compiler"
            export root_dir="$products_dir/root/$root_version/$root_compiler"
        fi
    fi

    if tbb_version=$(find_latest_version "$products_dir" "tbb" ""); then
        echo "Using TBB version: $tbb_version"
        if tbb_compiler=$(find_compiler_dir "$products_dir" "tbb" "$tbb_version"); then
            echo "Using TBB compiler: $tbb_compiler"
            export tbb_dir="$products_dir/tbb/$tbb_version/$tbb_compiler"
        fi
    fi

    if cmake_version=$(find_latest_version "$products_dir" "cmake" ""); then
        echo "Using CMake version: $cmake_version"
        if cmake_compiler=$(find_compiler_dir "$products_dir" "cmake" "$cmake_version"); then
            echo "Using CMake compiler: $cmake_compiler"
            export cmake_dir="$products_dir/cmake/$cmake_version/$cmake_compiler"
        fi
    fi

    # Set up library paths
    if [[ -n "$gcc_dir" ]]; then
        export LD_LIBRARY_PATH="$gcc_dir/lib64:$gcc_dir/lib:$LD_LIBRARY_PATH"
        export PATH="$gcc_dir/bin:$PATH"
    fi

    if [[ -n "$tbb_dir" ]]; then
        export LD_LIBRARY_PATH="$tbb_dir/lib:$LD_LIBRARY_PATH"
    fi

    if [[ -n "$cmake_dir" ]]; then
        export PATH="$cmake_dir/bin:$PATH"
    fi
fi

# Set up ROOT environment
if [[ -z "$ROOTSYS" ]]; then
    # If we have a detected root_dir, use it
    if [[ -n "$root_dir" ]]; then
        export ROOTSYS="$root_dir"
    else
        # Fallback: try to use the preferred ARM64-compatible ROOT installation
        if [[ -f "/Applications/root_src/root_install/bin/root-config" ]]; then
            export ROOTSYS="/Applications/root_src/root_install"
            echo "Using preferred ARM64-compatible ROOT installation: $ROOTSYS"
        else
            echo "Warning: No ROOT installation found"
            return 1
        fi
    fi

    export PATH="$ROOTSYS/bin:$PATH"

    # Set library paths based on OS
    if [[ "$OSTYPE" == "darwin"* ]]; then
        export DYLD_LIBRARY_PATH="$ROOTSYS/lib/root:$ROOTSYS/lib:$DYLD_LIBRARY_PATH"
    else
        export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"
    fi

    # Set ROOT include path
    export ROOT_INCLUDE_PATH="$HIGHLANDPATH/include"

    echo "ROOT environment set up: $ROOTSYS"
fi

# Set up HIGHLAND paths
export PATH="$HIGHLANDPATH/highland/bin:$PATH"

if [[ "$OSTYPE" == "darwin"* ]]; then
    export DYLD_LIBRARY_PATH="$HIGHLANDPATH/highland/lib:$DYLD_LIBRARY_PATH"
else
    export LD_LIBRARY_PATH="$HIGHLANDPATH/highland/lib:$LD_LIBRARY_PATH"
fi

export CMAKE_PREFIX_PATH="$HIGHLANDPATH:$ROOTSYS"

# Set package hierarchy
export HIGHLAND_PACKAGE_HIERARCHY="psycheCore"

# Set the ROOT folder for every package in highland
echo "Setting up package ROOT variables..."
D0=$PWD
cd "$HIGHLANDPATH/highland/src" || exit 1

for D1 in psyche highland2; do
    if [[ ! -d "$D1" ]]; then
        continue
    fi
    cd "$D1" || continue
    for D2 in *; do
        if [[ ! -d "$D2" ]]; then
            continue
        fi
        D3=$(echo "$D2" | tr "[a-z]" "[A-Z]")
        D4="${D3}ROOT"
        export "${D4}=$HIGHLANDPATH/highland/src/$D1/$D2"
    done
    cd .. || exit 1
done

cd "$D0" || exit 1

echo "HIGHLAND environment setup complete!"
echo "ROOTSYS: ${ROOTSYS:-'Not set'}"
echo "PATH includes: $HIGHLANDPATH/bin"
echo "Library paths updated for: $OSTYPE"

