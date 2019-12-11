# Application

## Read in OSM file

## First pass over all ways

- Ignore ways without tag `highway=*`
- Generate a table keyed by node ref IDs, valued by number of occurences of that node ID as a node ref

## Second pass over all ways
- Lookup node ref IDs in the hashtable
    - if any of the node ref IDs have a value > 1 in the table, mark those node refs as "anchor" nodes
    - anchor nodes are nodes that cannot be discarded. First and last nodes are also anchor nodes.

- If a way is a "simple" way, ie no node IDs with occurence value > 1
    - run `normalization` on that way, copy tags to it and write to disk
- If a way is a "non-simple" way
    - run normalization on the segments between each anchor node
    - Write an OSM way object with the resulting normalized segments

# Normalization

Prioritize having a regular interval of segments that's somewhere close to the normalization
distance than have segments that are exactly the normalization distance with some leftover bits.

The process of writing a new osmium way object, given an osmium way object and the normalization distance (e.g. 15m).
Output should be:
- a new osmium way object
- new node objects corresponding to the new way's node refs
- mapping between old and new way ids

## Process
Given a way that is 212m long, drawn with four nodes are irregular intervals. And a normalization distance of 10m.
The way has 3 way_segments: [(ab),(bc),(cd)]. `a` and `d` are anchor nodes and their location values should not be changed.

```
     185     8  19
a-----------b-c---d
```

- On first pass through nodes in way, detect way segments that are less than the normalization distance [(bc)]
- Run the following segment calculation on remaining segments [(ab),(cd)]:

```
// get rough number of segments
num_segments = floor(185/10)
// = 18;

// get leftover distance
leftover_m = 185 % 10;
// = 5 m

// The way segment (ab) should be normalized to have 18 segments (19 nodes) that are each 10 + (5/18)m long.
```
