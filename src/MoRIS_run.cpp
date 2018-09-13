/**
 * \file      MoRIS_run.cpp
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      15-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     MoRIS_run executable
 */

/************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include "../cmake/Config.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <sys/stat.h>
#include <gsl/gsl_multimin.h>
#include <assert.h>

#include "./lib/Parameters.h"
#include "./lib/Simulation.h"

void printUsage( void );
void readArgs( int argc, char const** argv, Parameters* parameters );
void create_output_folder( void );


/**
 * \brief    main function
 * \details  --
 * \param    int argc
 * \param    char const** argv
 * \return   \e int
 */
int main(int argc, char const** argv)
{
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Read command line arguments and load parameters */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  Parameters* parameters = new Parameters();
  readArgs(argc, argv, parameters);
  if (parameters->saveOutputs())
  {
    create_output_folder();
    parameters->write_parameters("output/parameters.txt");
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Create the simulation                           */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  Simulation* simulation = new Simulation(parameters);
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Run the simulation                              */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  while (simulation->get_iteration() < parameters->get_number_of_iterations())
  {
    simulation->compute_next_iteration();
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 4) Compute the score and save the final state      */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  simulation->compute_score();
  if (parameters->saveOutputs())
  {
    simulation->write_state("output/final_state.txt");
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 5) Return the score, free the memory and exit      */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  double empty_score = simulation->get_empty_score();
  double score       = simulation->get_score();
  delete simulation;
  simulation = NULL;
  delete parameters;
  parameters = NULL;
  std::cout << empty_score << " " << score << "\n";
  return EXIT_SUCCESS;
}

/**
 * \brief    Read arguments
 * \details  --
 * \param    int argc
 * \param    char const** argv
 * \param    Parameters* parameters
 * \return   \e void
 */
void readArgs( int argc, char const** argv, Parameters* parameters )
{
  double xcoord = 0.0;
  double ycoord = 0.0;
  std::vector<double> rweights;
  rweights.assign(6, 0.0);
  
  std::unordered_map<std::string, bool> options;
  options["seed"]             = false;
  options["typeofdata"]       = false;
  options["network"]          = false;
  options["map"]              = false;
  options["sample"]           = false;
  options["rep"]              = false;
  options["iterations"]       = false;
  options["xcoord"]           = false;
  options["ycoord"]           = false;
  options["p-intro"]          = false;
  options["lambda"]           = false;
  options["mu"]               = false;
  options["sigma"]            = false;
  options["law"]              = false;
  options["wroad1"]           = false;
  options["wroad2"]           = false;
  options["wroad3"]           = false;
  options["wroad4"]           = false;
  options["wroad5"]           = false;
  options["wroad6"]           = false;
  options["min-connectivity"] = false;
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
    {
      printUsage();
      exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
    {
      std::cout << PACKAGE << " (" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << ")\n";
      exit(EXIT_SUCCESS);
    }
    if (strcmp(argv[i], "-seed") == 0 || strcmp(argv[i], "--seed") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: command seed value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_prng_seed((unsigned)atol(argv[i+1]));
        options["seed"] = true;
      }
    }
    if (strcmp(argv[i], "-typeofdata") == 0 || strcmp(argv[i], "--type-of-data") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: typeofdata parameter is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        options["typeofdata"] = true;
        if (strcmp(argv[i+1], "PRESENCE_ONLY") == 0)
        {
          parameters->set_type_of_data(PRESENCE_ONLY);
        }
        else if (strcmp(argv[i+1], "PRESENCE_ABSENCE") == 0)
        {
          parameters->set_type_of_data(PRESENCE_ABSENCE);
        }
        else
        {
          std::cout << "Error: wrong typeofdata value.\n";
          exit(EXIT_FAILURE);
        }
      }
    }
    if (strcmp(argv[i], "-network") == 0 || strcmp(argv[i], "--network") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: network filename is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_network_filename(argv[i+1]);
        options["network"] = true;
      }
    }
    if (strcmp(argv[i], "-map") == 0 || strcmp(argv[i], "--map") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: map filename is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_map_filename(argv[i+1]);
        options["map"] = true;
      }
    }
    if (strcmp(argv[i], "-sample") == 0 || strcmp(argv[i], "--sample") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: sample filename is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_sample_filename(argv[i+1]);
        options["sample"] = true;
      }
    }
    if (strcmp(argv[i], "-rep") == 0 || strcmp(argv[i], "--rep") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: rep value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_repetitions_by_simulation(atoi(argv[i+1]));
        options["rep"] = true;
      }
    }
    if (strcmp(argv[i], "-iterations") == 0 || strcmp(argv[i], "--iterations") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: iterations value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_number_of_iterations(atoi(argv[i+1]));
        options["iterations"] = true;
      }
    }
    if (strcmp(argv[i], "-xcoord") == 0 || strcmp(argv[i], "--xcoord") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: xcoord value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        xcoord = atof(argv[i+1]);
        options["xcoord"] = true;
      }
    }
    if (strcmp(argv[i], "-ycoord") == 0 || strcmp(argv[i], "--ycoord") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: ycoord value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        ycoord = atof(argv[i+1]);
        options["ycoord"] = true;
      }
    }
    if (strcmp(argv[i], "-p-intro") == 0 || strcmp(argv[i], "--p-intro") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: p-intro value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_introduction_probability(atof(argv[i+1]));
        options["p-intro"] = true;
      }
    }
    if (strcmp(argv[i], "-lambda") == 0 || strcmp(argv[i], "--lambda") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: lambda value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_lambda(atof(argv[i+1]));
        options["lambda"] = true;
      }
    }
    if (strcmp(argv[i], "-mu") == 0 || strcmp(argv[i], "--mu") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: mu value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_mu(atof(argv[i+1]));
        options["mu"] = true;
      }
    }
    if (strcmp(argv[i], "-sigma") == 0 || strcmp(argv[i], "--sigma") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: sigma value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_sigma(atof(argv[i+1]));
        options["sigma"] = true;
      }
    }
    if (strcmp(argv[i], "-law") == 0 || strcmp(argv[i], "--law") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: law name is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        options["law"] = true;
        if (strcmp(argv[i+1], "DIRAC") == 0)
        {
          parameters->set_jump_law(DIRAC);
        }
        else if (strcmp(argv[i+1], "GAUSSIAN") == 0)
        {
          parameters->set_jump_law(GAUSSIAN);
        }
        else if (strcmp(argv[i+1], "LOGNORMAL") == 0)
        {
          parameters->set_jump_law(LOGNORMAL);
        }
        else
        {
          std::cout << "Error: wrong law value.\n";
          exit(EXIT_FAILURE);
        }
      }
    }
    if (strcmp(argv[i], "-wroad1") == 0 || strcmp(argv[i], "--wroad1") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: wroad1 value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        rweights[0] = atof(argv[i+1]);
        options["wroad1"] = true;
      }
    }
    if (strcmp(argv[i], "-wroad2") == 0 || strcmp(argv[i], "--wroad2") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: wroad2 value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        rweights[1] = atof(argv[i+1]);
        options["wroad2"] = true;
      }
    }
    if (strcmp(argv[i], "-wroad3") == 0 || strcmp(argv[i], "--wroad3") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: wroad3 value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        rweights[2] = atof(argv[i+1]);
        options["wroad3"] = true;
      }
    }
    if (strcmp(argv[i], "-wroad4") == 0 || strcmp(argv[i], "--wroad4") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: wroad4 value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        rweights[3] = atof(argv[i+1]);
        options["wroad4"] = true;
      }
    }
    if (strcmp(argv[i], "-wroad5") == 0 || strcmp(argv[i], "--wroad5") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: wroad5 value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        rweights[4] = atof(argv[i+1]);
        options["wroad5"] = true;
      }
    }
    if (strcmp(argv[i], "-wroad6") == 0 || strcmp(argv[i], "--wroad6") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: wroad6 value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        rweights[5] = atof(argv[i+1]);
        options["wroad6"] = true;
      }
    }
    if (strcmp(argv[i], "-min-connectivity") == 0 || strcmp(argv[i], "--min-connectivity") == 0)
    {
      if (i+1 == argc)
      {
        std::cout << "Error: min-connectivity value is missing.\n";
        exit(EXIT_FAILURE);
      }
      else
      {
        parameters->set_minimal_connectivity(atof(argv[i+1]));
        options["min-connectivity"] = true;
      }
    }
    if (strcmp(argv[i], "-save-outputs") == 0 || strcmp(argv[i], "--save-outputs") == 0)
    {
      parameters->set_save_outputs(true);
    }
  }
  bool parameter_lacking = false;
  for (auto it = options.begin(); it != options.end(); ++it)
  {
    if (!it->second)
    {
      std::cout << "--" << it->first << " option is mandatory.\n";
      parameter_lacking = true;
    }
  }
  if (parameter_lacking)
  {
    exit(EXIT_FAILURE);
  }
  options.clear();
  std::pair<double, double> coordinates(xcoord, ycoord);
  parameters->set_introduction_coordinates(&coordinates);
  parameters->set_road_linear_combination(&rweights);
  rweights.clear();
}

/**
 * \brief    Print usage
 * \details  --
 * \param    void
 * \return   \e void
 */
void printUsage( void )
{
  std::cout << "\n";
  std::cout << "**********************************************************************\n";
#ifdef DEBUG
  std::cout << " " << PACKAGE << " " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << " ( debug )\n";
#endif
#ifdef NDEBUG
  std::cout << " " << PACKAGE << " " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << " ( release )\n";
#endif
  std::cout << " MoRIS (Model of Routes of Invasive Spread)                           \n";
  std::cout << "                                                                      \n";
  std::cout << " Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet \n";
  std::cout << "                                                                      \n";
  std::cout << " This program comes with ABSOLUTELY NO WARRANTY.                      \n";
  std::cout << " This is free software, and you are welcome to redistribute it under  \n";
  std::cout << " certain conditions; See the GNU General Public License for details   \n";
  std::cout << "**********************************************************************\n";
  std::cout << "Usage: MoRIS_run -h or --help\n";
  std::cout << "   or: MoRIS_run [list of mandatory parameters]\n";
  std::cout << "Options are:\n";
  std::cout << "  -h, --help\n";
  std::cout << "        print this help, then exit\n";
  std::cout << "  -v, --version\n";
  std::cout << "        print the current version, then exit\n";
  std::cout << "  -seed, --seed <seed>\n";
  std::cout << "        Specify the prng seed\n";
  std::cout << "  -typeofdata, --type-of-data <typeofdata>\n";
  std::cout << "        Specify the type of experimental data (PRESENCE_ONLY or PRESENCE_ABSENCE)\n";
  std::cout << "  -network, --network <filename>\n";
  std::cout << "        Specify the network file (default: network.txt)\n";
  std::cout << "  -map, --map <filename>\n";
  std::cout << "        Specify the map file (default: map.txt)\n";
  std::cout << "  -sample, --sample <filename>\n";
  std::cout << "        Specify the sample file (default: sample.txt)\n";
  std::cout << "  -rep, --rep <repetitions>\n";
  std::cout << "        Specify the number of repetitions by simulation\n";
  std::cout << "  -iterations, --iterations <iterations>\n";
  std::cout << "        Specify the number of iterations by simulation\n";
  std::cout << "  -xcoord, --xcoord <coordinate>\n";
  std::cout << "        Specify the x coordinate of the introduction cell\n";
  std::cout << "  -ycoord, --ycoord <coordinate>\n";
  std::cout << "        Specify the y coordinate of the introduction cell\n";
  std::cout << "  -p-intro, --p-intro <p-intro>\n";
  std::cout << "        Specify the probability of introduction\n";
  std::cout << "  -lambda, --lambda <lambda>\n";
  std::cout << "        Specify the mean number of jumps/cell/year\n";
  std::cout << "  -mu, --mu <mu>\n";
  std::cout << "        Specify the mean of the jump distribution\n";
  std::cout << "  -sigma, --sigma <sigma>\n";
  std::cout << "        Specify the variance of the jump distribution\n";
  std::cout << "  -law, --law <law>\n";
  std::cout << "        Specify the jump distribution law\n";
  std::cout << "  -wroad1, --wroad1 <weight>\n";
  std::cout << "        Specify the weight of type I roads\n";
  std::cout << "  -wroad2, --wroad2 <weight>\n";
  std::cout << "        Specify the weight of type II roads\n";
  std::cout << "  -wroad3, --wroad3 <weight>\n";
  std::cout << "        Specify the weight of type III roads\n";
  std::cout << "  -wroad4, --wroad4 <weight>\n";
  std::cout << "        Specify the weight of type IV roads\n";
  std::cout << "  -wroad5, --wroad5 <weight>\n";
  std::cout << "        Specify the weight of type V roads\n";
  std::cout << "  -wroad6, --wroad6 <weight>\n";
  std::cout << "        Specify the weight of type VI roads\n";
  std::cout << "  -min-connectivity, --min-connectivity <connectivity>\n";
  std::cout << "        Specify the minimal connectivity between cells\n";
  std::cout << "  -save-outputs, --save-outputs\n";
  std::cout << "        Save various simulation outputs (final state, lineage tree, ...)\n";
  std::cout << "\n";
}

/**
 * \brief    Create outputs folder
 * \details  --
 * \param    void
 * \return   \e void
 */
void create_output_folder( void )
{
  system("rm -rf output");
  system("mkdir output");
}

