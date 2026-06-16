# Algorithms
This document describes the design and requirements for an algorithm to be used in ReShuffler.

## Design
The algorithm is required to shuffle the bits by preserving all bits within a byte while shuffling the bits within the byte as a whole or within each 'bit' or chunk within a byte. It wouldn't matter how the shuffling itself happens as long it satisfies the requirements.

## Requirements
* All bits in a byte must be preserved. Any bit if modified without shuffling is not permitted.
* If the ReShuffler is operating in chunk mode: Only the chunks themselves should be shuffled and the bits within a chunk must all be preserved in the same order.
* If the ReShuffler is operating in bit mode: The bits can be shuffled in any manner within the byte but all bits must be preserved.
* The algorithm must be reversible for any input and configuration provided.

## List of algorithms

### Bit wise
* Mirror swap

### Chunk wise
* 