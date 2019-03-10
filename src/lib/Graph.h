/**
 * \file      Graph.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2019 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Graph class declaration
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
#include "Parameters.h"
#include "Node.h"


class Graph
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Graph( void ) = delete;
  Graph( Parameters* parameters );
  Graph( const Graph& graph ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Graph( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  inline int   get_number_of_nodes( void );
  inline Node* get_node( int identifier );
  inline Node* get_first( void );
  inline Node* get_next( void );
  
  /*--------------------------------------- MINIMIZATION SCORES */
  
  inline double get_total_log_likelihood( void ) const;
  inline double get_total_log_empty_likelihood( void ) const;
  inline double get_total_log_maximum_likelihood( void ) const;
  inline double get_empty_score( void ) const;
  inline double get_score( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Graph& operator=(const Graph&) = delete;
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void untag( void );
  void update_state( void );
  void compute_score( bool empty );
  void write_state( std::string filename );
  void write_invasion_euclidean_distributions( std::string observed_filename, std::string simulated_filename );
  
  /*----------------------------
   * PUBLIC ATTRIBUTES
   *----------------------------*/
  
protected:
  
  /*----------------------------
   * PROTECTED METHODS
   *----------------------------*/
  int    get_introduction_node_from_coordinates( void );
  void   load_map( void );
  void   load_network( void );
  void   load_sample( void );
  void   compute_statistics( void );
  void   compute_jump_probability( void );
  void   reset_states( void );
  void   set_introduction_node( void );
  double compute_euclidean_distance( Node* node1, Node* node2 );
  
  /*----------------------------
   * PROTECTED ATTRIBUTES
   *----------------------------*/
  
  /*--------------------------------------- MAIN PARAMETERS */
  
  Parameters* _parameters; /*!< Main parameters */
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  std::unordered_map<int, Node*>           _map; /*!< Nodes map          */
  std::unordered_map<int, Node*>::iterator _it;  /*!< Nodes map iterator */
  
  /*--------------------------------------- GRAPH STATISTICS */
  
  int    _introduction_node;      /*!< Introduction node            */
  double _min_x_coord;            /*!< Minimum x coordinate         */
  double _max_x_coord;            /*!< Maximum x coordinate         */
  double _min_y_coord;            /*!< Minimum y coordinate         */
  double _max_y_coord;            /*!< Maximum y coordinate         */
  double _min_weights_sum;        /*!< Minimum weights sum in nodes */
  double _max_weights_sum;        /*!< Maximum weights sum in nodes */
  double _min_population;         /*!< Minimum population size      */
  double _max_population;         /*!< Maximum population size      */
  double _min_population_density; /*!< Minimum population density   */
  double _max_population_density; /*!< Maximum population density   */
  double _min_road_density;       /*!< Minimum population size      */
  double _max_road_density;       /*!< Maximum population size      */
  
  /*--------------------------------------- MINIMIZATION SCORE */
  
  double _total_log_likelihood;         /*!< Total log hypergeometric likelihood         */
  double _total_log_empty_likelihood;   /*!< Total log empty hypergeometric likelihood   */
  double _total_log_maximum_likelihood; /*!< Total log maximum hypergeometric likelihood */
  double _empty_score;                  /*!< Optimization score with empty map           */
  double _score;                        /*!< Optimization score                          */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*--------------------------------------- GRAPH STRUCTURE */

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

/*--------------------------------------- MINIMIZATION SCORES */

/**
 * \brief    Get the total log likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_total_log_likelihood( void ) const
{
  return _total_log_likelihood;
}

/**
 * \brief    Get the total log empty likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_total_log_empty_likelihood( void ) const
{
  return _total_log_empty_likelihood;
}

/**
 * \brief    Get the total log maximum likelihood
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_total_log_maximum_likelihood( void ) const
{
  return _total_log_maximum_likelihood;
}

/**
 * \brief    Get the optimization score with empty map
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_empty_score( void ) const
{
  return _empty_score;
}

/**
 * \brief    Get the optimization score
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Graph::get_score( void ) const
{
  return _score;
}

/*----------------------------
 * SETTERS
 *----------------------------*/


#endif /* defined(__MoRIS__Graph__) */
