# GenOS Store

A browser-based app store prototype for the GenOS repository.

## Use

Open `index.html` through a local web server or a deployed web preview. Search/filter the catalog, open listed web prototypes, and save an installed-app list in browser localStorage.

## Scope and limitations

- “Install” only records the selected app in this browser; it does not download or install binaries.
- “Open” launches existing web prototype pages. The Python CMD listing links to its feature branch and is not part of main's current app tree unless its PR is merged.
- The store does not yet have a backend, package repository, signatures, dependency management, updates, or native `.gem` installation integration.
- This is not a native GenKernel application; the current kernel does not execute these HTML apps natively.
