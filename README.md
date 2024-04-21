<p align="center">
<img src="docs/pics/logo.png" width=400>
<br/>
</p>

<p align="center">
<strong>Model of Routes of Invasive Spread</strong>
<br />
<br />
<a href="https://github.com/charlesrocabert/MoRIS/releases/latest"><img src="https://img.shields.io/github/release/charlesrocabert/MoRIS/all.svg" /></a>&nbsp;
<a href="https://github.com/charlesrocabert/MoRIS/actions"><img src="https://github.com/charlesrocabert/MoRIS/workflows/CMake/badge.svg" /></a>&nbsp;
<a href="https://github.com/charlesrocabert/MoRIS/LICENSE.html"><img src="https://img.shields.io/badge/License-GPLv3-blue.svg" /></a>
</p>

-----------------

<p align="justify">
Human-mediated dispersal acts as a vector for many exotic species, both at the introduction and secondary spread stages. Primary and secondary introductions arise from human-mediated long distance dispersal happening at global scales. Secondary spread occurs at smaller spatial and time scales (<em>e.g.</em> landscape) and results from either natural or human-mediated dispersal. Despite the importance of materials transportation (<em>e.g.</em> landscaping, construction) for the spread of invasive species, few studies have investigated short distance human-mediated dispersal and even less have tried to model it.
</p>

<p align="justify">
  MoRIS (<em>Model of Routes of Invasive Spread</em>) is a spatially explicit spread model designed to simulate invasive species dispersal by transport at local to regional spatial scales. MoRIS is an innovative, yet simple model, taking into account the road network topology to influence the direction of dispersal events. MoRIS is designed to minimize <em>a priori</em> making (<em>e.g.</em>, expert knowledge), to enable the estimation of human-mediated dispersal parameters based on a simple presence/absence locations dataset and to produce predictive maps of spread.
</p>

<p align="justify">
  To learn how to use MoRIS, first install the <a href="https://github.com/charlesrocabert/MoRIS/releases/latest">latest release</a>, and follow the <a href="installation_instructions">installation instructions</a> and the <a href="first_usage">first usage</a> guide. MoRIS software requires input files built in a certain way. To learn how to build this files, read the <a href="https://github.com/charlesrocabert/MoRIS/blob/master/INPUT_FILES_TUTORIAL.md">input files tutorial</a>.
</p>

## Table of contents
- [Publications and communications](#publications)
- [Authors](#authors)
- [Citing MoRIS](#citing)
- [Installation instructions](#installation_instructions)
  - [Supported platforms](#supported_platforms)
  - [Dependencies](#dependencies)
  - [Software compilation](#software_compilation)
- [First usage](#first_usage)
  - [Ready-to-use examples](#examples)
  - [Run a MoRIS simulation](#run_simulation)
  - [Couple MoRIS to the optimization algorithm](#optimize)
- [Input files tutorial](#tutorial)
- [Copyright](#copyright)
- [License](#license)

## Publications and communications <a name="publications"></a>
• C. Rocabert, S. Fenet, B. Kaufmann and J. M.W. Gippet. Accounting for the topology of road networks to better explain human-mediated dispersal in terrestrial landscapes. <em>Ecography</em>, e07068 (2024). https://doi.org/10.1111/ecog.07068

• J. M.W. Gippet, S. Fenet, A. Dumet, B. Kaufmann and C. Rocabert (2016, August). MoRIS: Model of Routes of Invasive Spread. Human-mediated dispersal, road network and invasion parameters. In <em>Proceedings of IENE 2016 conference. 5th International Conference on Ecology and Transportation: Integrating Transport Infrastructures with Living Landscapes</em> (Lyon, France). http://hal.cirad.fr/LJK_MAD_STEEP/hal-01412280v1

• J. M.W. Gippet, C. Rocabert, S. Fenet, A. Dumet and B. Kaufmann (2015, July). Modeling and evaluating human-mediated dispersal mechanisms at landscape scale: a study of road network and invasion parameters for Lasius neglectus ants invasive species. In <em>Proceedings of World Conference on Natural Resource Modeling</em> (Bordeaux, France). https://hal.archives-ouvertes.fr/hal-01242828/

## Authors <a name="authors"></a>
MoRIS is being developed by <a href="https://charlesrocabert.github.io/" target="_blank">Charles Rocabert</a>, <a href="https://jgippet.github.io/" target="_blank">Jérôme M.W Gippet</a> and <a href="https://perso.liris.cnrs.fr/sfenet/homepage/" target="_blank">Serge Fenet</a>.

#### Major contributors
* Aymeric Bonnamour
* Bernard Kaufmann

#### Other contributors
* Vivek Srivastava

<p align="center">
<table>
    <tr>
        <td><a href="http://umr5023.univ-lyon1.fr/"><img src="docs/pics/logo-lehna.png" height="60px"></a></td>
        <td><a href="https://liris.cnrs.fr/"><img src="docs/pics/logo-liris.png" height="60px"></a></td>
        <td><a href="http://www.in2p3.fr/"><img src="docs/pics/logo-in2p3.png" height="60px"></a></td>
        <td><a href="http://www.ixxi.fr/"><img src="docs/pics/logo-ixxi.jpeg" height="60px"></a></td>
        <td><a href="https://www.unil.ch/central/en/home.html/"><img src="docs/pics/logo-lausanne-univ.png" height="60px"></a></td>
    </tr>
</table>
</p>

## Citing MoRIS <a name="citing"></a>
If you are using MoRIS software for research purpose and publication, please cite the following manuscript: https://doi.org/10.1111/ecog.07068

## Installation instructions <a name="installation_instructions"></a>
Download the <a href="https://github.com/charlesrocabert/MoRIS/releases/latest">latest release</a> of MoRIS, and save it to a directory of your choice. Open a terminal and use the <code>cd</code> command to navigate to this directory. Then follow the steps below to compile and build the executables.

### Supported platforms <a name="supported_platforms"></a>
MoRIS software has been successfully tested on Unix/Linux and macOS platforms.

### Dependencies <a name="dependencies"></a>
* A C++ compiler (GCC, LLVM, ...),
* CMake (command line version),
* GSL for C/C++,
* CBLAS for C/C++,
* Python &ge; 3 (Packages CMA-ES and numpy are required),
* R (**optional**, packages <code>ggplot2</code>, <code>cowplot</code> and <code>sf</code> are required).

### Software compilation <a name="software_compilation"></a>

#### User mode
To compile MoRIS, run the following instructions on the command line:

    cd cmake/

and

    bash make.sh

#### Debug mode
To compile the software in DEBUG mode, use <code>make_debug.sh</code> script instead of <code>make.sh</code>:

    bash make_debug.sh

This mode should only be used for test or development phases.

#### Executable files emplacement
Binary executable files are in <code>build/bin</code> folder.

## First usage <a name="first_usage"></a>
Once MoRIS has been installed, follow the next steps for a first usage of the software.

### Ready-to-use examples <a name="examples"></a>
Ready-to-use examples are available in the folder <code>examples</code>:

• <code>run_single_simulation.sh</code>: This script will run a single MoRIS simulation with given human-mediated dispersal parameters. You can execute it using the following command line:

    bash run_single_simulation.sh

At the end of the simulation, a figure providing an overview of the simulation result is created (<code>single_simulation.pdf</code>). You can edit the parameter values at will to test the behaviour of the model. See below for a full description of the parameters.

• <code>run_optimization.sh</code>: This script will launch an optimization session by running multiple MoRIS simulations, and trying to find the solution matching the experimental data at best. You can execute it using the following command line:

    bash run_optimization.sh

You can track the ongoing optimization in the text-file <code>optimization.txt</code>. At the end of the optimization session, MoRIS writes the best parameters set in a text-file. Optimization parameters are defined in a specific file named <code>parameters.txt</code>. See below for a full description of the parameters.

### Run a MoRIS simulation <a name="run_simulation"></a>
To run one MoRIS simulation, place yourself in the folder <code>examples</code> using the <code>cd</code> command, and execute the following command line:

    ../build/bin/MoRIS_run <parameters>

The command line parameters are described below. The description is also available by executing the following command line in a terminal:

    ../build/bin/MoRIS_run -h

#### Parameters:
- <code>-h</code>, <code>--help</code>: Print this help, then exit,
- <code>-v</code>, <code>--version</code>: Print the current version, then exit,
- <code>-seed</code>, <code>--seed</code>: Specify the PRNG seed,
- <code>-typeofdata</code>, <code>--type-of-data</code>: Specify the type of experimental data (<code>PRESENCE_ONLY</code> or <code>PRESENCE_ABSENCE</code>),
- <code>-network</code>, <code>--network</code>: Specify the network file (default: <code>network.txt</code>),
- <code>-map</code>, <code>--map</code>: Specify the map file (default: <code>map.txt</code>),
- <code>-sample</code>, <code>--sample</code>: Specify the sample file (default: <code>sample.txt</code>),
- <code>-reps</code>, <code>--reps</code>: Specify the number of repetitions by simulation,
- <code>-iters</code>, <code>--iters</code>: Specify the number of iterations by simulation (usually one iteration is one year),
- <code>-law</code>, <code>--law</code>: Specify the jump distribution law (<code>DIRAC</code>, <code>NORMAL</code>, <code>LOG_NORMAL</code>, <code>CAUCHY</code>),
- <code>-optimfunc</code>, <code>--optimfunc</code>: Specify the optimization function (<code>LSS</code>, <code>LOG_LIKELIHOOD</code>, <code>LIKELIHOOD_LSS</code>). Use preferably the option <code>LOG_LIKELIHOOD</code>,
- <code>-humanactivity</code>, <code>--humanactivity</code>: Specify if the human activity index should be used to weight the number of jump events (<code>NO</code>, <code>YES</code>),
- <code>-xintro</code> <code>--xintro</code>: Specify the _x_ coordinate of the introduction cell,
- <code>-yintro</code>, <code>--yintro</code>: Specify the _y_ coordinate of the introduction cell,
- <code>-pintro</code>, <code>--pintro</code>: Specify the prevalence of the introduction (usually 1),
- <code>-lambda</code>, <code>--lambda</code>: Specify the mean number of jumps per cell per year,
- <code>-mu</code>, <code>--mu</code>: Specify the mean of the jump distribution (only works with <code>DIRAC</code>, <code>NORMAL</code>, <code>LOG_NORMAL</code>),
- <code>-sigma</code>, <code>--sigma</code>: Specify the variance of the jump distribution (only works with <code>NORMAL</code>, <code>LOG_NORMAL</code>),
- <code>-gamma</code>, <code>--gamma</code>: Specify the gamma parameter of the jump distribution (only works with <code>CAUCHY</code>),
- <code>-w1</code>, <code>--w1</code>: Specify the weight of category I roads,
- <code>-w2</code>, <code>--w2</code>: Specify the weight of category II roads,
- <code>-w3</code>, <code>--w3</code>: Specify the weight of category III roads,
- <code>-w4</code>, <code>--w4</code>: Specify the weight of category IV roads,
- <code>-w5</code>, <code>--w5</code>: Specify the weight of category V roads,
- <code>-w6</code>, <code>--w6</code>: Specify the weight of category VI roads,
- <code>-wmin</code>, <code>--wmin</code>: Specify the minimal weight between cells,
- <code>-save-outputs</code>, <code>--save-outputs</code>: Save simulation outputs (final state, lineage tree, ...),
- <code>-save-all-states</code>, <code>--save--all-states</code>: Save simulation state at any time.

### Couple MoRIS to the optimization algorithm <a name="optimize"></a>
To determine the HMD parameters explaining at best a given experimental dataset, MoRIS simulations are coupled to an optimization algorithm. MoRIS software provides a tool to do this: <code>MoRIS_optimize.py</code>. This script depends on a parameters file named <code>parameters.txt</code> that has a specific structure and parameters (see the example file <code>./examples/parameters.txt</code>). This parameters file allows the user to define the number of HMD parameters to optimize, their boundaries, and the type of optimization function desired. The parameters file adds a layer on top of simulation parameters (see above):

- <code>EXEC_PATH</code>: Specify the path of MoRIS_run executable,
- <code>NETWORK_FILE</code>: Specify the path of the road network file (usually <code>network.txt</code>),
- <code>MAP_FILE</code>: Specify the path of the map file (usually <code>map.txt</code>),
- <code>SAMPLE_FILE</code>: Specify the path of the sample file (usually <code>sample.txt</code>),
- <code>SEED</code>: Specify the PRNG seed,
- <code>TYPE_OF_DATA</code>: Specify the type of experimental data provided in <code>map.txt</code> (PRESENCE_ONLY, PRESENCE_ABSENCE),
- <code>REPETITIONS_BY_SIMULATION</code>: Specify the number of repetitions by simulation,
- <code>NUMBER_OF_ITERATIONS</code>: Specify the number of iterations by simulation (usually, one iteration is one year),
- <code>JUMP_LAW</code>: Specify the the jump distribution law (DIRAC, NORMAL, LOG_NORMAL, CAUCHY),
- <code>OPTIMIZATION_FUNCTION</code>: Specify the optimization function (LSS, LOG_LIKELIHOOD, LIKELIHOOD_LSS),
- <code>HUMAN_ACTIVITY_INDEX</code>: Specify if the human activity index should be used to weight the number of jump events (NO, YES),
- <code>WMIN</code>: Specify the minimal weight between cells,
- <code>XINTRO_DEFAULT</code>: Specify the default _x_ coordinate of the introduction cell (if not optimized),
- <code>YINTRO_DEFAULT</code>: Specify the default _y_ coordinate of the introduction cell (if not optimized),
- <code>PINTRO_DEFAULT</code>: Specify the default prevalence of introduction (if not optimized),
- <code>LAMBDA_DEFAULT</code>: Specify the default mean number of jumps per cell per year (if not optimized),
- <code>MU_DEFAULT</code>: Specify the default mean of the jump distribution (only works with DIRAC, NORMAL, LOG_NORMAL) (if not optimized),
- <code>SIGMA_DEFAULT</code>: Specify the default variance of the jump distribution (with NORMAL, LOG_NORMAL) (if not optimized),
- <code>GAMMA_DEFAULT</code>: Specify the default gamma parameter of the jump distribution (with CAUCHY) (if not optimized),
- <code>W1_DEFAULT</code>: Specify the default weight of category I roads (if not optimized),
- <code>W2_DEFAULT</code>: Specify the default weight of category II roads (if not optimized),
- <code>W3_DEFAULT</code>: Specify the default weight of category III roads (if not optimized),
- <code>W4_DEFAULT</code>: Specify the default weight of category IV roads (if not optimized),
- <code>W5_DEFAULT</code>: Specify the default weight of category V roads (if not optimized),
- <code>W6_DEFAULT</code>: Specify the default weight of category VI roads (if not optimized),
- <code>OPTIMIZE_XINTRO</code>: Specify if the _x_ coordinate of the introduction cell must be optimized (NO, YES),
- <code>OPTIMIZE_YINTRO</code>: Specify if the _y_ coordinate of the introduction cell must be optimized (NO, YES),
- <code>OPTIMIZE_PINTRO</code>: Specify if the prevalence of introduction must be optimized (NO, YES),
- <code>OPTIMIZE_LAMBDA</code>: Specify if the mean number of jumps per cell per year must be optimized (NO, YES),
- <code>OPTIMIZE_MU</code>: Specify if the mean of the jump distribution must be optimized (NO, YES),
- <code>OPTIMIZE_SIGMA</code>: Specify if the variance of the jump distribution must be optimized (NO, YES),
- <code>OPTIMIZE_GAMMA</code>: Specify if the gamma parameter of the jump distribution must be optimized (NO, YES),
- <code>OPTIMIZE_W1</code>: Specify if the weight of category I roads must be optimized (NO, YES),
- <code>OPTIMIZE_W2</code>: Specify if the weight of category II roads must be optimized (NO, YES),
- <code>OPTIMIZE_W3</code>: Specify if the weight of category III roads must be optimized (NO, YES),
- <code>OPTIMIZE_W4</code>: Specify if the weight of category IV roads must be optimized (NO, YES),
- <code>OPTIMIZE_W5</code>: Specify if the weight of category V roads must be optimized (NO, YES),
- <code>OPTIMIZE_W6</code>: Specify if the weight of category VI roads must be optimized (NO, YES),
- <code>XINTRO_MIN</code> and <code>XINTRO_MAX</code>: Specify the boundaries of _x_ (0 &leq; _x_ &leq; 1),
- <code>YINTRO_MIN</code> and <code>YINTRO_MAX</code>: Specify the boundaries of _y_ (0 &leq; _y_ &leq; 1),
- <code>PINTRO_MIN</code> and <code>PINTRO_MAX</code>: Specify the boundaries of _p_<sub>intro</sub> (0 &leq; _p_<sub>intro</sub> &leq; 1),
- <code>LAMBDA_MIN</code> and <code>LAMBDA_MAX</code>: Specify the boundaries of &lambda; (0 &leq; &lambda;),
- <code>MU_MIN</code> and <code>MU_MAX</code>: Specify the boundaries of &mu; (0 &leq; &mu;),
- <code>SIGMA_MIN</code> and <code>SIGMA_MAX</code>: Specify the boundaries of &sigma; (0.1 &leq; &sigma;),
- <code>GAMMA_MIN</code> and <code>GAMMA_MAX</code>: Specify the boundaries of &gamma; (0 &leq; &gamma;),
- <code>W1_MIN</code> and <code>W1_MAX</code>: Specify the boundaries of _w_<sub>1</sub> (0 &leq; _w_<sub>1</sub> &leq; 1),
- <code>W2_MIN</code> and <code>W2_MAX</code>: Specify the boundaries of _w_<sub>2</sub> (0 &leq; _w_<sub>2</sub> &leq; 1),
- <code>W3_MIN</code> and <code>W3_MAX</code>: Specify the boundaries of _w_<sub>3</sub> (0 &leq; _w_<sub>3</sub> &leq; 1),
- <code>W4_MIN</code> and <code>W4_MAX</code>: Specify the boundaries of _w_<sub>4</sub> (0 &leq; _w_<sub>4</sub> &leq; 1),
- <code>W5_MIN</code> and <code>W5_MAX</code>: Specify the boundaries of _w_<sub>5</sub> (0 &leq; _w_<sub>5</sub> &leq; 1),
- <code>W6_MIN</code> and <code>W6_MAX</code>: Specify the boundaries of _w_<sub>6</sub> (0 &leq; _w_<sub>6</sub> &leq; 1).

## Input files tutorial <a name="tutorial"></a>
A tutorial to build MoRIS input files is <a href="https://github.com/charlesrocabert/MoRIS/blob/master/INPUT_FILES_TUTORIAL.md">available here</a>.

## Copyright <a name="copyright"></a>
Copyright © 2014-2024 Charles Rocabert, Jérôme M.W Gippet, Serge Fenet. All rights reserved.

## License <a name="license"></a>
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.

<p align="center">
<br/><br/>
<img src="docs/pics/travelling_ant.png" width=200>
<br/>
</p>

