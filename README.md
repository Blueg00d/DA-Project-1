# Scientific Conference Organization Tool (DA-Project-1)

## Overview
The Scientific Conference Organization Tool is a C++ application designed to optimally assign scientific submissions to reviewers based on expertise, availability, and specific configuration parameters. The system models the assignment problem as a Bipartite Matching problem and solves it efficiently using flow networks and Max-Flow algorithms.

## Key Features
* **Max-Flow Assignment**: Uses the Ford-Fulkerson algorithm to find the optimal assignment of reviewers to submissions.
* **Configurable Expertise Levels**: Supports different strictness levels for assignments (e.g., matching only primary topics vs. allowing secondary topic overlaps).
* **Risk Analysis**: Identifies "risky" reviewers whose absence would cause submissions to fall short of their minimum required reviews, using an optimized incremental isolation technique.
* **Robust CSV Parsing**: Safely bypasses corrupted dataset lines (e.g., invalid data types or out-of-range limits) instead of crashing the application.
* **Dual Execution Modes**: Provides an interactive CLI menu and a headless batch-processing mode (`-b`) for automation.

## Architecture

The project is divided into specialized modules to enforce a strong separation of concerns:

### 1. Core (`Brainer`)
The `Brainer` class acts as the central orchestrator of the assignment pipeline. It is responsible for:
* Building the flow network by connecting a `Source` to `Reviewers`, `Reviewers` to `Submissions`, and `Submissions` to a `Sink`.
* Executing the assignment algorithm (`runAssignment`).
* Interpreting edge flows into actionable matches and identifying submissions that lack enough reviews.
* Running the risk analysis pipeline.

### 2. Data Structures (`Graph`)
A custom template-based `Graph` class implemented specifically for flow networks.
* Employs an `std::unordered_map` to achieve O(1) time complexity for vertex lookups (`findVertex`).
* Implements the `fordFulkerson` max-flow algorithm utilizing Depth-First Search (DFS) to find augmenting paths.
* Includes a specialized `resumeFordFulkerson` method to calculate incremental flows without resetting the entire network state.

### 3. I/O & Parsing (`Parser`)
Handles dataset ingestion.
* Extracts structured data for `#Submissions`, `#Reviewers`, `#Parameters`, and `#Control` configurations.

## Algorithmic Highlights

### Ford-Fulkerson Implementation
The assignment is mapped to a directed graph where edge capacities represent the maximum number of reviews a reviewer can do, and the minimum reviews a submission requires. The Ford-Fulkerson algorithm calculates the maximum possible valid assignments in `O(E * f_max)` time.

### Highly Optimized Risk Analysis
Instead of naively rebuilding the entire graph for every reviewer to test the network's resilience, the `runRiskAnalysis` function uses an **incremental isolation** technique:
1. A baseline max flow is established and the state is cached in memory.
2. For each reviewer, their connection to the source is temporarily severed (weight and flow set to 0).
3. The specific flow that traversed through that isolated reviewer is canceled out locally.
4. `resumeFordFulkerson` is called to attempt to route the canceled flow through other available reviewers.
5. The original network state is restored in `O(E)` time from the cache, ready for the next iteration.

This reduces the complexity drastically compared to a brute-force approach.

## Usage

### Interactive Mode
Launch the application without arguments to access the interactive CLI menu:

```bash
cd DA-Project-1
make
./myProg
```

From the menu, you can sequentially navigate options:

1. Read File

1. Print File (to verify parsed data)

1. Change Parameters (modify generation strictness, min/max reviews)

1. Build Graph

1. Run Ford Fulkerson Algorithm

1. Interpret Graph Flow

1. Save output

1. Test all Inputs

1. Debug

### Batch Mode
For automated processing pipelines, run the executable with the `-b` flag, specifying the input and output filenames:

```bash
./myProg -b <input_filename.csv> <output_filename.csv>
```
_**Note**: The application expects inputs to be located in `samples/input/` and saves outputs to `samples/generated/`._

## Input File Format
The parser expects datasets to contain specific block headers:

`#Submissions`: `Id, Title, Author, Email, PrimaryTopic, SecondaryTopic`

`#Reviewers`: `Id, Name, Email, PrimaryTopic, SecondaryTopic`

`#Parameters`: Contains keys like `MinReviewsPerSubmission` and `MaxReviewsPerReviewer`

`#Control`: Contains execution flags like `GenerateAssignments` and `RiskAnalysis`

## Debugging
A dedicated `handleDebugger` module is available in the interactive menu (Option 9) to inspect the Graph Structure, Graph Flow, and Interpretation Results directly.

## Known Issues
Memory Management: The `Graph` template currently utilizes raw pointers for dynamic allocation of Vertices and Edges but lacks a complete implementation in its destructor (`~Graph() = default;`). This causes a memory leak when a `Graph` instance is destroyed.