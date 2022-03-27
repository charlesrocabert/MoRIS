/**
 * \file      Parameters.cpp
 * \author    Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * \date      15-12-2014
 * \copyright MoRIS. Copyright © 2014-2022 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Parameters class definition
 */

/****************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright © 2014-2022 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * Web: https://github.com/charlesrocabert/MoRIS
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
 ****************************************************************************/

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
  
  /*------------------------------------------------------------------ Input data filenames */
  
  _map_filename     = "";
  _network_filename = "";
  _sample_filename  = "";
  
  /*------------------------------------------------------------------ Main parameters */
  
  _data                 = PRESENCE_ABSENCE;
  _repetitions          = 0;
  _iterations           = 0;
  _jump_law             = DIRAC;
  _optim_function       = LOG_LIKELIHOOD;
  _human_activity_index = false;
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  _x_introduction = 0.0;
  _y_introduction = 0.0;
  _p_introduction = 0.0;
  _lambda         = 0.0;
  _mu             = 0.0;
  _sigma          = 0.0;
  _gamma          = 0.0;
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  _w1   = 0.0;
  _w2   = 0.0;
  _w3   = 0.0;
  _w4   = 0.0;
  _w5   = 0.0;
  _w6   = 0.0;
  _wmin = 0.0;
  
  /*------------------------------------------------------------------ Extra statistics */
  
  _save_outputs    = false;
  _save_all_states = false;
  
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
  /*~~~~~~~~~~~~~~~~~*/
  /* 1) Open file    */
  /*~~~~~~~~~~~~~~~~~*/
  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  
  /*~~~~~~~~~~~~~~~~~*/
  /* 2) Write header */
  /*~~~~~~~~~~~~~~~~~*/
  file << "seed" << " ";
  file << "map" << " ";
  file << "network" << " ";
  file << "sample" << " ";
  file << "typeofdata" << " ";
  file << "reps" << " ";
  file << "iters" << " ";
  file << "law" << " ";
  file << "optimfunc" << " ";
  file << "humanactivity" << " ";
  file << "xintro" << " ";
  file << "yintro" << " ";
  file << "pintro" << " ";
  file << "lambda" << " ";
  file << "mu" << " ";
  file << "sigma" << " ";
  file << "gamma" << " ";
  file << "w1" << " ";
  file << "w2" << " ";
  file << "w3" << " ";
  file << "w4" << " ";
  file << "w5" << " ";
  file << "w6" << " ";
  file << "wmin" << " ";
  file << "save-outputs" << " ";
  file << "save-all-states" << "\n";
  
  /*~~~~~~~~~~~~~~~~~*/
  /* 3) Write data   */
  /*~~~~~~~~~~~~~~~~~*/
  file << _seed << " ";
  file << _map_filename << " ";
  file << _network_filename << " ";
  file << _sample_filename << " ";
  if (_data == PRESENCE_ONLY)
  {
    file << "PRESENCE_ONLY" << " ";
  }
  else if (_data == PRESENCE_ABSENCE)
  {
    file << "PRESENCE_ABSENCE" << " ";
  }
  file << _repetitions << " ";
  file << _iterations << " ";
  if (_jump_law == DIRAC)
  {
    file << "DIRAC" << " ";
  }
  else if (_jump_law == NORMAL)
  {
    file << "NORMAL" << " ";
  }
  else if (_jump_law == LOG_NORMAL)
  {
    file << "LOG_NORMAL" << " ";
  }
  else if (_jump_law == CAUCHY)
  {
    file << "CAUCHY" << " ";
  }
  if (_optim_function == LSS)
  {
    file << "LSS" << " ";
  }
  else if (_optim_function == LOG_LIKELIHOOD)
  {
    file << "LOG_LIKELIHOOD" << " ";
  }
  else if (_optim_function == LIKELIHOOD_LSS)
  {
    file << "LIKELIHOOD_LSS" << " ";
  }
  if (_human_activity_index)
  {
    file << "YES" << " ";
  }
  else
  {
    file << "NO" << " ";
  }
  file << _x_introduction << " ";
  file << _y_introduction << " ";
  file << _p_introduction << " ";
  file << _lambda << " ";
  file << _mu << " ";
  file << _sigma << " ";
  file << _gamma << " ";
  file << _w1 << " ";
  file << _w2 << " ";
  file << _w3 << " ";
  file << _w4 << " ";
  file << _w5 << " ";
  file << _w6 << " ";
  file << _wmin << " ";
  file << _save_outputs << " ";
  file << _save_all_states << "\n";
  
  /*---------------*/
  /* 4) Close file */
  /*---------------*/
  file.close();
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

