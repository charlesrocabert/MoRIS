/**
 * \file      Graph.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Graph class declaration
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

#ifndef __MoRIS__Graph__
#define __MoRIS__Graph__

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "Enums.h"
#include "Prng.h"
#include "Node.h"


class Graph
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Graph( void ) = delete;
  Graph( const Graph& graph ) = delete;
  Graph( Prng* prng, type_of_data data, std::string network_filename, std::string map_filename, std::string sample_filename, int nb_repetitions, std::vector<double>* road_linear_combination, double minimal_connectivity );
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Graph( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*------------------------------------------------------------------ Graph structure */
  
  inline int   get_number_of_nodes( void );
  inline Node* get_node( int identifier );
  inline Node* get_first( void );
  inline Node* get_next( void );
  
  /*------------------------------------------------------------------ Graph data */
  
  inline double get_maximum_weights_sum( void ) const;
  inline double get_minimization_score( void ) const;
  inline double get_minimum_x_coordinate( void ) const;
  inline double get_minimum_y_coordinate( void ) const;
  inline double get_maximum_x_coordinate( void ) const;
  inline double get_maximum_y_coordinate( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Graph& operator=(const Graph&) = delete;
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void update( bool last_iteration );
  void write_state( int iteration, double xintro, double yintro );
  
  inline void compute_jump_probabilities( void );
  inline void reset_states( void );
  inline void untag( void );
  
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
  
  /*------------------------------------------------------------------ Main parameters */
  
  Prng*        _prng;           /*!< Pseudorandom numbers generator */
  int          _nb_repetitions; /*!< Number of repetitions          */
  type_of_data _data;           /*!< Type of experimental data      */
  
  /*------------------------------------------------------------------ Graph structure */
  
  std::unordered_map<int, Node*>           _map; /*!< Nodes map          */
  std::unordered_map<int, Node*>::iterator _it;  /*!< Nodes map iterator */
  
  /*------------------------------------------------------------------ Graph data */
  
  double _maximum_weights_sum; /*!< Maximum weights sum in nodes */
  double _minimization_score;  /*!< Minimization score           */
  double _min_x_coord;         /*!< Minimum x coordinate         */
  double _min_y_coord;         /*!< Minimum y coordinate         */
  double _max_x_coord;         /*!< Maximum x coordinate         */
  double _max_y_coord;         /*!< Maximum y coordinate         */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Graph structure */

/**
 * \brief    Get total number of nodes
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Graph::get_number_of_nodes( void )
{
  return (int)_map.size();
}

/**
 * \brief    Get node
 * \details  --
 * \param    int identifier
 * \return   \e Node*
 */
inline Node* Graph::get_node( int identifier )
{
  if (_map.find(identifier) != _map.end())
  {
    return _map[identifier];
  }
  return NULL;
}

/**
 * \brief    Get first node
 * \details  --
 * \param    void
 * \return   \e Node*
 */
inline Node* Graph::get_first( void )
{
  _it = _map.begin();
  return _it->second;
}

/**
 * \brief    Get next node
 * \details  --
 * \param    void
 * \return   \e Node*
 */
inline Node* Graph::get_next( void )
{
  ++_it;
  if (_it == _map.end())
  {
    return NULL;
  }
  return _it->second;
}

/*------------------------------------------------------------------ Graph data */

/**
 * \brief    Get the maximum weights sum in the graph
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_maximum_weights_sum( void ) const
{
  return _maximum_weights_sum;
}

/**
 * \brief    Get the minimization score
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_minimization_score( void ) const
{
  return _minimization_score;
}

/**
 * \brief    Get the minimum x coordinate
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_minimum_x_coordinate( void ) const
{
  return _min_x_coord;
}

/**
 * \brief    Get the minimum y coordinate
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_minimum_y_coordinate( void ) const
{
  return _min_y_coord;
}

/**
 * \brief    Get the maximum x coordinate
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_maximum_x_coordinate( void ) const
{
  return _max_x_coord;
}

/**
 * \brief    Get the maximum y coordinate
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_maximum_y_coordinate( void ) const
{
  return _max_y_coord;
}

/*----------------------------
 * SETTERS
 *----------------------------*/

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Compute jump probabilities depending on the maximum weight of the map
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Graph::compute_jump_probabilities( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->set_jump_probability(it->second->get_weights_sum() / _maximum_weights_sum);
  }
}

/**
 * \brief    Reset all node states
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Graph::reset_states( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->reset();
  }
  _minimization_score = 0.0;
  _it                 = _map.begin();
}

/**
 * \brief    Untag all the nodes
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Graph::untag( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->untag();
  }
}


#endif /* defined(__MoRIS__Graph__) */
