<h1 align="center">:ant: MoRIS tutorial</h1>
<p align="center">
J. M. W. Gippet – July 2018
</p>

This role of this tutorial is to describe to the user how to build MoRIS input files, in order to perform HMD (Human Mediated Dispersal) parameters estimation. MoRIS requires 3 input files:
- <strong>The map file</strong>, describing a discretized version of the area of interest, made of "cells" of a given size (<em>e.g.</em> squares, or hexagons, or anything else),
- <strong>The network file</strong>, describing the network connecting discrete areas of the map (<em>e.g.</em> a road network, but any network could work, such as a river system or an aerial system),
- <strong>The sample file</strong>, describing the sampling effort of the invasive species of interest, cell by cell. A presence-absence scheme is required.

## Table of contents
1. [Map file](#map)
2. [Network file](#network)
3. [Sample file](#sample)

## 1. How to build input files for MoRIS software?

Three datafiles are necessary to run MoRIS:
1. A text file describing the area of interest, named by default `map.txt`,
2. A text file describing the road network of interest, named by default `network.txt`,
3. A text file describing the presence-absence data, named by default `sample.txt`.

### 1. How to build the map file (area of interest)?
Here is an example:

| 1 | 0    | 2000 | 4000000 | 4000000 |
|---|------|------|---------|---------|
| 2 | 2000 | 2000 | 4000000 | 4000000 |
| 3 | 4000 | 2000 | 4000000 | 4000000 |
| 4 | ...  | ...  | ...     | ...     |

- Column 1 contains cell identifiers, which must be positive integer numbers (usually from `1` to `n`),
- Column 2 contains the x-axis coordinate of the cell, here in meters. The coordinate of the cell centroid is preferred,
- Column 3 contains the y-axis coordinate of the cell, here in meters. The coordinate of the cell centroid is preferred,
- Column 4 contains the area of the cell, here in square meters,
- Column 5 contains the suitable habitat area of the cell, in square meters.

1. First define a working area (fig1 red square) around your area of interest, namely: the area where you have presence/absence data on your favorite invasive species (fig1 red/green dots). This working area must be centered around and larger by at least 100% than the invaded part of your area of interest, except for natural barriers (the ocean for example, left/West side on fig1). Do not underestimate the importance of the bufferring area, to avoid estimation biases in <strong>MoRIS</strong>.

<strong>[Writing in progress ...]</strong>




