/**
 * \file      Parameters.h
 * \author    Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * \date      15-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2019 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Parameters class declaration
 */

/****************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright (c) 2014-2019 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
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

#ifndef __MoRIS__Parameters__
#define __MoRIS__Parameters__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "Prng.h"
#include "Enums.h"


class Parameters
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Parameters( void );
  Parameters( const Parameters& parameters ) = delete;
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Parameters( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  inline Prng*             get_prng( void );
  inline unsigned long int get_prng_seed( void ) const;
  
  /*------------------------------------------------------------------ Input data filenames */
  
  inline std::string get_map_filename( void ) const;
  inline std::string get_network_filename( void ) const;
  inline std::string get_sample_filename( void ) const;
  
  /*------------------------------------------------------------------ Main parameters */
  
  inline type_of_data          get_typeofdata( void ) const;
  inline int                   get_repetitions( void ) const;
  inline int                   get_iterations( void ) const;
  inline jump_distribution_law get_jump_law( void ) const;
  inline optimization_function get_optimization_function( void ) const;
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  inline double get_x_introduction( void ) const;
  inline double get_y_introduction( void ) const;
  inline double get_p_introduction( void ) const;
  inline double get_lambda( void ) const;
  inline double get_mu( void ) const;
  inline double get_sigma( void ) const;
  inline double get_gamma( void ) const;
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  inline double get_w1( void ) const;
  inline double get_w2( void ) const;
  inline double get_w3( void ) const;
  inline double get_w4( void ) const;
  inline double get_w5( void ) const;
  inline double get_w6( void ) const;
  inline double get_wmin( void ) const;
  
  /*------------------------------------------------------------------ Extra statistics */
  
  inline bool get_save_outputs( void ) const;
  inline bool saveOutputs( void ) const;
  inline bool get_save_all_states( void ) const;
  inline bool saveAllStates( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Parameters& operator=(const Parameters&) = delete;
  
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  inline void set_prng_seed( unsigned long int seed );
  
  /*------------------------------------------------------------------ Input data filenames */
  
  inline void set_map_filename( std::string filename );
  inline void set_network_filename( std::string filename );
  inline void set_sample_filename( std::string filename );
  
  /*------------------------------------------------------------------ Main parameters */
  
  inline void set_typeofdata( type_of_data data );
  inline void set_repetitions( int repetitions );
  inline void set_iterations( int iterations );
  inline void set_jump_law( jump_distribution_law jump_law );
  inline void set_optimization_function( optimization_function optim_function );
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  inline void set_x_introduction( double x );
  inline void set_y_introduction( double y );
  inline void set_p_introduction( double p );
  inline void set_lambda( double lambda );
  inline void set_mu( double mu );
  inline void set_sigma( double sigma );
  inline void set_gamma( double gamma );
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  inline void set_w1( double weight );
  inline void set_w2( double weight );
  inline void set_w3( double weight );
  inline void set_w4( double weight );
  inline void set_w5( double weight );
  inline void set_w6( double weight );
  inline void set_wmin( double weight );
  
  /*------------------------------------------------------------------ Extra statistics */
  
  inline void set_save_outputs( bool save_outputs );
  inline void set_save_all_states( bool save_all_states );
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void write_parameters( std::string filename );
  
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
  
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  Prng*             _prng; /*!< Pseudorandom numbers generator */
  unsigned long int _seed; /*!< Prng seed                      */
  
  /*------------------------------------------------------------------ Data filenames */
  
  std::string _map_filename;     /*!< Map filename     */
  std::string _network_filename; /*!< Network filename */
  std::string _sample_filename;  /*!< Sample filename  */
  
  /*------------------------------------------------------------------ Main parameters */
  
  type_of_data          _data;           /*!< Type of experimental data           */
  int                   _repetitions;    /*!< Number of repetitions by simulation */
  int                   _iterations;     /*!< Number of iterations by simulation  */
  jump_distribution_law _jump_law;       /*!< Jump distribution law               */
  optimization_function _optim_function; /*!< Optimization function               */
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  double _x_introduction; /*!< X coordinate of the introduction node */
  double _y_introduction; /*!< Y coordinate of the introduction node */
  double _p_introduction; /*!< Introduction probability              */
  double _lambda;         /*!< Lambda                                */
  double _mu;             /*!< Mu                                    */
  double _sigma;          /*!< Sigma                                 */
  double _gamma;          /*!< Gamma                                 */
  
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  double _w1;   /*!< Category I roads weight      */
  double _w2;   /*!< Category II roads weight     */
  double _w3;   /*!< Category III roads weight    */
  double _w4;   /*!< Category IV roads weight     */
  double _w5;   /*!< Category V roads weight      */
  double _w6;   /*!< Category VI roads weight     */
  double _wmin; /*!< Minimal weight between nodes */
  
  /*------------------------------------------------------------------ Extra statistics */
  
  bool _save_outputs;    /*!< Save simulation outputs    */
  bool _save_all_states; /*!< Save all simulation states */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Pseudorandom numbers generator */

/**
 * \brief    Get the pseudorandom numbers generator
 * \details  --
 * \param    void
 * \return   \e Prng*
 */
inline Prng* Parameters::get_prng( void )
{
  return _prng;
}

/**
 * \brief    Get the prng seed
 * \details  --
 * \param    void
 * \return   \e Prng*
 */
inline unsigned long int Parameters::get_prng_seed( void ) const
{
  return _seed;
}

/*------------------------------------------------------------------ Input data filenames */

/**
 * \brief    Get the map filename
 * \details  --
 * \param    void
 * \return   \e std::string
 */
inline std::string Parameters::get_map_filename( void ) const
{
  return _map_filename;
}

/**
 * \brief    Get the network filename
 * \details  --
 * \param    void
 * \return   \e std::string
 */
inline std::string Parameters::get_network_filename( void ) const
{
  return _network_filename;
}

/**
 * \brief    Get the sample filename
 * \details  --
 * \param    void
 * \return   \e std::string
 */
inline std::string Parameters::get_sample_filename( void ) const
{
  return _sample_filename;
}

/*------------------------------------------------------------------ Main parameters */

/**
 * \brief    Get the type of experimental data
 * \details  --
 * \param    void
 * \return   \e type_of_data
 */
inline type_of_data Parameters::get_typeofdata( void ) const
{
  return _data;
}

/**
 * \brief    Get the number of repetitions by simulation
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Parameters::get_repetitions( void ) const
{
  return _repetitions;
}

/**
 * \brief    Get the number of iterations
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Parameters::get_iterations( void ) const
{
  return _iterations;
}

/**
 * \brief    Get jump distribution law
 * \details  --
 * \param    void
 * \return   \e distribution_law
 */
inline jump_distribution_law Parameters::get_jump_law( void ) const
{
  return _jump_law;
}

/**
 * \brief    Get the optimization function
 * \details  --
 * \param    void
 * \return   \e optimization_function
 */
inline optimization_function Parameters::get_optimization_function( void ) const
{
  return _optim_function;
}

/*------------------------------------------------------------------ Simulation parameters */

/**
 * \brief    Get the X coordinate of the introduction node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_x_introduction( void ) const
{
  return _x_introduction;
}

/**
 * \brief    Get the Y coordinate of the introduction node
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_y_introduction( void ) const
{
  return _y_introduction;
}

/**
 * \brief    Get the introduction prevalence
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_p_introduction( void ) const
{
  return _p_introduction;
}

/**
 * \brief    Get lambda parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_lambda( void ) const
{
  return _lambda;
}

/**
 * \brief    Get mu parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_mu( void ) const
{
  return _mu;
}

/**
 * \brief    Get sigma parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_sigma( void ) const
{
  return _sigma;
}

/**
 * \brief    Get gamma parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_gamma( void ) const
{
  return _gamma;
}

/*------------------------------------------------------------------ Linear combination of road categories */

/**
 * \brief    Get the road category I weight
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_w1( void ) const
{
  return _w1;
}

/**
 * \brief    Get the road category II weight
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_w2( void ) const
{
  return _w2;
}

/**
 * \brief    Get the road category III weight
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_w3( void ) const
{
  return _w3;
}

/**
 * \brief    Get the road category IV weight
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_w4( void ) const
{
  return _w4;
}

/**
 * \brief    Get the road category V weight
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_w5( void ) const
{
  return _w5;
}

/**
 * \brief    Get the road category VI weight
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_w6( void ) const
{
  return _w6;
}

/**
 * \brief    Get the minimal weight between nodes
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_wmin( void ) const
{
  return _wmin;
}

/*------------------------------------------------------------------ Extra statistics */

/**
 * \brief    Get the save simulation outputs boolean
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Parameters::get_save_outputs( void ) const
{
  return _save_outputs;
}

/**
 * \brief    Save outputs?
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Parameters::saveOutputs( void ) const
{
  return _save_outputs;
}

/**
 * \brief    Get save simulation state at all times
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Parameters::get_save_all_states( void ) const
{
  return _save_all_states;
}

/**
 * \brief    Save simulation state at all times?
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Parameters::saveAllStates( void ) const
{
  return _save_all_states;
}

/*----------------------------
 * SETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Pseudorandom numbers generator */

/**
 * \brief    Set the prng seed
 * \details  --
 * \param    unsigned long int seed
 * \return   \e void
 */
inline void Parameters::set_prng_seed( unsigned long int seed )
{
  _seed = seed;
  _prng->set_seed(seed);
}

/*------------------------------------------------------------------ Input data filenames */

/**
 * \brief    Set map filename
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
inline void Parameters::set_map_filename( std::string filename )
{
  _map_filename = std::string(filename);
}

/**
 * \brief    Set network filename
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
inline void Parameters::set_network_filename( std::string filename )
{
  _network_filename = std::string(filename);
}

/**
 * \brief    Set sample filename
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
inline void Parameters::set_sample_filename( std::string filename )
{
  _sample_filename = std::string(filename);
}

/*------------------------------------------------------------------ Main parameters */

/**
 * \brief    Set the type of experimental data
 * \details  --
 * \param    type_of_data data
 * \return   \e void
 */
inline void Parameters::set_typeofdata( type_of_data data )
{
  _data = data;
}

/**
 * \brief    Set the number of repetitions by simulation
 * \details  --
 * \param    int repetitions
 * \return   \e void
 */
inline void Parameters::set_repetitions( int repetitions )
{
  assert(repetitions > 0);
  _repetitions = repetitions;
}

/**
 * \brief    Set the number of iterations
 * \details  --
 * \param    int iterations
 * \return   \e void
 */
inline void Parameters::set_iterations( int iterations )
{
  assert(iterations > 0);
  _iterations = iterations;
}

/**
 * \brief    Set jump law
 * \details  --
 * \param    jump_distribution_law jump_law
 * \return   \e void
 */
inline void Parameters::set_jump_law( jump_distribution_law jump_law )
{
  _jump_law = jump_law;
}

/**
 * \brief    Set the optimization function
 * \details  --
 * \param    optimization_function optim_function
 * \return   \e void
 */
inline void Parameters::set_optimization_function( optimization_function optim_function )
{
  _optim_function = optim_function;
}

/*------------------------------------------------------------------ Simulation parameters */

/**
 * \brief    Set X coordinate of the introduction node
 * \details  --
 * \param    double x
 * \return   \e void
 */
inline void Parameters::set_x_introduction( double x )
{
  _x_introduction = x;
}

/**
 * \brief    Set Y coordinate of the introduction node
 * \details  --
 * \param    double y
 * \return   \e void
 */
inline void Parameters::set_y_introduction( double y )
{
  _y_introduction = y;
}

/**
 * \brief    Set the introduction prevalence
 * \details  --
 * \param    double p
 * \return   \e void
 */
inline void Parameters::set_p_introduction( double p )
{
  assert(p >= 0.0);
  assert(p <= 1.0);
  _p_introduction = p;
}

/**
 * \brief    Set lambda
 * \details  --
 * \param    double lambda
 * \return   \e void
 */
inline void Parameters::set_lambda( double lambda )
{
  assert(lambda >= 0.0);
  _lambda = lambda;
}

/**
 * \brief    Set mu
 * \details  --
 * \param    double mu
 * \return   \e void
 */
inline void Parameters::set_mu( double mu )
{
  assert(mu >= 0.0);
  _mu = mu;
}

/**
 * \brief    Set sigma
 * \details  --
 * \param    double sigma
 * \return   \e void
 */
inline void Parameters::set_sigma( double sigma )
{
  assert(sigma >= 0.0);
  _sigma = sigma;
}

/**
 * \brief    Set gamma
 * \details  --
 * \param    double gamma
 * \return   \e void
 */
inline void Parameters::set_gamma( double gamma )
{
  assert(gamma >= 0.0);
  _gamma = gamma;
}

/*------------------------------------------------------------------ Linear combination of road categories */

/**
 * \brief    Set roads category I weight
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_w1( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _w1 = weight;
}

/**
 * \brief    Set roads category II weight
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_w2( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _w2 = weight;
}

/**
 * \brief    Set roads category III weight
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_w3( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _w3 = weight;
}

/**
 * \brief    Set roads category IV weight
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_w4( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _w4 = weight;
}

/**
 * \brief    Set roads category V weight
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_w5( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _w5 = weight;
}

/**
 * \brief    Set roads category VI weight
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_w6( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _w6 = weight;
}

/**
 * \brief    Set minimal weight between nodes
 * \details  --
 * \param    double weight
 * \return   \e void
 */
inline void Parameters::set_wmin( double weight )
{
  assert(weight >= 0.0);
  assert(weight <= 1.0);
  _wmin = weight;
}

/*------------------------------------------------------------------ Extra statistics */

/**
 * \brief    Set save simulation outputs state boolean
 * \details  --
 * \param    bool save_outputs
 * \return   \e void
 */
inline void Parameters::set_save_outputs( bool save_outputs )
{
  _save_outputs = save_outputs;
}

/**
 * \brief    Set save simulation state at all times
 * \details  --
 * \param    bool save_all_states
 * \return   \e void
 */
inline void Parameters::set_save_all_states( bool save_all_states )
{
  _save_all_states = save_all_states;
}


#endif /* defined(__MoRIS__Parameters__) */
