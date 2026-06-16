# Algorithms
This document describes the design and requirements for an algorithm to be used in Shuffler. The Shuffler being the simplest transformation engine, doesn't have many requirements.

## Design
The algorithm is required to do some modification on bits based on some constant logic while adhering to all requirements.

## Requirements
* The algorithm must be reversible for any input and configuration provided.

**NOTE:** In most cases, validating the algorithm for 8-bit bytes should be sufficient although it is recommended to validate with random configurations.

## List of algorithms
* State Machine
* Prefix Parity
* Simple NOT