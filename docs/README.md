{% include_relative mathjax.html %}

# Documentation
This is the documentation for "Locker". Firstly, the project is named "_Locker" just for fun and there's no hidden meaning behind the underscore, so "Locker" and "_Locker" refer to the same thing.

This documentation specifies application design, the design of algorithms used in Shufflers and Reshufflers.

# Locker
The application performs numerical transformations on bits to transform the bitstream into unreadable format. However, the file may or may not be objectively unreadable as this entirely depends on the configuration chosen which includes transformation algorithms, and their parameters. For instance, although statistically low it is very much possible that the configuration chosen for shuffler will perfectly be negated by the reshuffler or when encoding the files in multiple passes, the transformations can be undone.

The encoded files are **NOT** cryptographically secure. The algorithms are not verified for cryptographic safety or any sort of safety whatsoever. The primary goal of Locker is to make it difficult to read the original file directly by making it a random chunk of chaos and anyone with sufficient computational power can eventually recover the original data by exhaustively searching the configuration space.

Unless mentioned otherwise, consider all algorithms to be cryptographically and computationally insecure and the entire app to be an experimental encoder and decoder model. Furthermore, cryptographically secure algorithms can be filtered for by using the "--cryptographically-secure-only" option **WHICH IS NOT IMPLEMENTED UNTIL AS OF NOW**.

Locker derives its resistance to casual inspection from the number of possible transformation configurations.
* Identifying the Shuffler algorithm used
* Identifying the Shuffler configuration: modification factor and shuffler chunk size
* Identifying the Reshuffler type and algorithm used
* Identifying the Reshuffler configuration: The chunk size, block size and the seed (primary key) used.
* Reshuffler support extra arguments, however not every algorithm uses them the same way and some algorithms ignore them as a whole. Identifying the number of extra arguments used and the value for each of them.

This should not be interpreted as cryptographic security, as the effective search space may be substantially smaller than the theoretical configuration space and with sufficient computational power can absolutely brute force the entire search space which in the year of designing Locker is already significantly sufficient.

## Decoder and Reversible nature
Locker requires every algorithm and possible configuration to yield reversible results. Locker achieves that goal in a simpler way by subjecting every algorithm to be reversible by themselves and **assumes** that the nature is preserved when chained together which by logical and mathematical means is true. Simply put:

Let $f(x)$ and $g(x)$ be invertible functions such that:
$$f^{-1}(f(x)) = x \quad \text{and} \quad g^{-1}(g(x)) = x$$

Let $x$ be the input and $g(x)$ be a Shuffler transformation and $f(x)$ be a ReShuffler transformation. Assume both are individually invertible, and $y$ is the final encoded result:
>
> $$y = f(g(x))$$
> 
> To decode and recover the original input $x$, the inverse operations are applied sequentially:
> 
> 1. **Reverse the ReShuffler:**
>   $$f^{-1}(y) = f^{-1}(f(g(x))) = g(x)$$
> 
> 2. **Reverse the Shuffler:**
>   $$g^{-1}(g(x)) = x$$

Thus, we prove that by ensuring every algorithm is independently reversible the encoded result from Shuffler and ReShuffler in any configuration are mathematically reversible and the data is preserved.

Given identical inputs and identical configurations, Locker produces identical outputs. Any source of randomness used by an algorithm must be derived from explicitly supplied parameters such as seeds.

## Boundary condition issues
Although the reversibility property follows mathematically from the assumptions above, Locker cannot exhaustively verify that every implementation, configuration, and algorithm adheres to those assumptions.

Locker transformations operate on logical units derived from user-supplied parameters. Certain parameter combinations may produce incomplete logical units near the end of the input stream.

The precise cause of these failures may not always be readily identifiable and may arise from interactions between implementation details, computational constraints, and specific parameter combinations. Some combinations have been observed to yield non-reversible outputs for about 30-40 bits towards end of file.

However, for most cases the algorithms and configurations should yield a reversible output from Locker. Users should therefore verify integrity after decoding and before removing the original files. The steps are mentioned below.

### Reversible integrity check

Always keep backups of original files and choose to remove the original file only after integrity check is performed.
The integrity check feature is not currently implemented.

### Manual integrity verification (recommended)
To verify correctness manually:

1. Encode the file.
2. Decode the output file using the exact same parameters.
3. Compare the decoded file with the original one, ideally using tools such as `diff` or `fc`.

If the files are different then the integrity verification has failed. In this case, locker cannot be used either on the given file or with current configuration or both.

**NOTE:** If the `diff` command produces any output, then the files are different.
**NOTE:** Locker will soon be bundled with an integrity checker and a built-in `diff` utility along with checksum validation.

## Redefining bits and bytes
The entire idea of Locker configuration is its ability to redefine what bit and bytes fundamentally mean and then transforming upon its own understanding of said bits and bytes. This makes the encoded files very difficult for a computer to understand or read.

### Parameter: Chunk size
When used with Shuffler, the chunk size parameter specifies the number of bits that make up a byte. Thus, when the transformation engine is required to do something for every X bits, it would assume the count to reset after a byte but the byte is no longer 8-bits.

However, when used with ReShuffler, the chunk size parameter specifies the number of bits that make up a bit itself. To understand this better: when we speak of a byte we mean 8-bits but now, each bit may be made of N-bits and hence a byte is 8*N-bits.

### Parameter: Block size
This is a ReShuffler only option which specifies the number of bits that make a byte. So if chunk-size is C and block-size is B, then each byte is C*B bits.

# Shuffler
Shuffler is the most basic transformation engine used in Locker. The Shuffler requires a parameter known as modification-factor which helps the shuffler decide when to modify each bit. Thus, while traversing a byte at every modification-factor bit the engine will perform its modification based on the algorithm chosen.

For further documentation upon the Shuffler, read the [Shuffler design](Shuffler.md) and [Shuffler algorithms](Shuffler/Shuffler_algorithms.md).

# ReShuffler
ReShuffler is the transformation engine that performs the real "Shuffling". Unlike the Shuffler which modifies the bits, ReShuffler preserves the bits but shuffles them.

For further documentation upon the ReShuffler, read the [ReShuffler Design](Reshuffler.md) and [ReShuffler algorithms](Shuffler/Shuffler_algorithms.md).

# Building from Source
See [build.md](build.md) for build instructions.

# License
This project is licensed under GPLv3. For more information, see the [LICENSE](LICENSE) file.