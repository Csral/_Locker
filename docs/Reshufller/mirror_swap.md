# Mirror swap algorithm
A simple in-place algorithm that reverses (mirrors) the chunks in Reshuffle block. The parameter seed in this case is called position and is used to decide where to place the mirror.

## ExtArgs
Mirror swap uses at most one single extended argument. This argument is called 'direction'. By default, all chunks to the left of mirror are mirrored but any value other than '0' as direction inverts this nature. This is passed as a boolean to the encoder which if set to true would invert the default behavior.

## Decoding
To decode/invert mirror swap reshuffle algorithm given the same 'seed' and 'direction' used in encoding, the decoder is expected to do the following:
* Place the mirror at 'seed'.
* Decide the direction.
* Invert the chunks in that direction.

**NOTE:** This simply means that the encoder can act as the decoder.