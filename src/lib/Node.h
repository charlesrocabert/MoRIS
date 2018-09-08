/**
 * \file      Node.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Node class declaration
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

#ifndef __MoRIS__Node__
#define __MoRIS__Node__

#include <iostream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "Prng.h"


class Node
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Node( void ) = delete;
  Node( Prng* prng, int identifier, int nb_repetitions );
  Node( const Node& node ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Node( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*------------------------------------------------------------------ Simulation variables */
  
  inline int    get_identifier( void ) const;
  inline bool   isTagged( void ) const;
  inline int    get_current_state( int rep );
  inline int    get_next_state( int rep );
  inline double get_probability_of_presence( void ) const;
  
  /*------------------------------------------------------------------ Graph structure */
  
  inline std::vector<double>* get_weights( void );
  inline std::vector<Node*>*  get_neighbors( void );
  inline double               get_weights_sum( void ) const;
  inline double               get_jump_probability( void ) const;
  
  /*------------------------------------------------------------------ Map data */
  
  inline double get_x_coord( void ) const;
  inline double get_y_coord( void ) const;
  inline double get_node_area( void ) const;
  
  /*------------------------------------------------------------------ Sample data */
  
  inline double get_y( void ) const;
  inline double get_n( void ) const;
  inline double get_f( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Node& operator=(const Node&) = delete;
  
  /*------------------------------------------------------------------ Simulation variables */
  
  inline void tag( void );
  inline void untag( void );
  inline void set_current_state( int rep, int state );
  inline void set_next_state( int rep, int state );
  inline void set_as_introduction_node( void );
  
  /*------------------------------------------------------------------ Graph structure */
  
  inline void add_neighbor( double weight, Node* node );
  inline void set_jump_probability( double jump_probability );
  
  /*------------------------------------------------------------------ Map data */
  
  inline void set_x_coord( double x_coord );
  inline void set_y_coord( double y_coord );
  inline void set_map_data( double x_coord, double y_coord, double node_area );
  
  /*------------------------------------------------------------------ Sample data */
  
  inline void set_sample_data( double y, double n );
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  Node* jump( void );
  
  inline bool   isAllowedToJump( int rep );
  inline void   update( void );
  inline void   reset( void );
  inline void   add_introduction( int rep );
  inline double compute_mean_nb_introductions( void );
  
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
  
  /*------------------------------------------------------------------ Simulation variables */
  
  Prng*   _prng;                    /*!< Pseudorandom numbers generator    */
  int     _identifier;              /*!< Node identifier                   */
  int     _nb_repetitions;          /*!< Number of repetitions             */
  bool    _tagged;                  /*!< Node tag state                    */
  int*    _current_state;           /*!< Hexagon current state             */
  int*    _next_state;              /*!< Hexagon next state                */
  double  _probability_of_presence; /*!< Simulated probability of presence */
  double* _nb_introductions;        /*!< Number of introductions           */
  double  _mean_nb_introductions;   /*!< Mean number of introductions      */
  
  /*------------------------------------------------------------------ Network data */
  
  
  std::vector<double> _weights;          /*!< Vector of weights   */
  std::vector<Node*>  _neighbors;        /*!< Vector of neighbors */
  double              _weights_sum;      /*!< Sum of weights      */
  double              _jump_probability; /*!< Jump probability    */
  
  /*------------------------------------------------------------------ Map data */
  
  double _x_coord;   /*!< Node x coordinate */
  double _y_coord;   /*!< Node y coordinate */
  double _node_area; /*!< Node area         */
  
  /*------------------------------------------------------------------ Sample data */
  
  double _y; /*!< Number of positive observations */
  double _n; /*!< Total number of observations    */
  double _f; /*!< Observed frequency              */
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Simulation variables */

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
inline int Node::get_current_state( int rep )
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  return _current_state[rep];
}

/**
 * \brief    Get next state for repetition rep
 * \details  --
 * \param    int rep
 * \return   \e int
 */
inline int Node::get_next_state( int rep )
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  return _next_state[rep];
}

/**
 * \brief    Get the simulated probability of presence
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_probability_of_presence( void ) const
{
  return _probability_of_presence;
}

/*------------------------------------------------------------------ Graph structure */

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

/*------------------------------------------------------------------ Map data */

/**
 * \brief    Get the x coordinate of the node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_x_coord( void ) const
{
  return _x_coord;
}

/**
 * \brief    Get the y coordinate of the node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_y_coord( void ) const
{
  return _y_coord;
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

/*------------------------------------------------------------------ Sample data */

/**
 * \brief    Get observed number of positive samples
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_y( void ) const
{
  return _y;
}

/**
 * \brief    Get observed number of samples
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_n( void ) const
{
  return _n;
}

/**
 * \brief    Get observed frequency
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_f( void ) const
{
  return _f;
}

/*----------------------------
 * SETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Simulation variables */

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
 * \brief    Set node current state (occupied or not) for repetition rep
 * \details  --
 * \param    int rep
 * \param    int state
 * \return   \e void
 */
inline void Node::set_current_state( int rep, int state )
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  assert(state == 0 || state == 1);
  _current_state[rep] = state;
}

/**
 * \brief    Set node next state (occupied or not) for repetition rep
 * \details  --
 * \param    int rep
 * \param    int state
 * \return   \e void
 */
inline void Node::set_next_state( int rep, int state )
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  assert(state == 0 || state == 1);
  _next_state[rep]        = state;
  _nb_introductions[rep] += 1.0;
}

/**
 * \brief    Set this node as an introduction node
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Node::set_as_introduction_node( void )
{
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _current_state[rep]       = 1;
    _next_state[rep]          = 1;
    _probability_of_presence  = 1.0;
    _nb_introductions[rep]   += 1.0;
  }
}

/*------------------------------------------------------------------ Graph structure */

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

/*------------------------------------------------------------------ Map data */

/**
 * \brief    Set x coordinate
 * \details  --
 * \param    double x_coord
 * \return   \e void
 */
inline void Node::set_x_coord( double x_coord )
{
  _x_coord = x_coord;
}

/**
 * \brief    Set y coordinate
 * \details  --
 * \param    double y_coord
 * \return   \e void
 */
inline void Node::set_y_coord( double y_coord )
{
  _y_coord = y_coord;
}

/**
 * \brief    Set the map data
 * \details  --
 * \param    double x_coord
 * \param    double y_coord
 * \param    double node_area
 * \return   \e void
 */
inline void Node::set_map_data( double x_coord, double y_coord, double node_area )
{
  assert(node_area > 0.0);
  _x_coord   = x_coord;
  _y_coord   = y_coord;
  _node_area = node_area;
}

/*------------------------------------------------------------------ Sample data */

/**
 * \brief    Set the sampling data
 * \details  --
 * \param    double y
 * \param    double n
 * \return   \e void
 */
inline void Node::set_sample_data( double y, double n )
{
  assert(y >= 0.0);
  assert(n >= 0.0);
  assert(y <= n);
  _y = y;
  _n = n;
  _f = 0.0;
  if (_n > 0.0)
  {
    _f = _y/_n;
  }
}

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Evaluate the jump capability
 * \details  If the node is occupied (state = 1), a new jump is allowed
 * \param    int rep
 * \return   \e bool
 */
inline bool Node::isAllowedToJump( int rep )
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  if (_current_state[rep] == 1)
  {
    _next_state[rep] = 1;
    return true;
  }
  return false;
}

/**
 * \brief    Update node state
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Node::update( void )
{
  _probability_of_presence = 0.0;
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _current_state[rep]       = _next_state[rep];
    _probability_of_presence += _current_state[rep];
    _next_state[rep]          = 0;
  }
  _probability_of_presence /= (double)_nb_repetitions;
}

/**
 * \brief    Reset node state
 * \details  --
 * \param    void
 * \return   \e void
 */
inline void Node::reset( void )
{
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _current_state[rep]    = 0;
    _next_state[rep]       = 0;
    _nb_introductions[rep] = 0.0;
  }
  _probability_of_presence = 0.0;
  _mean_nb_introductions   = 0.0;
}

/**
 * \brief    Add an introduction at position 'rep'
 * \details  --
 * \param    int rep
 * \return   \e void
 */
inline void Node::add_introduction( int rep )
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  _nb_introductions[rep]++;
}

/**
 * \brief    Compute the mean number of introductions
 * \details  --
 * \param    void
 * \return   \e void
 */
inline double Node::compute_mean_nb_introductions( void )
{
  _mean_nb_introductions = 0.0;
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _mean_nb_introductions += _nb_introductions[rep];
  }
  return _mean_nb_introductions/(double)_nb_repetitions;
}


#endif /* defined(__MoRIS__Node__) */
