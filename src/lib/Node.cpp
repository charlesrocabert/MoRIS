/**
 * \file      Node.cpp
 * \author    Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright © 2014-2024 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Node class definition
 */

/****************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright © 2014-2024 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
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

#include "Node.h"


/*----------------------------
 * CONSTRUCTORS
 *----------------------------*/

/**
 * \brief    Constructor
 * \details  --
 * \param    Parameters* parameters
 * \param    int identifier
 * \return   \e void
 */
Node::Node( Parameters* parameters, int identifier )
{
  assert(parameters != NULL);
  
  /*--------------------------------------- MAIN PARAMETERS */
  
  _parameters = parameters;
  _prng       = _parameters->get_prng();
  _identifier = identifier;
  
  /*--------------------------------------- MAP */
  
  _x                  = 0;
  _y                  = 0;
  _node_area          = 0.0;
  _suitable_area      = 0.0;
  _population         = 0.0;
  _population_density = 0.0;
  _road_density       = 0.0;
  
  /*--------------------------------------- NETWORK */
  
  _neighbors.clear();
  _weights.clear();
  _weights_sum          = 0.0;
  _human_activity_index = 0.0;
  
  /*--------------------------------------- SAMPLE */
  
  _y_obs = 0.0;
  _n_obs = 0.0;
  _p_obs = 0.0;
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  _tagged                 = false;
  _current_state          = new int[_parameters->get_repetitions()];
  _next_state             = new int[_parameters->get_repetitions()];
  _nb_introductions       = new double[_parameters->get_repetitions()];
  _total_nb_introductions = 0.0;
  _mean_nb_introductions  = 0.0;
  _var_nb_introductions   = 0.0;
  for (int i = 0; i < _parameters->get_repetitions(); i++)
  {
    _current_state[i]    = 0;
    _next_state[i]       = 0;
    _nb_introductions[i] = 0.0;
  }
  _n_sim              = (double)_parameters->get_repetitions();
  _y_sim              = 0.0;
  _p_sim              = 0.0;
  _first_invasion_age = new double[_parameters->get_repetitions()];
  _last_invasion_age  = new double[_parameters->get_repetitions()];
  for (int i = 0; i < _parameters->get_repetitions(); i++)
  {
    _first_invasion_age[i] = -1.0;
    _last_invasion_age[i]  = -1.0;
  }
  _mean_first_invasion_age = 0.0;
  _mean_last_invasion_age  = 0.0;
  _var_first_invasion_age  = 0.0;
  _var_last_invasion_age   = 0.0;
  
  /*--------------------------------------- SCORES */
  
  _likelihood             = 0.0;
  _empty_likelihood       = 0.0;
  _maximum_likelihood     = 0.0;
  _log_likelihood         = 0.0;
  _log_empty_likelihood   = 0.0;
  _log_maximum_likelihood = 0.0;
  _empty_score            = 0.0;
  _score                  = 0.0;
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
  /*--------------------------------------- NETWORK */
  
  _neighbors.clear();
  _weights.clear();
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  delete[] _current_state;
  _current_state = NULL;
  delete[] _next_state;
  _next_state = NULL;
  delete[] _nb_introductions;
  _nb_introductions = NULL;
  delete[] _first_invasion_age;
  _first_invasion_age = NULL;
  delete[] _last_invasion_age;
  _last_invasion_age = NULL;
}

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Jump to the next node
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
  printf("Error during jump (Node::jump() method)...\n");
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
  _mean_nb_introductions      = 0.0;
  _var_nb_introductions       = 0.0;
  _y_sim                      = 0.0;
  _p_sim                      = 0.0;
  _mean_first_invasion_age    = 0.0;
  _mean_last_invasion_age     = 0.0;
  _var_first_invasion_age     = 0.0;
  _var_last_invasion_age      = 0.0;
  double first_invasion_count = 0.0;
  double last_invasion_count  = 0.0;
  for (int rep = 0; rep < _parameters->get_repetitions(); rep++)
  {
    _current_state[rep]     = _next_state[rep];
    _mean_nb_introductions += _nb_introductions[rep];
    _var_nb_introductions  += _nb_introductions[rep]*_nb_introductions[rep];
    _y_sim                 += (double)_current_state[rep];
    _p_sim                 += (double)_current_state[rep];
    if (_first_invasion_age[rep] != -1.0)
    {
      _mean_first_invasion_age += _first_invasion_age[rep];
      _var_first_invasion_age  += _first_invasion_age[rep]*_first_invasion_age[rep];
      first_invasion_count     += 1.0;
    }
    if (_last_invasion_age[rep] != -1.0)
    {
      _mean_last_invasion_age += _last_invasion_age[rep];
      _var_last_invasion_age  += _last_invasion_age[rep]*_last_invasion_age[rep];
      last_invasion_count     += 1.0;
    }
  }
  _mean_nb_introductions /= _n_sim;
  _var_nb_introductions  /= _n_sim;
  _var_nb_introductions  -= _mean_nb_introductions*_mean_nb_introductions;
  _p_sim                 /= _n_sim;
  if (first_invasion_count > 0.0)
  {
    _mean_first_invasion_age /= first_invasion_count;
    _var_first_invasion_age  /= first_invasion_count;
    _var_first_invasion_age  -= _mean_first_invasion_age*_mean_first_invasion_age;
  }
  if (last_invasion_count > 0.0)
  {
    _mean_last_invasion_age /= last_invasion_count;
    _var_last_invasion_age  /= last_invasion_count;
    _var_last_invasion_age  -= _mean_last_invasion_age*_mean_last_invasion_age;
  }
}

/**
 * \brief    Reset node state
 * \details  --
 * \param    void
 * \return   \e void
 */
void Node::reset_state( void )
{
  for (int rep = 0; rep < _parameters->get_repetitions(); rep++)
  {
    _current_state[rep]    = 0;
    _next_state[rep]       = 0;
    _nb_introductions[rep] = 0.0;
  }
  _total_nb_introductions = 0.0;
  _mean_nb_introductions  = 0.0;
  _var_nb_introductions   = 0.0;
  _y_sim                  = 0.0;
  _p_sim                  = 0.0;
  for (int rep = 0; rep < _parameters->get_repetitions(); rep++)
  {
    _first_invasion_age[rep] = -1.0;
    _last_invasion_age[rep]  = -1.0;
  }
  _mean_first_invasion_age = 0.0;
  _mean_last_invasion_age  = 0.0;
  _var_first_invasion_age  = 0.0;
  _var_last_invasion_age   = 0.0;
}

/**
 * \brief    Compute likelihoods and score
 * \details  --
 * \param    void
 * \return   \e void
 */
void Node::compute_score( void )
{
  unsigned int a          = (unsigned int)(_y_sim);
  unsigned int b          = (unsigned int)(_y_obs);
  unsigned int c          = (unsigned int)(_n_sim-_y_sim);
  unsigned int d          = (unsigned int)(_n_obs-_y_obs);
  double current_MFS      = gsl_ran_hypergeometric_pdf(b, b+b, d+d, b+d);
  double current_FS       = gsl_ran_hypergeometric_pdf(a, a+b, c+d, a+c);
  _likelihood             = current_FS;
  _maximum_likelihood     = current_MFS;
  _log_likelihood         = -log(current_FS);
  _log_maximum_likelihood = -log(current_MFS);
  _score                  = 0.0;
  if (_parameters->get_optimization_function() == LSS)
  {
    _score = (_p_sim-_p_obs)*(_p_sim-_p_obs);
  }
  else if (_parameters->get_optimization_function() == LOG_LIKELIHOOD)
  {
    _score = _log_likelihood;
  }
  else if (_parameters->get_optimization_function() == LIKELIHOOD_LSS)
  {
    _score = (1.0-_likelihood/_maximum_likelihood)*(1.0-_likelihood/_maximum_likelihood);
  }
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

