/**
 * \file      Node.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Node class declaration
 */

/************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright (c) 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet
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
 ************************************************************************/

#ifndef __MoRIS__Node__
#define __MoRIS__Node__

#include <iostream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "Enums.h"
#include "Prng.h"
#include "Parameters.h"


class Node
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Node( void ) = delete;
  Node( Parameters* parameters, int identifier );
  Node( const Node& node ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Node( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*--------------------------------------- MAIN PARAMETERS */
  
  inline int get_identifier( void ) const;
  
  /*--------------------------------------- MAP */
  
  inline double get_x( void ) const;
  inline double get_y( void ) const;
  inline double get_node_area( void ) const;
  inline double get_suitable_area( void ) const;
  inline double get_population( void ) const;
  inline double get_population_density( void ) const;
  inline double get_road_density( void ) const;
  
  /*--------------------------------------- NETWORK */
  
  inline std::vector<Node*>*  get_neighbors( void );
  inline std::vector<double>* get_weights( void );
  inline double               get_weights_sum( void ) const;
  inline double               get_jump_probability( void ) const;
  
  /*--------------------------------------- SAMPLE */
  
  inline double get_y_obs( void ) const;
  inline double get_n_obs( void ) const;
  inline double get_p_obs( void ) const;
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  inline bool   isTagged( void ) const;
  inline bool   isOccupied( int rep ) const;
  inline double get_total_nb_introductions( void ) const;
  inline double get_mean_nb_introductions( void ) const;
  inline double get_var_nb_introductions( void ) const;
  inline double get_n_sim( void ) const;
  inline double get_y_sim( void ) const;
  inline double get_p_sim( void ) const;
  
  /*--------------------------------------- SCORES */
  
  inline double get_likelihood( void ) const;
  inline double get_empty_likelihood( void ) const;
  inline double get_maximum_likelihood( void ) const;
  inline double get_log_likelihood( void ) const;
  inline double get_log_empty_likelihood( void ) const;
  inline double get_log_maximum_likelihood( void ) const;
  inline double get_empty_score( void ) const;
  inline double get_score( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Node& operator=(const Node&) = delete;
  
  /*--------------------------------------- MAP */
  
  inline void set_map_data( double x, double y, double node_area, double suitable_area, double population, double population_density, double road_density );
  
  /*--------------------------------------- NETWORK */
  
  inline void add_neighbor( double weight, Node* node );
  inline void set_jump_probability( double jump_probability );
  
  /*--------------------------------------- SAMPLE */
  
  inline void set_sample_data( double y_obs, double n_obs );
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  inline void tag( void );
  inline void untag( void );
  inline void add_introduction( int rep );
  inline void set_as_introduction_node( void );
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  Node* jump( void );
  void  update_state( void );
  void  reset_state( void );
  void  compute_score( void );
  
  /*----------------------------
   * PUBLIC ATTRIBUTES
   *----------------------------*/
  
protected:
  
  /*----------------------------
   * PROTECTED METHODS
   *----------------------------*/
  
  /*----------------------------
   * PROTECTED ATTRIBUTES
   *----------------------------*/
  
  /*--------------------------------------- MAIN PARAMETERS */
  
  Parameters* _parameters; /*!< Simulation parameters          */
  Prng*       _prng;       /*!< Pseudorandom numbers generator */
  int         _identifier; /*!< Node identifier                */
  
  /*--------------------------------------- NETWORK */
  
  std::vector<Node*>  _neighbors;        /*!< Vector of neighbors */
  std::vector<double> _weights;          /*!< Vector of weights   */
  double              _weights_sum;      /*!< Sum of weights      */
  double              _jump_probability; /*!< Jump probability    */
  
  /*--------------------------------------- MAP */
  
  double _x;                  /*!< Node x coordinate   */
  double _y;                  /*!< Node y coordinate   */
  double _node_area;          /*!< Node area           */
  double _suitable_area;      /*!< Suitable area       */
  double _population;         /*!< Communal population */
  double _population_density; /*!< Population density  */
  double _road_density;       /*!< Road density        */
  
  /*--------------------------------------- SAMPLE */
  
  double _y_obs; /*!< Number of positive observations */
  double _n_obs; /*!< Total number of observations    */
  double _p_obs; /*!< Observed prevalence             */
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  bool    _tagged;                 /*!< Node tag state                          */
  int*    _current_state;          /*!< Hexagon current state                   */
  int*    _next_state;             /*!< Hexagon next state                      */
  double* _nb_introductions;       /*!< Number of introductions                 */
  double  _total_nb_introductions; /*!< Total number of introductions           */
  double  _mean_nb_introductions;  /*!< Mean number of introductions            */
  double  _var_nb_introductions;   /*!< Variance of the number of introductions */
  double  _n_sim;                  /*!< Number of virtual sampled cells         */
  double  _y_sim;                  /*!< Number of occupied cells                */
  double  _p_sim;                  /*!< Simulated prevalence                    */
  
  /*--------------------------------------- SCORES */
  
  double _likelihood;             /*!< Fisher's likelihood         */
  double _empty_likelihood;       /*!< Fisher's empty likelihood   */
  double _maximum_likelihood;     /*!< Fisher's maximum likelihood */
  double _log_likelihood;         /*!< Log likelihood              */
  double _log_empty_likelihood;   /*!< Log empty likelihood        */
  double _log_maximum_likelihood; /*!< Log maximum likelihood      */
  double _empty_score;            /*!< Empty score                 */
  double _score;                  /*!< Score                       */
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*--------------------------------------- MAIN PARAMETERS */

/**
 * \brief    Get the node identifier
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Node::get_identifier( void ) const
{
  return _identifier;
}

/*--------------------------------------- MAP */

/**
 * \brief    Get the x coordinate of the node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_x( void ) const
{
  return _x;
}

/**
 * \brief    Get the y coordinate of the node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_y( void ) const
{
  return _y;
}

/**
 * \brief    Get the area of the node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_node_area( void ) const
{
  return _node_area;
}

/**
 * \brief    Get the suitable area of the node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_suitable_area( void ) const
{
  return _suitable_area;
}

/**
 * \brief    Get the population
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_population( void ) const
{
  return _population;
}

/**
 * \brief    Get the population density
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_population_density( void ) const
{
  return _population_density;
}

/**
 * \brief    Get the road density
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_road_density( void ) const
{
  return _road_density;
}

/*--------------------------------------- NETWORK */

/**
 * \brief    Get the list of neighbors
 * \details  --
 * \param    void
 * \return   \e std::vector<Node*>*
 */
inline std::vector<Node*>* Node::get_neighbors( void )
{
  return &_neighbors;
}

/**
 * \brief    Get the list of weights
 * \details  --
 * \param    void
 * \return   \e std::vector<double>*
 */
inline std::vector<double>* Node::get_weights( void )
{
  return &_weights;
}

/**
 * \brief    Get the sum of edge weights
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_weights_sum( void ) const
{
  return _weights_sum;
}

/**
 * \brief    Get jump probability
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_jump_probability( void ) const
{
  return _jump_probability;
}

/*--------------------------------------- SAMPLE */

/**
 * \brief    Get observed number of positive samples
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_y_obs( void ) const
{
  return _y_obs;
}

/**
 * \brief    Get observed number of samples
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_n_obs( void ) const
{
  return _n_obs;
}

/**
 * \brief    Get observed prevalence
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_p_obs( void ) const
{
  return _p_obs;
}

/*--------------------------------------- SIMULATION VARIABLES */

/**
 * \brief    Check if the node is tagged
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Node::isTagged( void ) const
{
  return _tagged;
}

/**
 * \brief    Get current state for repetition rep
 * \details  --
 * \param    int rep
 * \return   \e int
 */
inline bool Node::isOccupied( int rep ) const
{
  assert(rep >= 0);
  assert(rep < _parameters->get_repetitions());
  return (bool)_current_state[rep];
}

/**
 * \brief    Get the total number of introductions through all the repetitions
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_total_nb_introductions( void ) const
{
  return _total_nb_introductions;
}

/**
 * \brief    Get the mean number of introductions
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_mean_nb_introductions( void ) const
{
  return _mean_nb_introductions;
}

/**
 * \brief    Get the variance of the number of introductions
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_var_nb_introductions( void ) const
{
  return _var_nb_introductions;
}

/**
 * \brief    Get the number of virtual sampled cells
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_n_sim( void ) const
{
  return _n_sim;
}

/**
 * \brief    Get the number of occupied cells
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_y_sim( void ) const
{
  return _y_sim;
}

/**
 * \brief    Get the simulated prevalence
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_p_sim( void ) const
{
  return _p_sim;
}

/*--------------------------------------- SCORES */

/**
 * \brief    Get the likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_likelihood( void ) const
{
  return _likelihood;
}

/**
 * \brief    Get the empty likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_empty_likelihood( void ) const
{
  return _empty_likelihood;
}

/**
 * \brief    Get the maximum likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_maximum_likelihood( void ) const
{
  return _maximum_likelihood;
}

/**
 * \brief    Get the log likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_log_likelihood( void ) const
{
  return _log_likelihood;
}

/**
 * \brief    Get the log empty likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_log_empty_likelihood( void ) const
{
  return _log_empty_likelihood;
}

/**
 * \brief    Get the log maximum likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_log_maximum_likelihood( void ) const
{
  return _log_maximum_likelihood;
}

/**
 * \brief    Get the empty score
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_empty_score( void ) const
{
  return _empty_score;
}

/**
 * \brief    Get the score
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_score( void ) const
{
  return _score;
}


/*----------------------------
 * SETTERS
 *----------------------------*/

/*--------------------------------------- MAP */

/**
 * \brief    Set the map data
 * \details  --
 * \param    double x_coord
 * \param    double y_coord
 * \param    double node_area
 * \param    double suitable_area
 * \param    double population
 * \param    double population_density
 * \param    double road_density
 * \return   \e void
 */
inline void Node::set_map_data( double x, double y, double node_area, double suitable_area, double population, double population_density, double road_density )
{
  assert(node_area > 0.0);
  assert(suitable_area > 0.0);
  assert(suitable_area <= node_area);
  assert(population >= 0.0);
  assert(population_density >= 0.0);
  assert(road_density >= 0.0);
  _x                  = x;
  _y                  = y;
  _node_area          = node_area;
  _suitable_area      = suitable_area;
  _population         = population;
  _population_density = population_density;
  _road_density       = road_density;
}

/*--------------------------------------- NETWORK */

/**
 * \brief    Add a neighbor to the neighbors list
 * \details  --
 * \param    double weight
 * \param    Node* node
 * \return   \e void
 */
inline void Node::add_neighbor( double weight, Node* node )
{
  assert(weight >= 0.0);
  _weights.push_back(weight);
  _neighbors.push_back(node);
  _weights_sum += weight;
}

/**
 * \brief    Set jump probability
 * \details  --
 * \param    double jump_probability
 * \return   \e void
 */
inline void Node::set_jump_probability( double jump_probability )
{
  assert(jump_probability >= 0.0);
  assert(jump_probability <= 1.0);
  _jump_probability = jump_probability;
}

/*--------------------------------------- SAMPLE */

/**
 * \brief    Set the sampling data
 * \details  --
 * \param    double y_obs
 * \param    double n_obs
 * \return   \e void
 */
inline void Node::set_sample_data( double y_obs, double n_obs )
{
  assert(y_obs >= 0.0);
  assert(n_obs >= 0.0);
  assert(y_obs <= n_obs);
  _y_obs = y_obs;
  _n_obs = n_obs;
  _p_obs = 0.0;
  if (_n_obs > 0.0)
  {
    _p_obs = _y_obs/_n_obs;
  }
}

/*--------------------------------------- SIMULATION VARIABLES */

/**
 * \brief    Tag the node
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Node::tag( void )
{
  _tagged = true;
}

/**
 * \brief    Untag the node
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Node::untag( void )
{
  _tagged = false;
}

/**
 * \brief    Add an introduction at repetition rep
 * \details  --
 * \param    int rep
 * \return   \e void
 */
inline void Node::add_introduction( int rep )
{
  assert(rep >= 0);
  assert(rep < _parameters->get_repetitions());
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Set state at time t+1                 */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  _next_state[rep] = 1;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Increment the number of introductions */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  _nb_introductions[rep]  += 1.0;
  _total_nb_introductions += 1.0;
}

/**
 * \brief    Set this node as an introduction node
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Node::set_as_introduction_node( void )
{
  _y_sim = 0.0;
  for (int rep = 0; rep < _parameters->get_repetitions(); rep++)
  {
    if (_prng->uniform() < _parameters->get_p_introduction())
    {
      _current_state[rep]     = 1;
      _next_state[rep]        = 1;
      _nb_introductions[rep] += 1.0;
      _y_sim                 += 1.0;
    }
    else
    {
      _current_state[rep] = 0;
      _next_state[rep]    = 0;
    }
  }
  _p_sim = _y_sim/_n_sim;
  /*
  for (int rep = 0; rep < _parameters->get_repetitions(); rep++)
  {
    _current_state[rep]     = 1;
    _next_state[rep]        = 1;
    _nb_introductions[rep] += 1.0;
  }
  _y_sim = (double)_parameters->get_repetitions();
  _p_sim = _parameters->get_p_introduction();
   */
}


#endif /* defined(__MoRIS__Node__) */
