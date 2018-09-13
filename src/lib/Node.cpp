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
 * \param    double introduction_probability
 * \return   \e void
 */
Node::Node( Prng* prng, int identifier, int nb_repetitions, double introduction_probability )
{
  /*--------------------------------------- SIMULATION VARIABLES */
  
  _prng                     = prng;
  _identifier               = identifier;
  _nb_repetitions           = nb_repetitions;
  _introduction_probability = introduction_probability;
  _tagged                   = false;
  _current_state            = new int[_nb_repetitions];
  _next_state               = new int[_nb_repetitions];
  _n_sim                    = (double)_nb_repetitions/_introduction_probability;
  _y_sim                    = 0.0;
  _f_sim                    = 0.0;
  _nb_introductions         = new double[_nb_repetitions];
  _total_nb_introductions   = 0.0;
  _mean_nb_introductions    = 0.0;
  _var_nb_introductions     = 0.0;
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  _weights.clear();
  _neighbors.clear();
  _weights_sum      = 0.0;
  _jump_probability = 0.0;
  
  /*--------------------------------------- MAP DATA */
  
  _x_coord       = 0;
  _y_coord       = 0;
  _node_area     = 0.0;
  _suitable_area = 0.0;
  
  /*--------------------------------------- SAMPLING DATA */
  
  _y_obs = 0.0;
  _n_obs = 0.0;
  _f_obs = 0.0;
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
  /*--------------------------------------- SIMULATION VARIABLES */
  
  delete[] _current_state;
  _current_state = NULL;
  delete[] _next_state;
  _next_state = NULL;
  delete[] _nb_introductions;
  _nb_introductions = NULL;
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
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
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Compute the weight sum (for a self-avoiding random walk) */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  double weight_sum = 0.0;
  for (size_t i = 0; i < _neighbors.size(); i++)
  {
    if (!(_neighbors[i] != NULL && _neighbors[i]->isTagged()))
    {
      weight_sum += _weights[i];
    }
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) If there is no way to escape the node, break             */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  if (weight_sum == 0.0)
  {
    return this;
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Or draw the next node with roulette wheel                */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  double draw = _prng->uniform()*weight_sum;
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

/**
 * \brief    Update node state
 * \details  This method also computes the next simulated probability of presence
 * \param    void
 * \return   \e void
 */
void Node::update_state( void )
{
  /* memcpy not used because _probability_of_presence
     must be computed */
  _y_sim = 0.0;
  _f_sim = 0.0;
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _current_state[rep]  = _next_state[rep];
    _y_sim              += (double)_next_state[rep];
    _f_sim              += (double)_next_state[rep];
  }
  _f_sim /= _n_sim;
}

/**
 * \brief    Reset node state
 * \details  --
 * \param    void
 * \return   \e void
 */
void Node::reset_state( void )
{
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _current_state[rep]    = 0;
    _next_state[rep]       = 0;
    _nb_introductions[rep] = 0.0;
  }
  _y_sim                   = 0.0;
  _f_sim                   = 0.0;
  _total_nb_introductions  = 0.0;
  _mean_nb_introductions   = 0.0;
  _var_nb_introductions    = 0.0;
}

/**
 * \brief    Compute the mean number of introductions
 * \details  --
 * \param    void
 * \return   \e void
 */
void Node::compute_mean_var_nb_introductions( void )
{
  _mean_nb_introductions = 0.0;
  _var_nb_introductions  = 0.0;
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _mean_nb_introductions += _nb_introductions[rep];
    _var_nb_introductions  += _nb_introductions[rep]*_nb_introductions[rep];
  }
  _mean_nb_introductions /= (double)_nb_repetitions;
  _var_nb_introductions  /= (double)_nb_repetitions;
  _var_nb_introductions  -= _mean_nb_introductions*_mean_nb_introductions;
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

