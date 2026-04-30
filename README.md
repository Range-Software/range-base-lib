# Range Base Library

Foundational C++ utility library (Qt-based) that underpins the entire Range FEA library stack.

## What it provides

**Math primitives** — `RRVector`, `RIVector`, `RR3Vector`, `RRMatrix`, `RIMatrix` for real/integer vectors and matrices; `RLimitBox`, `RPlane`, `RLocalDirection` for spatial geometry.

**Job & task system** — `RJob`/`RJobManager` for threaded work (QRunnable-based); `RToolTask`/`RToolAction`/`RToolInput` for tool execution pipelines.

**Logging** — `RLogger` with trace/debug/info/warning/error levels; `RMessage` for structured message formatting.

**Utilities** — `RError` (exception types), `RArgumentParser` (CLI argument parsing), `RStatistics` (min/max/avg/median), `RStopWatch` (performance timing), `RFileTools` (file I/O helpers), `RProgress` (progress tracking), `RVersion` (version management), `RLocker` (thread synchronisation).

## Class prefix

All public classes use the `R` prefix (e.g. `RLogger`, `RRVector`).

## Dependencies

- Qt 6.10+
- C++17
