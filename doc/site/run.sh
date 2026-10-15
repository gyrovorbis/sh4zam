#!/bin/sh
# Local preview of the docs site on macOS and Linux:
#   ./run.sh            live preview at http://localhost:4321/
#   ./run.sh --build    full production build, served from dist/
# Needs Node.js 20+ and Doxygen; scripts/run.mjs checks both and installs the npm packages.
cd "$(dirname "$0")" || exit 1
if ! command -v node >/dev/null 2>&1; then
	echo "Node.js 20 or newer is needed: brew install node, or https://nodejs.org/" >&2
	exit 1
fi
exec node scripts/run.mjs "$@"
