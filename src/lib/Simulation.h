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
#include "Node.h"
#include "Graph.h"


class Simulation
{

public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Simulation( void ) = delete;
  Simulation( Prng* prng, type_of_data data, std::string network_filename, std::string map_filename, std::string sample_filename, int introduction_node, double lambda, double mu, double sigma, jump_distribution_law jump_law, int nb_repetitions, std::vector<double>* road_linear_combination, double minimal_connectivity, bool save_lineage_tree );
  Simulation( const Simulation& sim ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Simulation( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  inline Graph* get_graph( void );
  inline int    get_iteration( void ) const;
  inline double get_minimization_score( void );
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Simulation& operator=(const Simulation&) = delete;
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void update( bool last_iteration );
  
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
  Prng*                 _prng;              /*!< Pseudorandom numbers generator */
  Graph*                _graph;             /*!< Graph                          */
  int                   _iteration;         /*!< Current iteration              */
  int                   _nb_repetitions;    /*!< Number of repetitions          */
  int                   _n;                 /*!< Number of nodes                */
  int                   _introduction_node; /*!< Introduction node              */
  double                _lambda;            /*!< Mean of Poisson law            */
  double                _mu;                /*!< Mean of jump length            */
  double                _sigma;             /*!< Variance of jump length        */
  jump_distribution_law _jump_law;          /*!< Jump distribution law          */
  bool                  _save_lineage_tree; /*!< Save the lineage tree          */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/**
 * \brief    Get the graph
 * \details  --
 * \param    void
 * \return   \e Graph*
 */
inline Graph* Simulation::get_graph( void )
{
  return _graph;
}

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
 * \brief    Get the minimization score
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Simulation::get_minimization_score( void )
{
  return _graph->get_minimization_score();
}

/*----------------------------
 * SETTERS
 *----------------------------*/


#endif /* defined(__MoRIS__Simulation__) */
