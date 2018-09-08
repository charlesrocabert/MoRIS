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
  
  _network_filename = "";
  _map_filename     = "";
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
  
  _save_simulation_state = false;
  _save_lineage_tree     = false;
  
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
  
  _network_filename = parameters._network_filename;
  _map_filename     = parameters._map_filename;
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
  
  _save_simulation_state = parameters._save_simulation_state;
  _save_lineage_tree     = parameters._save_lineage_tree;
  
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

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

