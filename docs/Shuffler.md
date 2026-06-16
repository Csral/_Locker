# Shuffler
Shuffler is the most basic transformation engine used in Locker. The Shuffler requires a parameter known as modification-factor which helps the shuffler decide when to modify each bit.

Let 'M' be the value of modification-factor. The transformation engine will traverse a byte which is made of `chunk-wise` bits and every Mth bit will be affected and modified in accordance to the algorithm chosen.