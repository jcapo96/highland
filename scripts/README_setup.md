# HIGHLAND Setup Script Documentation

## Overview

The `setup.sh` script has been redesigned to be more general and automatically detect available software products on any machine. It no longer requires hardcoded paths for specific machines.

## Features

### Auto-detection
- **Products Directory**: Automatically finds products in common locations like CVMFS, local installations, etc.
- **ROOT Installation**: Detects ROOT installations in standard locations or already in PATH
- **Version Selection**: Automatically selects the latest available versions of products
- **Compiler Detection**: Finds appropriate compiler directories for detected products

### Supported Locations

#### Products Directories
The script will look for products in these locations (in order of preference):
- `/cvmfs/larsoft.opensciencegrid.org/products` (CERN/Fermilab)
- `/data4/NEXT/software/scisoft` (IFIC)
- `/cvmfs/fermilab.opensciencegrid.org/products` (Fermilab)
- `/cvmfs/cern.ch/sw/lcg` (CERN)
- `/opt/products` (Local installation)
- `/usr/local/products` (Local installation)

#### ROOT Installations
The script will look for ROOT in these locations:
- Already in PATH (if `root-config` is available)
- `/usr/local/root`
- `/opt/root`
- `/usr/local/root_install`
- `/Applications/root_src/root_install` (macOS)
- `/hep/sw/root-6.12.06/myroot`
- `$HOME/root`
- `$HOME/root_install`

## Usage

### Basic Usage
The script can be run from any directory:

```bash
# From the HIGHLAND_NEW directory
source highland/scripts/setup.sh

# From any other directory (absolute path)
source /path/to/HIGHLAND_NEW/highland/scripts/setup.sh
```

### Environment Variable Override
You can override the auto-detection by setting environment variables:

```bash
# Override products directory
export HIGHLAND_PRODUCTS_DIR="/path/to/your/products"
source highland/scripts/setup.sh

# Override ROOT directory
export HIGHLAND_ROOT_DIR="/path/to/your/root"
source highland/scripts/setup.sh
```

## What the Script Does

1. **Auto-detects HIGHLANDPATH** from the script location
2. **Finds products directory** automatically or uses environment variable
3. **Detects ROOT installation** automatically or uses environment variable
4. **Sets up environment variables**:
   - `ROOTSYS`: ROOT installation path
   - `PATH`: Includes ROOT and HIGHLAND binaries
   - `LD_LIBRARY_PATH`/`DYLD_LIBRARY_PATH`: Library paths for Linux/macOS
   - `ROOT_INCLUDE_PATH`: HIGHLAND include directory
   - `CMAKE_PREFIX_PATH`: For CMake configuration
5. **Sets package ROOT variables** for all HIGHLAND packages

## Troubleshooting

### No Products Found
If no products directory is found, the script will:
- Try to use system ROOT installation
- Set up basic HIGHLAND environment
- Continue with available tools

### ROOT Not Found
If ROOT is not found automatically:
- Check if ROOT is installed and in PATH
- Set `HIGHLAND_ROOT_DIR` environment variable
- Install ROOT in one of the standard locations

### Library Path Issues
The script automatically handles:
- Linux: `LD_LIBRARY_PATH`
- macOS: `DYLD_LIBRARY_PATH`
- Cross-platform compatibility

## Migration from Old Script

The old script had hardcoded paths for specific machines. The new script:
- Automatically works on any machine with appropriate products
- No need to modify the script for new machines
- Maintains backward compatibility through environment variables
- Provides better error handling and feedback

## Example Output

```
Setting up HIGHLAND environment...
Auto-detected products directory: /cvmfs/larsoft.opensciencegrid.org/products
Using GCC version: v9_3_0
Using GCC compiler: Linux64bit+3.10-2.17
Using ROOT version: v6_22_08d
Using ROOT compiler: Linux64bit+3.10-2.17-e20-p392-prof
Using TBB version: v2021_1_1
Using TBB compiler: Linux64bit+3.10-2.17-e20
Using CMake version: v3_22_2
Using CMake compiler: Linux64bit+3.10-2.17
ROOT environment set up: /cvmfs/larsoft.opensciencegrid.org/products/root/v6_22_08d/Linux64bit+3.10-2.17-e20-p392-prof
Setting up package ROOT variables...
HIGHLAND environment setup complete!
ROOTSYS: /cvmfs/larsoft.opensciencegrid.org/products/root/v6_22_08d/Linux64bit+3.10-2.17-e20-p392-prof
PATH includes: /path/to/highland/bin
Library paths updated for: linux-gnu
```
