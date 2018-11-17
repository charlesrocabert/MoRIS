<h1 align="center">:ant: MoRIS</h1>
<p align="center">
<em>Model of Routes of Invasive Spread</em>
<br />
<br />
<a href="https://github.com/charlesrocabert/MoRIS-development/releases/latest"><img src="https://img.shields.io/badge/version-RC 0.7.0-orange.svg" /></a>&nbsp;<a href="https://www.gnu.org/licenses/gpl-3.0"><img src="https://img.shields.io/badge/license-GPL v3-blue.svg" /></a>&nbsp;<a href="https://github.com/charlesrocabert/MoRIS-development/releases/latest"><img src="https://img.shields.io/badge/build-passing-green.svg" /></a>&nbsp;
</p>

## Introduction

<p align="justify">
Human-mediated dispersal acts as a vector for many exotic species, both at the introduction and secondary spread stages. Primary and secondary introductions arise from human-mediated long distance dispersal happening at global scales. Secondary spread occurs at smaller spatial and time scales (<em>e.g.</em> landscape) and results from either natural or human-mediated dispersal. Despite the importance of materials transportation (<em>e.g.</em> landscaping, construction) for the spread of invasive species, few studies have investigated short distance human-mediated dispersal and even less have tried to model it.
</p>

<p align="justify">
  <strong>MoRIS</strong> (<em>Model of Routes of Invasive Spread</em>) is a spatially explicit spread model designed to simulate invasive species dispersal by transport at local to regional spatial scales. <strong>MoRIS</strong> is an innovative, yet simple model, taking into account the road network topology to influence the direction of dispersal events. <strong>MoRIS</strong> is designed to minimize <em>a priori</em> making (<em>e.g.</em>, expert knowledge), to enable the estimation of human-mediated dispersal parameters based on a simple presence/absence locations dataset and to produce predictive maps of spread.
</p>

## Download
Download the latest pre-release: <a href="https://github.com/charlesrocabert/MoRIS-development/releases/latest"><img src="https://img.shields.io/badge/version-RC 0.7.0-orange.svg" /></a>

## How to build input files for MoRIS software?

Three datafiles are necessary to run MoRIS:
1. A text file describing the area of interest, named by default `map.txt`,
2. A text file describing the road network of interest, named by default `network.txt`,
3. A text file describing the presence-absence data, named by default `sample.txt`.

### 1. How to build the map file (area of interest)?
Here is an example:

| 1 | 0    | 2000 | 4000000 | 4000000 | 165 | 5.8144  | 1.63 |
| 2 | 2000 | 2000 | 4000000 | 4000000 | 245 | 6.7420  | 1.49 |
| 3 | 4000 | 2000 | 4000000 | 4000000 | 398 | 21.4843 | 1.40 |
| 4 | ...  | ...  | ...     | ...     | ... | ...     | ...  |



