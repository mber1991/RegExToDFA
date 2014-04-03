RegExToDFA
==========

Will take a regular expression and convert it first to an NFA, a DFA, and then a Minimal DFA

Documentation:
------------------
In addition to the software developed you will need to turn in, at a minimum, two written documents, namely a user guide for our customers and a maintenance document to be used by future hires

Tokens to keep in mind:
-----------------------

**Operators:**
* () -- Grouping
* | -- Or
* * -- 0 or More
* \ -- Escape

**Characters:**
* a...z -- Matches a range of characters
* A...Z -- Matches range of characters
* 0...9 -- Matches range of numbers
* . -- Literally just one character, a period. IP Address stuff.
* / -- Forward slash for IP Addresses
* : -- Denotes a port will follow
