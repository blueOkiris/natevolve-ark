// API for Morphball module

#pragma once

#include <err.hpp>

namespace natevolve {
    namespace morphball {
        // Whether the gloss causes a totally new word to be used,
        // replaces the beginning of the word, replaces the end of the word,
        // or replaces a piece in the middle of the word
        enum class Morph {
            None,
            Change,
            Suffix,
            Prefix,
            Infix
        };

        enum class Gloss {
            // 1st Person
            P1,

            // 2nd Person
            P2,

            // 3rd Person
            P3,

            // Agent
            A,

            // Ablative
            ABL,

            // Absolutive
            ABS,

            // Accusative
            ACC,

            // Adjective
            ADJ,

            // Adverbial
            ADV,

            // Agreement
            AGR,

            // Allative
            ALL,

            // Antipassive
            ANTIP,

            // Applicative
            APPL,

            // Article
            ART,

            // Auxiliary
            AUX,

            // Benefactive
            BEN,

            // Causative
            CAUS,

            // Classifier
            CLF,

            // Comitative
            COM,

            // Complementizer
            COMPL,

            // Conditional
            COND,

            // COP
            COP,

            // Converb
            CVB,

            // Dative
            DAT,

            // Declarative
            DECL,

            // Definite
            DEF,

            // Demonstrative
            DEM,

            // Determiner
            DET,

            // Distal
            DIST,

            // Distributive
            DISTR,

            // Dual
            DU,

            // Durative
            DUR,

            // Ergative
            ERG,

            // Exclusive
            EXCL,

            // Feminine
            F,

            // Focus
            FOC,

            // Future
            FUT,

            // Genitive
            GEN,

            // Imperative
            IMP,

            // Inclusive
            INCL,

            // Indicative
            IND,

            // Indefinite
            INDF,

            // Infinitive
            INF,

            // Instrumental
            INS,

            // Intransitive
            INTR,

            // Imperfective
            IPFV
        };

        struct MorphRule {
            // -------- Functions --------

            // -------- Members --------

            const Morph type;
            const Gloss when;
        };
    }
}

