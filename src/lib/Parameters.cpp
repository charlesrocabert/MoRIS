/**
 * \file      Parameters.cpp
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      15-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Parameters class definition
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

#include "Parameters.h"


/*----------------------------
 * CONSTRUCTORS
 *----------------------------*/

/**
 * \brief    Default constructor
 * \details  --
 * \param    void
 * \return   \e void
 */
Parameters::Parameters( void )
{
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  _prng = new Prng();
  _seed = 0;
  
  /*------------------------------------------------------------------ Type of experimental data */
  
  _data = PRESENCE_ONLY;
  
  /*------------------------------------------------------------------ Input data filenames */
  
  _map_filename     = "";
  _network_filename = "";
  _sample_filename  = "";
  
  /*------------------------------------------------------------------ Number of repetitions by simulation */
  
  _repetitions_by_simulation = 0;
  
  /*------------------------------------------------------------------ Number of iterations by simulation */
  
  _number_of_iterations = 0;
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  _introduction_coordinates = std::pair<double, double>(0.0, 0.0);
  _lambda                   = 0.0;
  _mu                       = 0.0;
  _sigma                    = 0.0;
  _jump_law                 = DIRAC;
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  _road_linear_combination.clear();
  _road_linear_combination.assign(6, 0.0);
  
  /*------------------------------------------------------------------ Minimal connectivity of the connectivity graph */
  
  _minimal_connectivity = 0.0;
  
  /*------------------------------------------------------------------ Extra statistics */
  
  _save_outputs = false;
  
}

/**
 * \brief    Copy constructor
 * \details  --
 * \param    const Parameters& parameters
 * \return   \e void
 */
Parameters::Parameters( const Parameters& parameters )
{
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  _prng = new Prng(*parameters._prng);
  _seed = parameters._seed;
  
  /*------------------------------------------------------------------ Type of experimental data */
  
  _data = parameters._data;
  
  /*------------------------------------------------------------------ Input data filenames */
  
  _map_filename     = parameters._map_filename;
  _network_filename = parameters._network_filename;
  _sample_filename  = parameters._sample_filename;
  
  /*------------------------------------------------------------------ Number of repetitions by simulation */
  
  _repetitions_by_simulation = parameters._repetitions_by_simulation;
  
  /*------------------------------------------------------------------ Number of iterations by simulation */
  
  _number_of_iterations = parameters._number_of_iterations;
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  _introduction_coordinates = std::pair<double, double>(parameters._introduction_coordinates.first, parameters._introduction_coordinates.second);
  _lambda                   = parameters._lambda;
  _mu                       = parameters._mu;
  _sigma                    = parameters._sigma;
  _jump_law                 = parameters._jump_law;
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  _road_linear_combination = std::vector<double>(parameters._road_linear_combination);
  
  /*------------------------------------------------------------------ Minimal connectivity between two nodes */
  
  _minimal_connectivity = parameters._minimal_connectivity;
  
  /*------------------------------------------------------------------ Extra statistics */
  
  _save_outputs = parameters._save_outputs;
  
}

/*----------------------------
 * DESTRUCTORS
 *----------------------------*/

/**
 * \brief    Destructor
 * \details  --
 * \param    void
 * \return   \e void
 */
Parameters::~Parameters( void )
{
  delete _prng;
  _prng = NULL;
}


/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Write parameters in a file
 * \details  --
 * \param    void
 * \return   \e void
 */
void Parameters::write_parameters( std::string filename )
{
  /*--------------*/
  /* 1) Open file */
  /*--------------*/
  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  
  /*-----------------*/
  /* 2) Write header */
  /*-----------------*/
  file << "seed" << " ";
  file << "type_of_data" << " ";
  file << "map" << " ";
  file << "network" << " ";
  file << "sample" << " ";
  file << "repetitions" << " ";
  file << "iterations" << " ";
  file << "x_introduction" << " ";
  file << "y_introduction" << " ";
  file << "lambda" << " ";
  file << "mu" << " ";
  file << "sigma" << " ";
  file << "jump_law" << " ";
  file << "road1_weight" << " ";
  file << "road2_weight" << " ";
  file << "road3_weight" << " ";
  file << "road4_weight" << " ";
  file << "road5_weight" << " ";
  file << "road6_weight" << " ";
  file << "minimal_connectivity" << " ";
  file << "save_outputs" << "\n";
  
  /*-----------------*/
  /* 3) Write data   */
  /*-----------------*/
  file << _seed << " ";
  if (_data == PRESENCE_ONLY)
  {
    file << "PRESENCE_ONLY" << " ";
  }
  else if (_data == PRESENCE_ABSENCE)
  {
    file << "PRESENCE_ABSENCE" << " ";
  }
  file << _map_filename << " ";
  file << _network_filename << " ";
  file << _sample_filename << " ";
  file << _repetitions_by_simulation << " ";
  file << _number_of_iterations << " ";
  file << _introduction_coordinates.first << " ";
  file << _introduction_coordinates.second << " ";
  file << _lambda << " ";
  file << _mu << " ";
  file << _sigma << " ";
  file << _jump_law << " ";
  file << _road_linear_combination[0] << " ";
  file << _road_linear_combination[1] << " ";
  file << _road_linear_combination[2] << " ";
  file << _road_linear_combination[3] << " ";
  file << _road_linear_combination[4] << " ";
  file << _road_linear_combination[5] << " ";
  file << _minimal_connectivity << " ";
  file << _save_outputs << "\n";
  
  /*---------------*/
  /* 4) Close file */
  /*---------------*/
  file.close();
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

