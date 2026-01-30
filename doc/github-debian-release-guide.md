# Distributing Debian Packages via GitHub Releases - Assessment

## Feasibility Assessment: **Highly Feasible** ✅

Your setup is well-positioned for this:
- Existing CI/CD with GitHub Actions
- Clean, minimal dependencies
- Proper Multi-Arch support already configured
- DEP-14 awareness (though full compliance is optional for non-official repos)

## Distribution Approaches (Ranked by Recommendation)

### **1. GitHub Releases with CI-built .deb files** (Recommended)

**How it works:**
- Build `.deb` files in GitHub Actions on tagged releases
- Upload as release assets
- Users download and install with `sudo dpkg -i`

**Pros:**
- Simple, widely used pattern
- No infrastructure to maintain
- Works with existing tooling
- Clear versioning via git tags

**Cons:**
- Users must manually manage updates
- No automatic dependency resolution
- Requires manual `apt-get install -f` after dpkg

**Implementation outline:**
```yaml
# .github/workflows/release.yml
on:
  push:
    tags:
      - 'v*'
      - 'debian/*'  # DEP-14 style tags

jobs:
  build-deb:
    strategy:
      matrix:
        os: [ubuntu-20.04, ubuntu-22.04, ubuntu-24.04]
    runs-on: ${{ matrix.os }}
    steps:
      - uses: actions/checkout@v4
      - name: Build Debian package
        run: dpkg-buildpackage -us -uc -b
      - name: Upload to Release
        uses: softprops/action-gh-release@v1
        with:
          files: ../*.deb
```

### **2. GitHub Releases + Custom APT Repository** (Best for frequent updates)

**How it works:**
- Build `.deb` files via CI
- Host APT repository metadata on GitHub Pages or separate branch
- Users add your repo to `/etc/apt/sources.list.d/`

**Pros:**
- Standard `apt install` workflow
- Automatic updates via `apt upgrade`
- Dependency resolution

**Cons:**
- More complex setup
- Need to maintain APT repository structure
- GPG key management for signing

**Tools to consider:**
- [`aptly`](https://www.aptly.info/) - full-featured APT repository management
- [`reprepro`](https://salsa.debian.org/brlink/reprepro) - lightweight alternative
- [`deb-s3`](https://github.com/deb-s3/deb-s3) if using S3/compatible storage

### **3. GitHub Container Registry (OCI format)** (Emerging pattern)

**How it works:**
- Package as OCI artifact in GitHub Container Registry
- Users pull with container tools or purpose-built clients

**Pros:**
- Modern approach
- Version tracking built-in
- No separate hosting needed

**Cons:**
- Less familiar to traditional Debian users
- Requires additional tooling

## DEP-14 Considerations

**Current state:** Your repo uses "master/main" branch with `debian/` folder alongside source.

**DEP-14 compliance options:**

1. **Relaxed (Current)** - Keep as-is, works fine for GitHub releases
   - Tag format: `v1.2.0` or `debian/1.2.0-1`

2. **Partial DEP-14** - Adopt tag naming only
   - Use: `debian/1.2-1`, `upstream/1.2`
   - Branch structure stays same

3. **Full DEP-14** - Separate branches
   - `upstream` branch = pristine source
   - `debian/master` branch = packaging
   - More complex, mainly for official Debian workflow

**Recommendation:** Partial DEP-14 (tag naming only) is a good middle ground.

## Recommended Implementation Plan

**Phase 1: Basic Release Automation**
1. Create GitHub Actions workflow for releases
2. Build `.deb` for Ubuntu 20.04, 22.04, 24.04 (different `debhelper-compat` available)
3. Upload to GitHub Releases on version tags
4. Document installation in README

**Phase 2: Enhanced (Optional)**
1. Add GPG signing of packages
2. Create checksums/signatures
3. Consider APT repository if user base grows

## Key Best Practices

1. **Version synchronization:**
   - Keep `debian/changelog` in sync with `CMakeLists.txt` version
   - Use git tags: `v1.2.0` or `debian/1.2-1`

2. **Build on target distributions:**
   - Build separate `.deb` for each Ubuntu/Debian version
   - Name files: `libmoirai_1.2-1_amd64_ubuntu22.04.deb`

3. **Documentation:**
   - Add installation instructions to README
   - Provide GPG key if signing packages
   - Include upgrade/uninstall procedures

4. **Lintian clean:**
   - Run `lintian` in CI to catch packaging issues
   - Your existing `debian/libmoirai.lintian-overrides` will carry through

## Version Compatibility Notes

**Your package should work on:**
- ✅ Debian 10 (Buster) and later
- ✅ Debian 11 (Bullseye) 
- ✅ Debian 12 (Bookworm)
- ✅ Debian 13 (Trixie/testing)
- ✅ Ubuntu 18.04 LTS and later

**Version-specific constraints:**
- `debhelper-compat (= 11)` requires Debian 10+
- `cmake_minimum_required(VERSION 3.10)` requires Debian 10+

## Related Resources

- [DEP-14](https://dep-team.pages.debian.net/deps/dep14/) - Debian git repository layout
- [Debian New Maintainers' Guide](https://www.debian.org/doc/manuals/maint-guide/)
- [GitHub Actions for Debian packaging](https://github.com/marketplace?type=actions&query=debian)
- [aptly](https://www.aptly.info/) - APT repository management tool
