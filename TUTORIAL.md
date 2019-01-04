<h1 align="center">:ant: MoRIS tutorial</h1>
<p align="center">
J. M. W. Gippet – July 2018
</p>

<br/>
<br/>
This tutorial explains to the user how to build MoRIS input files, in order to perform HMD (Human Mediated Dispersal) parameters estimation.

MoRIS requires 3 input files:
- <strong>The map file</strong>, describing a discretized version of the area of interest, made of "cells" of a given size (<em>e.g.</em> squares, or hexagons, or anything else),
- <strong>The network file</strong>, describing the network connecting cells on the map (<em>e.g.</em> a road network, but any network could work, such as a river system or an aerial system),
- <strong>The sample file</strong>, describing the sampling effort of the invasive species of interest, cell by cell. A presence-absence scheme is required.

## Table of contents
1. [How to build the map file](#map)
2. [How to build the network file](#network)
3. [How to build the sample file](#sample)

## 1. How to build the map file (area of interest)?

### 1.1 File name and structure

By default, the map file must be named `map.txt` (MoRIS offers the possibility to choose optional filenames).

Here is an example of the file structure:

| 1 | 0    | 2000 | 4000000 | 4000000 |
|---|------|------|---------|---------|
| 2 | 2000 | 2000 | 4000000 | 4000000 |
| 3 | 4000 | 2000 | 4000000 | 4000000 |
| 4 | ...  | ...  | ...     | ...     |

- Column 1 contains <strong>cell identifiers</strong>, which must be positive integer numbers (usually from `1` to `n` if the map contains `n` cells),
- Column 2 contains <strong>the x-axis coordinate of the cell</strong>, here in meters. The coordinate of the centroid is preferred,
- Column 3 contains <strong>the y-axis coordinate of the cell</strong>, here in meters. The coordinate of the centroid is preferred,
- Column 4 contains <strong>the area of the cell</strong>, here in square meters,
- Column 5 contains <strong>the suitable habitat area of the cell</strong>, in square meters.

### 1.2 How to get it?

1. <strong>Choose an extent.</strong> Define a <em>working area</em> (red square on figure 1) around your <em>area of interest</em>, namely: the area where you have presence/absence data on your favorite invasive species (red/green dots on figure 1).
<br/>
This <em>working area</em> must be centered around and larger by at least 100% than the invaded part of your area of interest, except for natural barriers (the ocean for example, left/West side on fig1). Do not underestimate the importance of the bufferring area, to avoid estimation biases in <strong>MoRIS</strong>.





