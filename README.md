# Natevolve Ark

## Description

A C++ library to aid in the creation of Naturalistic Conlangs

Modules:

- Natevolve - global functions useful for everything, for instance allowing UTF-8 characters which is needed for the IPA stuff used in other modules
- Soundwarp - based on a set of defined sound change rules in a file, apply (in order) the set of sound changes to a word
- Romanizer - given a map of IPA symbols to characters, convert from IPA to a Romanization and back
- Morphball - given a set of morphological rules, a root word, and a desired gloss for the word, create the resulting form of the word
- Evauthor - given a set of grammar changes and a gloss for a sentence, create a new glossed sentence
- Wordup - given a phonological inventory and syllable rules, randomly generate words

The goal of this project is to serve as a solid underlying component for a GUI, Natevolve Studio (or other front-ends that wish to make use of the code).

## Build

Dependencies:

- gcc
- make

`make`

To create a test application run `make test` then run `./test.bin`

