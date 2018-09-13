/**
 * \file      Simulation.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Simulation class declaration
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

#ifndef __MoRIS__Simulation__
#define __MoRIS__Simulation__

#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "Enums.h"
#include "Prng.h"
#include "Parameters.h"
#include "Node.h"
#include "Graph.h"


class Simulation
{

public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Simulation( void ) = delete;
  Simulation( Parameters* parameters );
  Simulation( const Simulation& sim ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Simulation( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  inline int    get_iteration( void ) const;
  inline double get_empty_score( void ) const;
  inline double get_score( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Simulation& operator=(const Simulation&) = delete;
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void compute_next_iteration( void );
  void compute_score( void );
  void write_state( std::string filename );
  
  /*----------------------------
   * PUBLIC ATTRIBUTES
   *----------------------------*/
  
protected:
  
  /*----------------------------
   * PROTECTED METHODS
   *----------------------------*/
  int    draw_number_of_jumps( double jump_probability );
  double draw_jump_size( void );
  double compute_euclidean_distance( Node* node1, Node* node2 );
  
  /*----------------------------
   * PROTECTED ATTRIBUTES
   *----------------------------*/
  
  Parameters* _parameters; /*!< Main parameters   */
  Graph*      _graph;      /*!< Graph structure   */
  int         _iteration;  /*!< Current iteration */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/**
 * \brief    Get current iteration
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Simulation::get_iteration( void ) const
{
  return _iteration;
}

/**
 * \brief    Get the optimization score when map is empty
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Simulation::get_empty_score( void ) const
{
  return _graph->get_empty_score();
}

/**
 * \brief    Get the optimization score
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Simulation::get_score( void ) const
{
  return _graph->get_score();
}

/*----------------------------
 * SETTERS
 *----------------------------*/


#endif /* defined(__MoRIS__Simulation__) */
