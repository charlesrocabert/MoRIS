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
  Node( Prng* prng, int identifier, int nb_repetitions, double introduction_probability );
  Node( const Node& node ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Node( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  inline int    get_identifier( void ) const;
  inline bool   isTagged( void ) const;
  inline bool   isOccupied( int rep ) const;
  inline double get_n_sim( void ) const;
  inline double get_y_sim( void ) const;
  inline double get_f_sim( void ) const;
  inline double get_total_nb_introductions( void ) const;
  inline double get_mean_nb_introductions( void ) const;
  inline double get_var_nb_introductions( void ) const;
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  inline std::vector<double>* get_weights( void );
  inline std::vector<Node*>*  get_neighbors( void );
  inline double               get_weights_sum( void ) const;
  inline double               get_jump_probability( void ) const;
  
  /*--------------------------------------- MAP DATA */
  
  inline double get_x_coord( void ) const;
  inline double get_y_coord( void ) const;
  inline double get_node_area( void ) const;
  inline double get_suitable_area( void ) const;
  
  /*--------------------------------------- SAMPLING DATA */
  
  inline double get_y_obs( void ) const;
  inline double get_n_obs( void ) const;
  inline double get_f_obs( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Node& operator=(const Node&) = delete;
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  inline void tag( void );
  inline void untag( void );
  inline void add_introduction( int rep );
  inline void set_as_introduction_node( void );
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  inline void add_neighbor( double weight, Node* node );
  inline void set_jump_probability( double jump_probability );
  
  /*--------------------------------------- MAP DATA */
  
  inline void set_map_data( double x_coord, double y_coord, double node_area, double suitable_area );
  
  /*--------------------------------------- SAMPLING DATA */
  
  inline void set_sample_data( double y_obs, double n_obs );
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  Node* jump( void );
  void  update_state( void );
  void  reset_state( void );
  void  compute_mean_var_nb_introductions( void );
  
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
  
  /*--------------------------------------- SIMULATION VARIABLES */
  
  Prng*   _prng;                     /*!< Pseudorandom numbers generator          */
  int     _identifier;               /*!< Node identifier                         */
  int     _nb_repetitions;           /*!< Number of repetitions                   */
  double  _introduction_probability; /*!< Introduction probability                */
  bool    _tagged;                   /*!< Node tag state                          */
  int*    _current_state;            /*!< Hexagon current state                   */
  int*    _next_state;               /*!< Hexagon next state                      */
  double  _n_sim;                    /*!< Number of virtual sampled cells         */
  double  _y_sim;                    /*!< Number of occupied cells                */
  double  _f_sim;                    /*!< Simulated probability of presence       */
  double* _nb_introductions;         /*!< Number of introductions                 */
  double  _total_nb_introductions;   /*!< Total number of introductions           */
  double  _mean_nb_introductions;    /*!< Mean number of introductions            */
  double  _var_nb_introductions;     /*!< Variance of the number of introductions */
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  std::vector<double> _weights;          /*!< Vector of weights   */
  std::vector<Node*>  _neighbors;        /*!< Vector of neighbors */
  double              _weights_sum;      /*!< Sum of weights      */
  double              _jump_probability; /*!< Jump probability    */
  
  /*--------------------------------------- MAP DATA */
  
  double _x_coord;       /*!< Node x coordinate */
  double _y_coord;       /*!< Node y coordinate */
  double _node_area;     /*!< Node area         */
  double _suitable_area; /*!< Suitable area     */
  
  /*--------------------------------------- SAMPLING DATA */
  
  double _y_obs; /*!< Number of positive observations */
  double _n_obs; /*!< Total number of observations    */
  double _f_obs; /*!< Observed frequency              */
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*--------------------------------------- SIMULATION VARIABLES */

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
inline bool Node::isOccupied( int rep ) const
{
  assert(rep >= 0);
  assert(rep < _nb_repetitions);
  return (bool)_current_state[rep];
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
 * \brief    Get the simulated probability of presence
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_f_sim( void ) const
{
  return _f_sim;
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

/*--------------------------------------- GRAPH STRUCTURE */

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

/*--------------------------------------- MAP DATA */

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

/*--------------------------------------- SAMPLING DATA */

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
 * \brief    Get observed frequency
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Node::get_f_obs( void ) const
{
  return _f_obs;
}

/*----------------------------
 * SETTERS
 *----------------------------*/

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
  assert(rep < _nb_repetitions);
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
  for (int rep = 0; rep < _nb_repetitions; rep++)
  {
    _current_state[rep]     = 1;
    _next_state[rep]        = 1;
    _nb_introductions[rep] += 1.0;
  }
  _y_sim = (double)_nb_repetitions;
  _f_sim = _introduction_probability;
}

/*--------------------------------------- GRAPH STRUCTURE */

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

/*--------------------------------------- MAP DATA */

/**
 * \brief    Set the map data
 * \details  --
 * \param    double x_coord
 * \param    double y_coord
 * \param    double node_area
 * \param    double suitable_area
 * \return   \e void
 */
inline void Node::set_map_data( double x_coord, double y_coord, double node_area, double suitable_area )
{
  assert(node_area > 0.0);
  assert(suitable_area > 0.0);
  assert(suitable_area <= node_area);
  _x_coord       = x_coord;
  _y_coord       = y_coord;
  _node_area     = node_area;
  _suitable_area = suitable_area;
}

/*--------------------------------------- SAMPLING DATA */

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
  _f_obs = 0.0;
  if (_n_obs > 0.0)
  {
    _f_obs = _y_obs/_n_obs;
  }
}


#endif /* defined(__MoRIS__Node__) */
