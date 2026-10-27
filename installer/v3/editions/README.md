# GenOS V3 edition profiles

These JSON files define three separate preview labels: Home, Ultimate, and Enterprise LTSC. They are metadata/specification only. The current kernel and payload do not yet implement edition-specific capabilities, so separate labels must not be marketed as distinct feature-complete products.

- **Home**: personal-use preview profile.
- **Ultimate**: advanced-use preview profile.
- **Enterprise LTSC**: enterprise-oriented preview profile only. There is no LTSC servicing lifecycle, support window, security-maintenance guarantee, or release cadence yet.

A release pipeline still needs to build each profile as a separate ISO, run compilation/package checks and QEMU BIOS boot tests, publish checksums, and publish release assets only after all three pass. UEFI and physical-hardware validation remain separate work.
