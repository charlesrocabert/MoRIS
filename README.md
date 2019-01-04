<h1 align="center">:ant: MoRIS</h1>
<p align="center">
<em>Model of Routes of Invasive Spread</em>
<br />
<br />
<a href="https://github.com/charlesrocabert/MoRIS/releases/latest"><img src="https://img.shields.io/badge/version-RC 0.7.0-orange.svg" /></a>&nbsp;<a href="https://www.gnu.org/licenses/gpl-3.0"><img src="https://img.shields.io/badge/license-GPL v3-blue.svg" /></a>&nbsp;<a href="https://github.com/charlesrocabert/MoRIS/releases/latest"><img src="https://img.shields.io/badge/build-passing-green.svg" /></a>&nbsp;
</p>

## Table of contents
1. [Introduction](#introduction)
2. [Publications](#publications)
3. [Copyright](#copyright)
4. [License](#license)
5. [Download](#download)
6. [Installation instructions](#installation_instructions)
7. [Tutorial](#tutorial)

## 1. Introduction <a name="introduction"></a>

<p align="justify">
Human-mediated dispersal acts as a vector for many exotic species, both at the introduction and secondary spread stages. Primary and secondary introductions arise from human-mediated long distance dispersal happening at global scales. Secondary spread occurs at smaller spatial and time scales (<em>e.g.</em> landscape) and results from either natural or human-mediated dispersal. Despite the importance of materials transportation (<em>e.g.</em> landscaping, construction) for the spread of invasive species, few studies have investigated short distance human-mediated dispersal and even less have tried to model it.
</p>

<p align="justify">
  <strong>MoRIS</strong> (<em>Model of Routes of Invasive Spread</em>) is a spatially explicit spread model designed to simulate invasive species dispersal by transport at local to regional spatial scales. <strong>MoRIS</strong> is an innovative, yet simple model, taking into account the road network topology to influence the direction of dispersal events. <strong>MoRIS</strong> is designed to minimize <em>a priori</em> making (<em>e.g.</em>, expert knowledge), to enable the estimation of human-mediated dispersal parameters based on a simple presence/absence locations dataset and to produce predictive maps of spread.
</p>

## 2. Publications <a name="publications"></a>


## 3. Copyright <a name="copyright"></a>

Copyright &copy; 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet.
All rights reserved.

## 4. License <a name="license"></a>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.

## 5. Download <a name="download"></a>
Download the latest pre-release: <a href="https://github.com/charlesrocabert/MoRIS-development/releases/latest"><img src="https://img.shields.io/badge/version-RC 0.7.0-orange.svg" /></a>

## How to build input files for MoRIS software?

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




