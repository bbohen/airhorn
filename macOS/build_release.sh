#!/bin/bash
#
# Build Release Script for Airhorn
# Creates a release build and packages it in the releases directory
#

set -e

MACOS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$MACOS_DIR")"
RELEASES_DIR="$PROJECT_DIR/releases"

echo "🔨 Building Airhorn Release..."
cd "$MACOS_DIR"

# Clean and build
xcodebuild -project Airhorn.xcodeproj \
    -scheme Airhorn \
    -configuration Release \
    clean build \
    | grep -E "(BUILD|error|warning)" || true

if [ ${PIPESTATUS[0]} -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "✅ Build succeeded!"

# Find the built app
BUILT_APP=$(find ~/Library/Developer/Xcode/DerivedData/Airhorn-*/Build/Products/Release -name "Airhorn.app" -maxdepth 1 | head -1)

if [ -z "$BUILT_APP" ]; then
    echo "❌ Could not find built app"
    exit 1
fi

echo "📦 Packaging release..."

# Create releases directory if it doesn't exist
mkdir -p "$RELEASES_DIR"

# Create zip file
cd "$(dirname "$BUILT_APP")"
ditto -c -k --sequesterRsrc --keepParent Airhorn.app "$RELEASES_DIR/Airhorn.zip"

echo "✅ Release package created!"
echo ""
echo "📍 Location: $RELEASES_DIR/Airhorn.zip"
echo "📦 Size: $(du -h "$RELEASES_DIR/Airhorn.zip" | cut -f1)"
echo ""
echo "To install: unzip and drag Airhorn.app to /Applications/"

