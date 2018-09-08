/**
 * \file      Node.cpp
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Node class definition
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

#include "Node.h"


/*----------------------------
 * CONSTRUCTORS
 *----------------------------*/

/**
 * \brief    Constructor
 * \details  --
 * \param    Prng* prng
 * \param    int identifier
 * \param    int nb_repetitions
 * \return   \e void
 */
Node::Node( Prng* prng, int identifier, int nb_repetitions )
{
  /*------------------------------------------------------------------ Simulation variables */
  
  _prng                    = prng;
  _identifier              = identifier;
  _nb_repetitions          = nb_repetitions;
  _tagged                  = false;
  _current_state           = new int[_nb_repetitions];
  _next_state              = new int[_nb_repetitions];
  _probability_of_presence = 0.0;
  _nb_introductions        = new double[_nb_repetitions];
  _mean_nb_introductions   = 0.0;
  
  /*------------------------------------------------------------------ Graph structure */
  
  _weights.clear();
  _neighbors.clear();
  _weights_sum      = 0.0;
  _jump_probability = 0.0;
  
  /*------------------------------------------------------------------ Map data */
  
  _x_coord   = 0;
  _y_coord   = 0;
  _node_area = 0.0;
  
  /*------------------------------------------------------------------ Sample data */
  
  _y = 0.0;
  _n = 0.0;
  _f = 0.0;
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
Node::~Node( void )
{
  /*------------------------------------------------------------------ Simulation variables */
  
  delete[] _current_state;
  _current_state = NULL;
  delete[] _next_state;
  _next_state = NULL;
  delete[] _nb_introductions;
  _nb_introductions = NULL;
  
  /*------------------------------------------------------------------ Graph structure */
  
  _weights.clear();
  _neighbors.clear();
}

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Jump to the next cell
 * \details  --
 * \param    void
 * \return   \e Node*
 */
Node* Node::jump( void )
{
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Compute the weight sum                       */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  double total = 0.0;
  for (size_t i = 0; i < _neighbors.size(); i++)
  {
    if (!(_neighbors[i] != NULL && _neighbors[i]->isTagged()))
    {
      total += _weights[i];
    }
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) If there is no way to escape the node, break */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  if (total == 0.0)
  {
    return this;
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Or draw the next node                        */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  double draw = _prng->uniform()*total;
  double sum  = 0.0;
  for (size_t i = 0; i < _neighbors.size(); i++)
  {
    if (!(_neighbors[i] != NULL && _neighbors[i]->isTagged()))
    {
      sum += _weights[i];
      if (draw < sum)
      {
        return _neighbors[i];
      }
    }
  }
  printf("Error during jump (Cell::jump() method)...\n");
  exit(EXIT_FAILURE);
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/
