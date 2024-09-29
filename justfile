set windows-shell := ["pwsh", "-NoLogo", "-NoProfileLoadTime", "-Command"]

configure:
    cmake -S . -B build

build:
    cmake --build build -j4

run: build
    ./build/bin/TowerDefence/TowerDefence